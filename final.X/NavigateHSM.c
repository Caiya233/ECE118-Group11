#include "BOARD.h"
#include "BeaconService.h"
#include "DriveService.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "LauncherService.h"
#include "NavigateHSM.h"
#include "ProjectConfig.h"
#include "TapeFeature.h"
#include "TapeService.h"
#include "TopHSM.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {
    NavInitPState,
    NavSpinFindBeacon,
    NavCenterBeacon,
    NavExitStartZone,
    NavSeekLeftEdge,
    NavLeftEdgeCruise,
    NavRecBackupBeforeRightShift,
    NavStrafeRight11,
    NavRecenterBeaconAfterRightShift,
    NavInteriorForward,
    NavRecBackupBeforeLeftReacquire,
    NavReacquireLeftEdgeAfterSecondBump,
    NavLeftEdgeCruiseRetry,
    NavRecBackupBeforeCrossRight,
    NavCrossToRightBoundary,
    NavRightEdgeCruise,
    NavISZSearchInterior,
    NavCreepIntoISZ,
    NavISZConfirmed,
    NavFailed
} NavState_t;

typedef enum {
    ROUTE_LEFT_PRIMARY,
    ROUTE_INNER_AFTER_RIGHT_11,
    ROUTE_LEFT_RETRY,
    ROUTE_CROSS_TO_RIGHT,
    ROUTE_RIGHT_SECONDARY
} RouteMode_t;

typedef enum {
    TAPE_GUARD_NORMAL,
    TAPE_GUARD_FRONT_WIDE_SUPPRESSED,
    TAPE_GUARD_ISZ_ARMED
} TapeGuardMode_t;

static const char *StateNames[] = {
    "NavInitPState",
    "NavSpinFindBeacon",
    "NavCenterBeacon",
    "NavExitStartZone",
    "NavSeekLeftEdge",
    "NavLeftEdgeCruise",
    "NavRecBackupBeforeRightShift",
    "NavStrafeRight11",
    "NavRecenterBeaconAfterRightShift",
    "NavInteriorForward",
    "NavRecBackupBeforeLeftReacquire",
    "NavReacquireLeftEdgeAfterSecondBump",
    "NavLeftEdgeCruiseRetry",
    "NavRecBackupBeforeCrossRight",
    "NavCrossToRightBoundary",
    "NavRightEdgeCruise",
    "NavISZSearchInterior",
    "NavCreepIntoISZ",
    "NavISZConfirmed",
    "NavFailed"
};

static const char *RouteNames[] = {
    "ROUTE_LEFT_PRIMARY",
    "ROUTE_INNER_AFTER_RIGHT_11",
    "ROUTE_LEFT_RETRY",
    "ROUTE_CROSS_TO_RIGHT",
    "ROUTE_RIGHT_SECONDARY"
};

static NavState_t CurrentState = NavInitPState;
static RouteMode_t RouteMode;
static TapeGuardMode_t TapeGuardMode;
static TapeFeatureEstimator_t TapeEstimator;
static TapeFeature_t TapeFeature;

static bool HasExitedStartZone;
static bool IszArmed;
static bool RearAnySeenDuringCreep;
static bool RearWideStableDuringCreep;
static bool RearWideActive;
static bool FrontWideLast;
static bool FrontWideRising;
static bool FrontAllBlackActive;

static uint8_t CollisionAttempt;
static uint8_t FieldMarkerCount;
static uint8_t BeaconScanRetries;
static int BeaconScanSign;
static uint8_t LastPrintedFrontMask;
static uint8_t LastPrintedRearMask;

static uint32_t NavStartTimeMs;
static uint32_t LastBumpTimeMs;
static uint32_t LastBeaconSeenTimeMs;
static uint32_t RearWideSinceMs;
static uint32_t FrontAllBlackSinceMs;

static int AbsInt(int value)
{
    if (value < 0) {
        return -value;
    }
    return value;
}

static void PostTopEvent(ES_EventTyp_t type)
{
    ES_Event event;
    event.EventType = type;
    event.EventParam = 0;
    PostTopHSM(event);
}

static bool IsBeaconSteeringEvent(ES_EventTyp_t eventType)
{
    return ((eventType == EV_BEACON_LOCKED) ||
            (eventType == EV_BEACON_LEFT_STRONG) ||
            (eventType == EV_BEACON_RIGHT_STRONG) ||
            (eventType == EV_BEACON_CENTERED));
}

static bool IsTapeEvent(ES_EventTyp_t eventType)
{
    return ((eventType == EV_TAPE_MASK_CHANGED) ||
            (eventType == EV_FRONT_WIDE_BLACK) ||
            (eventType == EV_FRONT_ALL_BLACK) ||
            (eventType == EV_TAPE_FRONT_CROSS) ||
            (eventType == EV_TAPE_REAR_CROSS) ||
            (eventType == EV_TAPE_LOST) ||
            (eventType == EV_TAPE_FRONT_L) ||
            (eventType == EV_TAPE_FRONT_LM) ||
            (eventType == EV_TAPE_FRONT_M) ||
            (eventType == EV_TAPE_FRONT_MR) ||
            (eventType == EV_TAPE_FRONT_R) ||
            (eventType == EV_TAPE_REAR_L) ||
            (eventType == EV_TAPE_REAR_R));
}

static bool IsRecoveryState(NavState_t state)
{
    return ((state == NavRecBackupBeforeRightShift) ||
            (state == NavStrafeRight11) ||
            (state == NavRecenterBeaconAfterRightShift) ||
            (state == NavRecBackupBeforeLeftReacquire) ||
            (state == NavReacquireLeftEdgeAfterSecondBump) ||
            (state == NavRecBackupBeforeCrossRight) ||
            (state == NavCrossToRightBoundary) ||
            (state == NavCreepIntoISZ));
}

static void RefreshTapeFeature(void)
{
    uint8_t frontMask = Tape_GetFrontMask();
    uint8_t rearMask = Tape_GetRearMask();
    uint32_t nowMs = ES_Timer_GetTime();

    TapeFeature_Update(&TapeEstimator, frontMask, rearMask, nowMs, &TapeFeature);

    FrontWideRising = (TapeFeature.frontWideBlack && !FrontWideLast);
    FrontWideLast = TapeFeature.frontWideBlack;

    if (TapeFeature.frontAllBlack) {
        if (!FrontAllBlackActive) {
            FrontAllBlackActive = true;
            FrontAllBlackSinceMs = nowMs;
        }
    } else {
        FrontAllBlackActive = false;
        FrontAllBlackSinceMs = nowMs;
    }

    if ((frontMask != LastPrintedFrontMask) || (rearMask != LastPrintedRearMask)) {
        LastPrintedFrontMask = frontMask;
        LastPrintedRearMask = rearMask;
        printf("\r\nTape masks: front=0x%02x rear=0x%02x Llock=%u Rlock=%u "
               "Ldanger=%u Rdanger=%u wide=%u all=%u rearAny=%u rearWide=%u\r\n",
               frontMask, rearMask,
               (unsigned int)TapeFeature.leftEdgeLocked,
               (unsigned int)TapeFeature.rightEdgeLocked,
               (unsigned int)TapeFeature.leftBoundaryDanger,
               (unsigned int)TapeFeature.rightBoundaryDanger,
               (unsigned int)TapeFeature.frontWideBlack,
               (unsigned int)TapeFeature.frontAllBlack,
               (unsigned int)TapeFeature.rearAnyBlack,
               (unsigned int)TapeFeature.rearWideBlack);
    }
}

static void UpdateIszArmed(void)
{
    bool armed = (HasExitedStartZone &&
                  ((uint32_t)(ES_Timer_GetTime() - NavStartTimeMs) >=
                   NAV_MIN_ISZ_TIME_MS) &&
                  (TapeGuardMode != TAPE_GUARD_FRONT_WIDE_SUPPRESSED) &&
                  !IsRecoveryState(CurrentState));

    IszArmed = armed;
    if (TapeGuardMode != TAPE_GUARD_FRONT_WIDE_SUPPRESSED) {
        TapeGuardMode = armed ? TAPE_GUARD_ISZ_ARMED : TAPE_GUARD_NORMAL;
    }
}

static int Navigate_GetBeaconYawCorrection(void)
{
    int error;
    int correction;

    if (Beacon_IsLocked() == FALSE) {
        if ((uint32_t)(ES_Timer_GetTime() - LastBeaconSeenTimeMs) <= BEACON_LOST_MS) {
            return 0;
        }
        return 0;
    }

    LastBeaconSeenTimeMs = ES_Timer_GetTime();
    error = Beacon_GetBearingError();
    correction = (error * BEACON_YAW_GAIN_NUM) / BEACON_YAW_GAIN_DEN;

    if (correction > NAV_YAW_CORRECT_MAX) {
        correction = NAV_YAW_CORRECT_MAX;
    } else if (correction < -NAV_YAW_CORRECT_MAX) {
        correction = -NAV_YAW_CORRECT_MAX;
    }

    return correction;
}

static bool BeaconIsCenteredEnough(void)
{
    return ((Beacon_IsLocked() != FALSE) &&
            (AbsInt(Beacon_GetBearingError()) <= BEACON_CENTER_TOL));
}

static void DriveSpinScan(void)
{
    Drive_SetBodyVelocity(0, 0, BeaconScanSign * NAV_YAW_SCAN_SPEED);
}

static void DriveCenterBeacon(void)
{
    Drive_SetBodyVelocity(0, 0, Navigate_GetBeaconYawCorrection());
}

static void DriveForwardWithBeacon(int forward)
{
    Drive_SetBodyVelocity(forward, 0, Navigate_GetBeaconYawCorrection());
}

static void DriveSeekLeftEdge(void)
{
    Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                          NAV_STRAFE_LEFT_SEARCH,
                          Navigate_GetBeaconYawCorrection());
}

static void DriveLeftEdgeControl(void)
{
    if (TapeFeature.leftBoundaryDanger) {
        Drive_SetBodyVelocity(0,
                              STRAFE_INWARD_FROM_LEFT,
                              Navigate_GetBeaconYawCorrection());
    } else if (TapeFeature.leftEdgeLocked) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD,
                              0,
                              Navigate_GetBeaconYawCorrection());
    } else if (TapeFeature.leftEdgeSoftLost) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                              NAV_STRAFE_LEFT_SEARCH / 2,
                              Navigate_GetBeaconYawCorrection());
    } else {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                              NAV_STRAFE_LEFT_SEARCH,
                              Navigate_GetBeaconYawCorrection());
    }
}

static void DriveRightEdgeControl(void)
{
    if (TapeFeature.rightBoundaryDanger) {
        Drive_SetBodyVelocity(0,
                              STRAFE_INWARD_FROM_RIGHT,
                              Navigate_GetBeaconYawCorrection());
    } else if (TapeFeature.rightEdgeLocked) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD,
                              0,
                              Navigate_GetBeaconYawCorrection());
    } else if (TapeFeature.rightEdgeSoftLost) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                              NAV_STRAFE_RIGHT_SEARCH / 2,
                              Navigate_GetBeaconYawCorrection());
    } else {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                              NAV_STRAFE_RIGHT_SEARCH,
                              Navigate_GetBeaconYawCorrection());
    }
}

static void DriveCrossToRight(void)
{
    Drive_SetBodyVelocity(CROSS_TO_RIGHT_FORWARD_BIAS,
                          CROSS_TO_RIGHT_STRAFE_SPEED,
                          Navigate_GetBeaconYawCorrection());
}

static void EnterNavState(const char *name)
{
    printf("\r\nNav state: %s route=%s guard=%u bumps=%u\r\n",
           name, RouteNames[RouteMode], (unsigned int)TapeGuardMode,
           (unsigned int)CollisionAttempt);
}

static bool HandleFrontTapeCandidate(NavState_t *nextState,
                                     uint8_t *makeTransition)
{
    if (TapeFeature.frontWideBlack) {
        if (TapeGuardMode == TAPE_GUARD_FRONT_WIDE_SUPPRESSED) {
            return false;
        }
        if (IszArmed) {
            printf("\r\nNav event: ISZ candidate from front-wide tape\r\n");
            PostTopEvent(EV_ISZ_ENTRY_CANDIDATE);
            *nextState = NavCreepIntoISZ;
            *makeTransition = TRUE;
            return true;
        }
        if (FrontWideRising) {
            FieldMarkerCount++;
            printf("\r\nNav event: field marker/front-wide count=%u\r\n",
                   (unsigned int)FieldMarkerCount);
            PostTopEvent(EV_FIELD_MARKER_CROSS);
        }
    }

    if (TapeFeature.frontAllBlack &&
        (TapeGuardMode != TAPE_GUARD_FRONT_WIDE_SUPPRESSED) &&
        !IszArmed &&
        FrontAllBlackActive &&
        ((uint32_t)(ES_Timer_GetTime() - FrontAllBlackSinceMs) >=
         FRONT_ALL_BLACK_PANIC_MS)) {
        printf("\r\nNav event: front-all-black panic\r\n");
        PostTopEvent(EV_TAPE_PANIC);
        *nextState = NavISZSearchInterior;
        *makeTransition = TRUE;
        return true;
    }

    return false;
}

static void UpdateCreepRearConfirmation(void)
{
    uint32_t nowMs = ES_Timer_GetTime();

    if (TapeFeature.rearAnyBlack && !RearAnySeenDuringCreep) {
        RearAnySeenDuringCreep = true;
        ES_Timer_InitTimer(ISZ_EXTRA_CREEP_TIMER, ISZ_EXTRA_CREEP_MS);
    }

    if (TapeFeature.rearWideBlack) {
        if (!RearWideActive) {
            RearWideActive = true;
            RearWideSinceMs = nowMs;
        } else if ((uint32_t)(nowMs - RearWideSinceMs) >=
                   REAR_WIDE_CONFIRM_MS) {
            RearWideStableDuringCreep = true;
        }
    } else {
        RearWideActive = false;
        RearWideSinceMs = nowMs;
    }
}

uint8_t InitNavigateHSM(void)
{
    CurrentState = NavInitPState;
    RouteMode = ROUTE_LEFT_PRIMARY;
    TapeGuardMode = TAPE_GUARD_NORMAL;
    TapeFeature_Reset(&TapeEstimator);
    HasExitedStartZone = false;
    IszArmed = false;
    RearAnySeenDuringCreep = false;
    RearWideStableDuringCreep = false;
    RearWideActive = false;
    FrontWideLast = false;
    FrontWideRising = false;
    FrontAllBlackActive = false;
    CollisionAttempt = 0u;
    FieldMarkerCount = 0u;
    BeaconScanRetries = 0u;
    BeaconScanSign = 1;
    LastPrintedFrontMask = 0xffu;
    LastPrintedRearMask = 0xffu;
    NavStartTimeMs = ES_Timer_GetTime();
    LastBumpTimeMs = 0u;
    LastBeaconSeenTimeMs = ES_Timer_GetTime();
    RearWideSinceMs = 0u;
    FrontAllBlackSinceMs = 0u;
    RefreshTapeFeature();
    return (RunNavigateHSM(INIT_EVENT).EventType == ES_NO_EVENT);
}

ES_Event RunNavigateHSM(ES_Event event)
{
    ES_Event ThisEvent = event;
    uint8_t makeTransition = FALSE;
    NavState_t nextState = CurrentState;

    ES_Tattle();

    RefreshTapeFeature();

    if (IsBeaconSteeringEvent(ThisEvent.EventType)) {
        LastBeaconSeenTimeMs = ES_Timer_GetTime();
    }

    if ((ThisEvent.EventType == ES_TIMEOUT) &&
        (ThisEvent.EventParam == NAV_TOTAL_GUARD_TIMER) &&
        (CurrentState != NavFailed) &&
        (CurrentState != NavISZConfirmed)) {
        nextState = NavFailed;
        makeTransition = TRUE;
        event.EventType = ES_NO_EVENT;
    }

    UpdateIszArmed();

    if (makeTransition == FALSE) {
        switch (CurrentState) {
        case NavInitPState:
            if (ThisEvent.EventType == ES_INIT) {
                ES_Timer_InitTimer(NAV_TOTAL_GUARD_TIMER, NAV_MAX_TOTAL_TIME_MS);
                nextState = NavSpinFindBeacon;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavSpinFindBeacon:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveSpinScan();
                ES_Timer_InitTimer(NAV_BEACON_SCAN_TIMER, NAV_BEACON_SCAN_MS);
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BEACON_LOCKED) {
                printf("\r\nNav event: beacon locked as heading cue\r\n");
                nextState = NavCenterBeacon;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_BEACON_SCAN_TIMER)) {
                BeaconScanRetries++;
                BeaconScanSign = -BeaconScanSign;
                if (BeaconScanRetries >= NAV_BEACON_SCAN_MAX_RETRIES) {
                    nextState = NavFailed;
                    makeTransition = TRUE;
                } else {
                    DriveSpinScan();
                    ES_Timer_InitTimer(NAV_BEACON_SCAN_TIMER, NAV_BEACON_SCAN_MS);
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavCenterBeacon:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveCenterBeacon();
                ES_Timer_InitTimer(REC_RECENTER_TIMER, REC_RECENTER_TIMEOUT_MS);
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BEACON_LOST) {
                nextState = NavSpinFindBeacon;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsBeaconSteeringEvent(ThisEvent.EventType)) {
                if ((ThisEvent.EventType == EV_BEACON_CENTERED) ||
                    BeaconIsCenteredEnough()) {
                    printf("\r\nNav event: beacon centered; heading canonicalized\r\n");
                    nextState = NavExitStartZone;
                    makeTransition = TRUE;
                } else {
                    DriveCenterBeacon();
                }
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_RECENTER_TIMER)) {
                if (BeaconIsCenteredEnough()) {
                    nextState = NavExitStartZone;
                    makeTransition = TRUE;
                } else {
                    DriveCenterBeacon();
                    ES_Timer_InitTimer(REC_RECENTER_TIMER, REC_RECENTER_TIMEOUT_MS);
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavExitStartZone:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                HasExitedStartZone = false;
                DriveForwardWithBeacon(NAV_EXIT_FORWARD_SPEED);
                ES_Timer_InitTimer(NAV_EXIT_START_TIMER, NAV_EXIT_START_MS);
                event.EventType = ES_NO_EVENT;
            } else if (IsBeaconSteeringEvent(ThisEvent.EventType)) {
                DriveForwardWithBeacon(NAV_EXIT_FORWARD_SPEED);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_EXIT_START_TIMER)) {
                HasExitedStartZone = true;
                nextState = NavSeekLeftEdge;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavSeekLeftEdge:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                RouteMode = ROUTE_LEFT_PRIMARY;
                DriveSeekLeftEdge();
                ES_Timer_InitTimer(NAV_EDGE_SEEK_TIMER, NAV_EDGE_SEEK_TIMEOUT_MS);
                event.EventType = ES_NO_EVENT;
            } else if (TapeFeature.leftEdgeLocked) {
                RouteMode = ROUTE_LEFT_PRIMARY;
                nextState = NavLeftEdgeCruise;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType)) {
                DriveSeekLeftEdge();
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_EDGE_SEEK_TIMER)) {
                RouteMode = ROUTE_LEFT_PRIMARY;
                nextState = NavLeftEdgeCruise;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavLeftEdgeCruise:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                RouteMode = ROUTE_LEFT_PRIMARY;
                DriveLeftEdgeControl();
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BUMPER_HIT) {
                LastBumpTimeMs = ES_Timer_GetTime();
                CollisionAttempt = 1u;
                RouteMode = ROUTE_INNER_AFTER_RIGHT_11;
                nextState = NavRecBackupBeforeRightShift;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (HandleFrontTapeCandidate(&nextState, &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType)) {
                DriveLeftEdgeControl();
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecBackupBeforeRightShift:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_SetBodyVelocity(REC_BACKUP_SPEED, 0, 0);
                ES_Timer_InitTimer(REC_BACKUP_TIMER, REC_BACKUP_MS);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_BACKUP_TIMER)) {
                nextState = NavStrafeRight11;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavStrafeRight11:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_SetBodyVelocity(0, REC_STRAFE_11IN_SPEED, 0);
                ES_Timer_InitTimer(REC_STRAFE_11IN_TIMER, REC_RIGHT_11IN_MS);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_STRAFE_11IN_TIMER)) {
                nextState = NavRecenterBeaconAfterRightShift;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecenterBeaconAfterRightShift:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveCenterBeacon();
                ES_Timer_InitTimer(REC_RECENTER_TIMER, REC_RECENTER_TIMEOUT_MS);
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BEACON_LOST) {
                nextState = NavSpinFindBeacon;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == EV_BEACON_CENTERED) ||
                       (IsBeaconSteeringEvent(ThisEvent.EventType) &&
                        BeaconIsCenteredEnough()) ||
                       ((ThisEvent.EventType == ES_TIMEOUT) &&
                        (ThisEvent.EventParam == REC_RECENTER_TIMER))) {
                RouteMode = ROUTE_INNER_AFTER_RIGHT_11;
                nextState = NavInteriorForward;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsBeaconSteeringEvent(ThisEvent.EventType)) {
                DriveCenterBeacon();
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavInteriorForward:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                RouteMode = ROUTE_INNER_AFTER_RIGHT_11;
                DriveForwardWithBeacon(NAV_EDGE_FORWARD);
                ES_Timer_InitTimer(NAV_ISZ_SEARCH_TIMER,
                                   NAV_INTERIOR_FORWARD_TIMEOUT_MS);
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BUMPER_HIT) {
                LastBumpTimeMs = ES_Timer_GetTime();
                CollisionAttempt = 2u;
                RouteMode = ROUTE_LEFT_RETRY;
                nextState = NavRecBackupBeforeLeftReacquire;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (HandleFrontTapeCandidate(&nextState, &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            } else if (IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsTapeEvent(ThisEvent.EventType)) {
                DriveForwardWithBeacon(NAV_EDGE_FORWARD);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_ISZ_SEARCH_TIMER)) {
                nextState = NavISZSearchInterior;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecBackupBeforeLeftReacquire:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_SetBodyVelocity(REC_BACKUP_SPEED, 0, 0);
                ES_Timer_InitTimer(REC_BACKUP_TIMER, REC_BACKUP_MS);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_BACKUP_TIMER)) {
                nextState = NavReacquireLeftEdgeAfterSecondBump;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavReacquireLeftEdgeAfterSecondBump:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveSeekLeftEdge();
                ES_Timer_InitTimer(LEFT_REACQUIRE_TIMER,
                                   REC_LEFT_REACQUIRE_TIMEOUT_MS);
                event.EventType = ES_NO_EVENT;
            } else if (TapeFeature.leftEdgeLocked) {
                nextState = NavLeftEdgeCruiseRetry;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType)) {
                DriveSeekLeftEdge();
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == LEFT_REACQUIRE_TIMER)) {
                nextState = NavLeftEdgeCruiseRetry;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavLeftEdgeCruiseRetry:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                RouteMode = ROUTE_LEFT_RETRY;
                DriveLeftEdgeControl();
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BUMPER_HIT) {
                LastBumpTimeMs = ES_Timer_GetTime();
                CollisionAttempt = 3u;
                RouteMode = ROUTE_CROSS_TO_RIGHT;
                nextState = NavRecBackupBeforeCrossRight;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (HandleFrontTapeCandidate(&nextState, &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType)) {
                DriveLeftEdgeControl();
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecBackupBeforeCrossRight:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_SetBodyVelocity(REC_BACKUP_SPEED, 0, 0);
                ES_Timer_InitTimer(REC_BACKUP_TIMER, REC_BACKUP_MS);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_BACKUP_TIMER)) {
                nextState = NavCrossToRightBoundary;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavCrossToRightBoundary:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                RouteMode = ROUTE_CROSS_TO_RIGHT;
                TapeGuardMode = TAPE_GUARD_FRONT_WIDE_SUPPRESSED;
                DriveCrossToRight();
                ES_Timer_InitTimer(CROSS_TO_RIGHT_SUPPRESS_TIMER,
                                   CROSS_TO_RIGHT_SUPPRESS_MS);
                ES_Timer_InitTimer(CROSS_TO_RIGHT_MAX_TIMER,
                                   CROSS_TO_RIGHT_MAX_MS);
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BUMPER_HIT) {
                LastBumpTimeMs = ES_Timer_GetTime();
                nextState = NavISZSearchInterior;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (TapeFeature.rightEdgeLocked) {
                TapeGuardMode = TAPE_GUARD_NORMAL;
                RouteMode = ROUTE_RIGHT_SECONDARY;
                nextState = NavRightEdgeCruise;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == CROSS_TO_RIGHT_SUPPRESS_TIMER)) {
                TapeGuardMode = TAPE_GUARD_NORMAL;
                DriveCrossToRight();
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == CROSS_TO_RIGHT_MAX_TIMER)) {
                TapeGuardMode = TAPE_GUARD_NORMAL;
                nextState = NavISZSearchInterior;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (HandleFrontTapeCandidate(&nextState, &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType)) {
                DriveCrossToRight();
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRightEdgeCruise:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                RouteMode = ROUTE_RIGHT_SECONDARY;
                TapeGuardMode = TAPE_GUARD_NORMAL;
                DriveRightEdgeControl();
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BUMPER_HIT) {
                LastBumpTimeMs = ES_Timer_GetTime();
                nextState = NavISZSearchInterior;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (HandleFrontTapeCandidate(&nextState, &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType)) {
                DriveRightEdgeControl();
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavISZSearchInterior:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveForwardWithBeacon(NAV_EDGE_FORWARD_SLOW);
                ES_Timer_InitTimer(NAV_ISZ_SEARCH_TIMER, NAV_ISZ_SEARCH_MS);
                event.EventType = ES_NO_EVENT;
            } else if (HandleFrontTapeCandidate(&nextState, &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            } else if (IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsTapeEvent(ThisEvent.EventType)) {
                DriveForwardWithBeacon(NAV_EDGE_FORWARD_SLOW);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_ISZ_SEARCH_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavCreepIntoISZ:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                RearAnySeenDuringCreep = false;
                RearWideStableDuringCreep = false;
                RearWideActive = false;
                RearWideSinceMs = ES_Timer_GetTime();
                Drive_SetBodyVelocity(NAV_ISZ_CREEP_SPEED, 0, 0);
                ES_Timer_InitTimer(NAV_ISZ_CREEP_TIMER, NAV_ISZ_CREEP_MS);
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                UpdateCreepRearConfirmation();
                if (RearWideStableDuringCreep) {
                    nextState = NavISZConfirmed;
                    makeTransition = TRUE;
                }
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == ISZ_EXTRA_CREEP_TIMER)) {
                if (RearAnySeenDuringCreep) {
                    nextState = NavISZConfirmed;
                    makeTransition = TRUE;
                }
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_ISZ_CREEP_TIMER)) {
                UpdateCreepRearConfirmation();
                if (RearWideStableDuringCreep || RearAnySeenDuringCreep) {
                    nextState = NavISZConfirmed;
                } else {
                    nextState = NavISZSearchInterior;
                }
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavISZConfirmed:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_Stop();
                printf("\r\nNav event: ISZ confirmed\r\n");
                PostTopEvent(EV_ISZ_CONFIRMED);
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavFailed:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_Stop();
                Launcher_Stop();
                printf("\r\nNav event: failed\r\n");
                PostTopEvent(EV_NAV_FAILED);
                event.EventType = ES_NO_EVENT;
            }
            break;
        }
    }

    if (makeTransition == TRUE) {
        RunNavigateHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunNavigateHSM(ENTRY_EVENT);
    }

    ES_Tail();
    return event;
}
