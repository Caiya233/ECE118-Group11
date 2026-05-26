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
    NavBackSearchRearBoundary,
    NavRearBoundaryCaptureLeft,
    NavLeftEdgeApproachSlow,
    NavCornerExitForward,
    NavLeftEdgeLock,
    NavLeftEdgeCruise,
    NavRecBackupAfterLeftBump,
    NavStrafeRight11,
    NavRecenterBeaconAfterRightShift,
    NavFishboneAcquire,
    NavFishboneFollow,
    NavPostFishboneISZSeek,
    NavRecBackupAfterFishboneBump,
    NavReacquireLeftEdgeFromMiddle,
    NavLeftEdgeCruiseRetry,
    NavRecBackupBeforeRightFallback,
    NavCrossToRightEdge,
    NavRightEdgeApproachSlow,
    NavRightEdgeLock,
    NavRightEdgeCruise,
    NavRecBackupAfterRightFallbackBump,
    NavStrafeLeftFromRightToFishbone,
    NavFishboneAcquireFromRight,
    NavFishboneFollowFromRightRecovery,
    NavPostFishboneISZSeekFromRightRecovery,
    NavRecBackupAfterCenterRecoveryBump,
    NavReacquireLeftEdgeFinal,
    NavLeftEdgeCruiseFinal,
    NavCreepIntoISZ,
    NavISZCenterSettle,
    NavISZConfirmed,
    NavFailed
} NavState_t;

typedef enum {
    ROUTE_LEFT_PRIMARY,
    ROUTE_CENTER_FISHBONE,
    ROUTE_LEFT_RETRY,
    ROUTE_RIGHT_FALLBACK,
    ROUTE_CENTER_FROM_RIGHT_RECOVERY,
    ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY
} RouteMode_t;

typedef enum {
    ISZ_FROM_LEFT_EDGE,
    ISZ_FROM_CENTER_FISHBONE,
    ISZ_FROM_RIGHT_EDGE
} IszApproachLane_t;

typedef enum {
    TAPE_GUARD_NORMAL,
    TAPE_GUARD_FRONT_WIDE_SUPPRESSED
} TapeGuardMode_t;

typedef struct {
    bool active;
    bool stable;
    uint32_t sinceMs;
} StableFlag_t;

static const char *StateNames[] = {
    "NavInitPState",
    "NavSpinFindBeacon",
    "NavCenterBeacon",
    "NavBackSearchRearBoundary",
    "NavRearBoundaryCaptureLeft",
    "NavLeftEdgeApproachSlow",
    "NavCornerExitForward",
    "NavLeftEdgeLock",
    "NavLeftEdgeCruise",
    "NavRecBackupAfterLeftBump",
    "NavStrafeRight11",
    "NavRecenterBeaconAfterRightShift",
    "NavFishboneAcquire",
    "NavFishboneFollow",
    "NavPostFishboneISZSeek",
    "NavRecBackupAfterFishboneBump",
    "NavReacquireLeftEdgeFromMiddle",
    "NavLeftEdgeCruiseRetry",
    "NavRecBackupBeforeRightFallback",
    "NavCrossToRightEdge",
    "NavRightEdgeApproachSlow",
    "NavRightEdgeLock",
    "NavRightEdgeCruise",
    "NavRecBackupAfterRightFallbackBump",
    "NavStrafeLeftFromRightToFishbone",
    "NavFishboneAcquireFromRight",
    "NavFishboneFollowFromRightRecovery",
    "NavPostFishboneISZSeekFromRightRecovery",
    "NavRecBackupAfterCenterRecoveryBump",
    "NavReacquireLeftEdgeFinal",
    "NavLeftEdgeCruiseFinal",
    "NavCreepIntoISZ",
    "NavISZCenterSettle",
    "NavISZConfirmed",
    "NavFailed"
};

static const char *RouteNames[] = {
    "LEFT_PRIMARY",
    "CENTER_FISHBONE",
    "LEFT_RETRY",
    "RIGHT_FALLBACK",
    "CENTER_FROM_RIGHT_RECOVERY",
    "LEFT_FINAL_AFTER_RIGHT_RECOVERY"
};

static const char *LaneNames[] = {
    "LEFT",
    "CENTER",
    "RIGHT"
};

static NavState_t CurrentState = NavInitPState;
static RouteMode_t RouteMode;
static TapeGuardMode_t TapeGuardMode;
static IszApproachLane_t IszApproachLane;
static TapeFeatureEstimator_t TapeEstimator;
static TapeFeature_t TapeFeature;

static bool HasCompletedInitialLeftEdgeAcquisition;
static bool IszArmed;
static bool FishboneSeenEver;
static bool FishboneClearSeen;
static bool RearAnySeenDuringCreep;
static bool RearWideStableDuringCreep;

static uint8_t BeaconScanRetries;
static int BeaconScanSign;
static uint8_t LastPrintedFrontMask;
static uint8_t LastPrintedRearMask;

static uint32_t LastBeaconSeenTimeMs;
static uint32_t RouteArmStartTimeMs;
static uint32_t FishboneFollowStartTimeMs;
static uint32_t StateEntryTimeMs;

static StableFlag_t BeaconLockedStableFlag;
static StableFlag_t BeaconCenteredStableFlag;
static StableFlag_t FrontWideBlackStableFlag;
static StableFlag_t FrontAnyBlackStableFlag;
static StableFlag_t FrontIszCandidateStableFlag;
static StableFlag_t RearWideBlackStableFlag;
static StableFlag_t RearAnyBlackStableFlag;
static StableFlag_t RearBoundaryStableFlag;
static StableFlag_t FrontMiddleStableFlag;
static StableFlag_t FrontLeftContactStableFlag;
static StableFlag_t FrontRightContactStableFlag;
static StableFlag_t LeftFrontOffsetStableFlag;
static StableFlag_t RightFrontOffsetStableFlag;
static StableFlag_t LeftEdgeLockStableFlag;
static StableFlag_t RightEdgeLockStableFlag;
static StableFlag_t FishboneClearStableFlag;
static StableFlag_t FrontLeftStableFlag;
static StableFlag_t FrontLmStableFlag;
static StableFlag_t FrontRmStableFlag;
static StableFlag_t FrontRightStableFlag;

static int AbsInt(int value)
{
    if (value < 0) {
        return -value;
    }
    return value;
}

static void ResetStableFlag(StableFlag_t *flag)
{
    flag->active = false;
    flag->stable = false;
    flag->sinceMs = ES_Timer_GetTime();
}

static void ResetAllStableFlags(void)
{
    ResetStableFlag(&BeaconLockedStableFlag);
    ResetStableFlag(&BeaconCenteredStableFlag);
    ResetStableFlag(&FrontWideBlackStableFlag);
    ResetStableFlag(&FrontAnyBlackStableFlag);
    ResetStableFlag(&FrontIszCandidateStableFlag);
    ResetStableFlag(&RearWideBlackStableFlag);
    ResetStableFlag(&RearAnyBlackStableFlag);
    ResetStableFlag(&RearBoundaryStableFlag);
    ResetStableFlag(&FrontMiddleStableFlag);
    ResetStableFlag(&FrontLeftContactStableFlag);
    ResetStableFlag(&FrontRightContactStableFlag);
    ResetStableFlag(&LeftFrontOffsetStableFlag);
    ResetStableFlag(&RightFrontOffsetStableFlag);
    ResetStableFlag(&LeftEdgeLockStableFlag);
    ResetStableFlag(&RightEdgeLockStableFlag);
    ResetStableFlag(&FishboneClearStableFlag);
    ResetStableFlag(&FrontLeftStableFlag);
    ResetStableFlag(&FrontLmStableFlag);
    ResetStableFlag(&FrontRmStableFlag);
    ResetStableFlag(&FrontRightStableFlag);
}

static void UpdateStableFlag(StableFlag_t *flag,
                             bool candidate,
                             uint32_t stableMs,
                             uint32_t nowMs)
{
    if (candidate) {
        if (!flag->active) {
            flag->active = true;
            flag->sinceMs = nowMs;
        }
        flag->stable = ((uint32_t)(nowMs - flag->sinceMs) >= stableMs);
    } else {
        flag->active = false;
        flag->stable = false;
        flag->sinceMs = nowMs;
    }
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

static bool IsBumperEvent(ES_EventTyp_t eventType)
{
    return ((eventType == EV_BUMPER_HIT) ||
            (eventType == EV_BUMPER_FRONT) ||
            (eventType == EV_BUMPER_LEFT) ||
            (eventType == EV_BUMPER_RIGHT) ||
            (eventType == EV_BUMPER_REAR));
}

static bool IsPatternCheckEvent(ES_Event event)
{
    return ((event.EventType == ES_TIMEOUT) &&
            (event.EventParam == NAV_PATTERN_CHECK_TIMER));
}

static void StartPatternCheck(void)
{
    ES_Timer_InitTimer(NAV_PATTERN_CHECK_TIMER, NAV_PATTERN_CHECK_MS);
}

static void StopPatternCheck(void)
{
    ES_Timer_StopTimer(NAV_PATTERN_CHECK_TIMER);
}

static uint8_t GetFrontBlackCount(void)
{
    return TapeFeature_CountBits(TapeFeature.frontMask);
}

static uint8_t GetRearBlackCount(void)
{
    return TapeFeature_CountBits(TapeFeature.rearMask);
}

static bool FrontHas(uint8_t bit)
{
    return ((TapeFeature.frontMask & bit) != 0u);
}

static bool RearHas(uint8_t bit)
{
    return ((TapeFeature.rearMask & bit) != 0u);
}

static bool IsLeftEdgeLockRaw(void)
{
    return (FrontHas(TAPE_F_LM_BIT) &&
            RearHas(TAPE_R_LM_BIT) &&
            !RearHas(TAPE_R_RM_BIT) &&
            !FrontHas(TAPE_F_M_BIT) &&
            (GetFrontBlackCount() <= 2u));
}

static bool IsRightEdgeLockRaw(void)
{
    return (FrontHas(TAPE_F_RM_BIT) &&
            RearHas(TAPE_R_RM_BIT) &&
            !RearHas(TAPE_R_LM_BIT) &&
            !FrontHas(TAPE_F_M_BIT) &&
            (GetFrontBlackCount() <= 2u));
}

static bool IsLeftFrontOffsetRaw(void)
{
    return (!FrontHas(TAPE_F_L_BIT) &&
            FrontHas(TAPE_F_LM_BIT) &&
            !FrontHas(TAPE_F_M_BIT));
}

static bool IsRightFrontOffsetRaw(void)
{
    return (!FrontHas(TAPE_F_R_BIT) &&
            FrontHas(TAPE_F_RM_BIT) &&
            !FrontHas(TAPE_F_M_BIT));
}

static void RefreshBeaconStability(void)
{
    uint32_t nowMs = ES_Timer_GetTime();
    bool locked = (Beacon_IsLocked() != FALSE);
    bool centered = (locked &&
                     (AbsInt(Beacon_GetBearingError()) <= BEACON_CENTER_TOL));

    if (locked) {
        LastBeaconSeenTimeMs = nowMs;
    }

    UpdateStableFlag(&BeaconLockedStableFlag, locked,
                     BEACON_LOCK_STABLE_MS, nowMs);
    UpdateStableFlag(&BeaconCenteredStableFlag, centered,
                     BEACON_CENTER_STABLE_MS, nowMs);
}

static void RefreshTapeFeature(void)
{
    uint8_t frontMask = Tape_GetFrontMask();
    uint8_t rearMask = Tape_GetRearMask();
    uint32_t nowMs = ES_Timer_GetTime();
    uint8_t frontCount;
    uint8_t rearCount;

    TapeFeature_Update(&TapeEstimator, frontMask, rearMask, nowMs, &TapeFeature);
    frontCount = GetFrontBlackCount();
    rearCount = GetRearBlackCount();

    UpdateStableFlag(&FrontWideBlackStableFlag, (frontCount >= 4u),
                     FRONT_WIDE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&FrontAnyBlackStableFlag, (frontCount > 0u),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&FrontIszCandidateStableFlag, (frontCount >= 2u),
                     FRONT_ISZ_CANDIDATE_STABLE_MS, nowMs);
    UpdateStableFlag(&RearWideBlackStableFlag, (rearCount >= 2u),
                     REAR_WIDE_CONFIRM_MS, nowMs);
    UpdateStableFlag(&RearAnyBlackStableFlag, (rearCount > 0u),
                     REAR_ANY_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&RearBoundaryStableFlag, (rearCount > 0u),
                     REAR_BOUNDARY_STABLE_MS, nowMs);
    UpdateStableFlag(&FrontMiddleStableFlag, FrontHas(TAPE_F_M_BIT),
                     FISHBONE_ACQUIRE_STABLE_MS, nowMs);
    UpdateStableFlag(&FrontLeftContactStableFlag,
                     ((frontMask & (TAPE_F_L_BIT | TAPE_F_LM_BIT)) != 0u),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&FrontRightContactStableFlag,
                     ((frontMask & (TAPE_F_R_BIT | TAPE_F_RM_BIT)) != 0u),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&LeftFrontOffsetStableFlag, IsLeftFrontOffsetRaw(),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&RightFrontOffsetStableFlag, IsRightFrontOffsetRaw(),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&LeftEdgeLockStableFlag, IsLeftEdgeLockRaw(),
                     EDGE_LOCK_DEBOUNCE_MS, nowMs);
    UpdateStableFlag(&RightEdgeLockStableFlag, IsRightEdgeLockRaw(),
                     EDGE_LOCK_DEBOUNCE_MS, nowMs);
    UpdateStableFlag(&FrontLeftStableFlag, FrontHas(TAPE_F_L_BIT),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&FrontLmStableFlag, FrontHas(TAPE_F_LM_BIT),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&FrontRmStableFlag, FrontHas(TAPE_F_RM_BIT),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);
    UpdateStableFlag(&FrontRightStableFlag, FrontHas(TAPE_F_R_BIT),
                     FRONT_SINGLE_BLACK_STABLE_MS, nowMs);

    if ((frontMask != LastPrintedFrontMask) || (rearMask != LastPrintedRearMask)) {
        LastPrintedFrontMask = frontMask;
        LastPrintedRearMask = rearMask;
        printf("\r\nTape masks: front=0x%02x rear=0x%02x "
               "frontCount=%u rearCount=%u Llock=%u Rlock=%u\r\n",
               frontMask, rearMask, (unsigned int)frontCount,
               (unsigned int)rearCount,
               (unsigned int)LeftEdgeLockStableFlag.stable,
               (unsigned int)RightEdgeLockStableFlag.stable);
    }
}

static bool IsRecoveryOrCrossingState(NavState_t state)
{
    return ((state == NavBackSearchRearBoundary) ||
            (state == NavRearBoundaryCaptureLeft) ||
            (state == NavLeftEdgeApproachSlow) ||
            (state == NavCornerExitForward) ||
            (state == NavLeftEdgeLock) ||
            (state == NavRecBackupAfterLeftBump) ||
            (state == NavStrafeRight11) ||
            (state == NavRecenterBeaconAfterRightShift) ||
            (state == NavFishboneAcquire) ||
            (state == NavRecBackupAfterFishboneBump) ||
            (state == NavReacquireLeftEdgeFromMiddle) ||
            (state == NavRecBackupBeforeRightFallback) ||
            (state == NavCrossToRightEdge) ||
            (state == NavRightEdgeApproachSlow) ||
            (state == NavRightEdgeLock) ||
            (state == NavRecBackupAfterRightFallbackBump) ||
            (state == NavStrafeLeftFromRightToFishbone) ||
            (state == NavFishboneAcquireFromRight) ||
            (state == NavRecBackupAfterCenterRecoveryBump) ||
            (state == NavReacquireLeftEdgeFinal) ||
            (state == NavCreepIntoISZ) ||
            (state == NavISZCenterSettle));
}

static void UpdateIszArmed(void)
{
    uint32_t elapsedMs = (uint32_t)(ES_Timer_GetTime() - RouteArmStartTimeMs);

    IszArmed = false;
    if ((TapeGuardMode == TAPE_GUARD_FRONT_WIDE_SUPPRESSED) ||
        IsRecoveryOrCrossingState(CurrentState)) {
        return;
    }

    if (((RouteMode == ROUTE_LEFT_PRIMARY) ||
         (RouteMode == ROUTE_LEFT_RETRY) ||
         (RouteMode == ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY)) &&
        HasCompletedInitialLeftEdgeAcquisition &&
        (elapsedMs >= NAV_MIN_LEFT_EDGE_TO_ISZ_MS)) {
        IszArmed = true;
    } else if ((RouteMode == ROUTE_RIGHT_FALLBACK) &&
               (elapsedMs >= NAV_MIN_RIGHT_EDGE_TO_ISZ_MS)) {
        IszArmed = true;
    }
}

static int ClampYaw(int yaw, int maxMagnitude)
{
    if (yaw > maxMagnitude) {
        return maxMagnitude;
    }
    if (yaw < -maxMagnitude) {
        return -maxMagnitude;
    }
    return yaw;
}

static int Navigate_GetBeaconYawCorrection(void)
{
    int error;
    int correction;

    if (Beacon_IsLocked() == FALSE) {
        if ((uint32_t)(ES_Timer_GetTime() - LastBeaconSeenTimeMs) <=
            BEACON_LOST_MS) {
            return 0;
        }
        return 0;
    }

    LastBeaconSeenTimeMs = ES_Timer_GetTime();
    error = Beacon_GetBearingError();
    correction = (error * BEACON_YAW_GAIN_NUM) / BEACON_YAW_GAIN_DEN;
    return ClampYaw(correction, NAV_YAW_CORRECT_MAX);
}

static int BeaconYawCorrectionSmall(void)
{
    return ClampYaw(Navigate_GetBeaconYawCorrection(), NAV_YAW_CORRECT_SMALL_MAX);
}

static bool BeaconIsCenteredEnough(void)
{
    return BeaconCenteredStableFlag.stable;
}

static void DriveSpinScan(void)
{
    Drive_SetBodyVelocity(0, 0, BeaconScanSign * NAV_YAW_SCAN_SPEED);
}

static void DriveCenterBeacon(void)
{
    Drive_SetBodyVelocity(0, 0, Navigate_GetBeaconYawCorrection());
}

static void DriveBackwardSearchRearBoundary(void)
{
    Drive_SetBodyVelocity(NAV_BACK_SEARCH_SPEED, 0, BeaconYawCorrectionSmall());
}

static void DriveRearBoundaryCaptureLeft(void)
{
    bool rLm = RearHas(TAPE_R_LM_BIT);
    bool rRm = RearHas(TAPE_R_RM_BIT);

    if (rLm && rRm) {
        Drive_SetBodyVelocity(0, NAV_REAR_CAPTURE_STRAFE_SPEED,
                              BeaconYawCorrectionSmall());
    } else if (rLm && !rRm) {
        Drive_SetBodyVelocity(0, NAV_REAR_CAPTURE_STRAFE_SLOW,
                              NAV_YAW_NUDGE_RIGHT);
    } else if (!rLm && rRm) {
        Drive_SetBodyVelocity(0, NAV_REAR_CAPTURE_STRAFE_SLOW,
                              NAV_YAW_NUDGE_LEFT);
    } else {
        Drive_SetBodyVelocity(NAV_REAR_CAPTURE_BACKUP_SPEED, 0,
                              BeaconYawCorrectionSmall());
    }
}

static void DriveSeekLeftEdge(void)
{
    Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                          NAV_STRAFE_LEFT_SEARCH,
                          BeaconYawCorrectionSmall());
}

static void DriveReacquireLeftEdge(void)
{
    if (FrontHas(TAPE_F_L_BIT) || FrontHas(TAPE_F_LM_BIT)) {
        if (FrontHas(TAPE_F_M_BIT)) {
            Drive_SetBodyVelocity(0, STRAFE_INWARD_FROM_LEFT,
                                  BeaconYawCorrectionSmall());
        } else {
            Drive_SetBodyVelocity(0, NAV_STRAFE_LEFT_SEARCH / 3,
                                  BeaconYawCorrectionSmall());
        }
    } else {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_VERY_SLOW,
                              NAV_STRAFE_LEFT_SEARCH,
                              BeaconYawCorrectionSmall());
    }
}

static void DriveLeftEdgeApproachSlow(void)
{
    if (FrontHas(TAPE_F_M_BIT)) {
        Drive_SetBodyVelocity(0, STRAFE_INWARD_FROM_LEFT,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_L_BIT) && FrontHas(TAPE_F_LM_BIT)) {
        Drive_SetBodyVelocity(0, NAV_STRAFE_LEFT_SEARCH / 3,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_L_BIT)) {
        Drive_SetBodyVelocity(0, NAV_STRAFE_LEFT_SEARCH / 2,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_LM_BIT)) {
        Drive_Stop();
    } else {
        DriveSeekLeftEdge();
    }
}

static void DriveCornerExitForward(void)
{
    if (FrontHas(TAPE_F_M_BIT)) {
        Drive_SetBodyVelocity(0, STRAFE_INWARD_FROM_LEFT,
                              BeaconYawCorrectionSmall());
    } else if (!FrontHas(TAPE_F_LM_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_VERY_SLOW,
                              NAV_STRAFE_LEFT_SEARCH / 2,
                              BeaconYawCorrectionSmall());
    } else {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW, 0,
                              BeaconYawCorrectionSmall());
    }
}

static void DriveLeftEdgeControl(void)
{
    bool ideal = (FrontHas(TAPE_F_LM_BIT) &&
                  RearHas(TAPE_R_LM_BIT) &&
                  !FrontHas(TAPE_F_M_BIT) &&
                  !RearHas(TAPE_R_RM_BIT) &&
                  (GetFrontBlackCount() <= 2u));

    if (ideal) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD, 0,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_M_BIT)) {
        Drive_SetBodyVelocity(0, STRAFE_INWARD_FROM_LEFT,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_L_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                              NAV_STRAFE_LEFT_SEARCH / 3,
                              BeaconYawCorrectionSmall());
    } else if (!FrontHas(TAPE_F_LM_BIT) && RearHas(TAPE_R_LM_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                              NAV_STRAFE_LEFT_SEARCH / 2,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_LM_BIT) && !RearHas(TAPE_R_LM_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW, 0,
                              BeaconYawCorrectionSmall());
    } else {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_VERY_SLOW,
                              NAV_STRAFE_LEFT_SEARCH,
                              BeaconYawCorrectionSmall());
    }
}

static void DriveCrossToRight(void)
{
    Drive_SetBodyVelocity(CROSS_TO_RIGHT_FORWARD_BIAS,
                          CROSS_TO_RIGHT_STRAFE_SPEED,
                          BeaconYawCorrectionSmall());
}

static void DriveRightEdgeApproachSlow(void)
{
    if (FrontHas(TAPE_F_M_BIT)) {
        Drive_SetBodyVelocity(0, STRAFE_INWARD_FROM_RIGHT,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_R_BIT) && FrontHas(TAPE_F_RM_BIT)) {
        Drive_SetBodyVelocity(0, NAV_STRAFE_RIGHT_SEARCH / 3,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_R_BIT)) {
        Drive_SetBodyVelocity(0, NAV_STRAFE_RIGHT_SEARCH / 2,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_RM_BIT)) {
        Drive_Stop();
    } else {
        DriveCrossToRight();
    }
}

static void DriveRightEdgeControl(void)
{
    bool ideal = (FrontHas(TAPE_F_RM_BIT) &&
                  RearHas(TAPE_R_RM_BIT) &&
                  !FrontHas(TAPE_F_M_BIT) &&
                  !RearHas(TAPE_R_LM_BIT) &&
                  (GetFrontBlackCount() <= 2u));

    if (ideal) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD, 0,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_M_BIT)) {
        Drive_SetBodyVelocity(0, STRAFE_INWARD_FROM_RIGHT,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_R_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                              NAV_STRAFE_RIGHT_SEARCH / 3,
                              BeaconYawCorrectionSmall());
    } else if (!FrontHas(TAPE_F_RM_BIT) && RearHas(TAPE_R_RM_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW,
                              NAV_STRAFE_RIGHT_SEARCH / 2,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_RM_BIT) && !RearHas(TAPE_R_RM_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_SLOW, 0,
                              BeaconYawCorrectionSmall());
    } else {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_VERY_SLOW,
                              NAV_STRAFE_RIGHT_SEARCH,
                              BeaconYawCorrectionSmall());
    }
}

static void DriveFishboneAcquire(void)
{
    if (FrontHas(TAPE_F_LM_BIT) || FrontHas(TAPE_F_L_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_VERY_SLOW,
                              -NAV_FISHBONE_STRAFE_SMALL,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_RM_BIT) || FrontHas(TAPE_F_R_BIT)) {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_VERY_SLOW,
                              NAV_FISHBONE_STRAFE_SMALL,
                              BeaconYawCorrectionSmall());
    } else {
        Drive_SetBodyVelocity(NAV_EDGE_FORWARD_VERY_SLOW, 0,
                              BeaconYawCorrectionSmall());
    }
}

static void DriveFishboneFollow(void)
{
    if (FrontHas(TAPE_F_M_BIT)) {
        if (FrontLmStableFlag.stable && !FrontRmStableFlag.stable) {
            Drive_SetBodyVelocity(NAV_FISHBONE_FORWARD,
                                  -NAV_FISHBONE_STRAFE_SMALL / 2,
                                  BeaconYawCorrectionSmall());
        } else if (FrontRmStableFlag.stable && !FrontLmStableFlag.stable) {
            Drive_SetBodyVelocity(NAV_FISHBONE_FORWARD,
                                  NAV_FISHBONE_STRAFE_SMALL / 2,
                                  BeaconYawCorrectionSmall());
        } else {
            Drive_SetBodyVelocity(NAV_FISHBONE_FORWARD, 0,
                                  BeaconYawCorrectionSmall());
        }
    } else if (FrontHas(TAPE_F_LM_BIT)) {
        Drive_SetBodyVelocity(NAV_FISHBONE_FORWARD_SLOW,
                              -NAV_FISHBONE_STRAFE_SMALL,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_RM_BIT)) {
        Drive_SetBodyVelocity(NAV_FISHBONE_FORWARD_SLOW,
                              NAV_FISHBONE_STRAFE_SMALL,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_L_BIT)) {
        Drive_SetBodyVelocity(0, -NAV_FISHBONE_STRAFE_STRONG,
                              BeaconYawCorrectionSmall());
    } else if (FrontHas(TAPE_F_R_BIT)) {
        Drive_SetBodyVelocity(0, NAV_FISHBONE_STRAFE_STRONG,
                              BeaconYawCorrectionSmall());
    } else {
        Drive_SetBodyVelocity(NAV_FISHBONE_FORWARD_SLOW, 0,
                              BeaconYawCorrectionSmall());
    }
}

static void DrivePostFishboneISZSeek(void)
{
    Drive_SetBodyVelocity(NAV_POST_FISHBONE_FORWARD, 0,
                          BeaconYawCorrectionSmall());
}

static void DriveStrafeLeftFromRightToFishbone(void)
{
    Drive_SetBodyVelocity(CROSS_TO_RIGHT_FORWARD_BIAS,
                          STRAFE_FROM_RIGHT_TO_FISHBONE_SPEED,
                          BeaconYawCorrectionSmall());
}

static void SetRouteMode(RouteMode_t mode)
{
    if (RouteMode != mode) {
        RouteMode = mode;
        printf("\r\nNAV route: %s\r\n", RouteNames[RouteMode]);
    } else {
        RouteMode = mode;
    }
}

static void SetTapeGuardMode(TapeGuardMode_t mode)
{
    TapeGuardMode = mode;
}

static void ResetRouteArmTimers(void)
{
    RouteArmStartTimeMs = ES_Timer_GetTime();
    IszArmed = false;
}

static void ResetFishboneTracking(void)
{
    FishboneSeenEver = false;
    FishboneClearSeen = false;
    FishboneFollowStartTimeMs = ES_Timer_GetTime();
    ResetStableFlag(&FishboneClearStableFlag);
}

static void UpdateFishboneTracking(void)
{
    uint32_t nowMs = ES_Timer_GetTime();
    bool clearCandidate;

    if ((GetFrontBlackCount() > 0u) || (GetRearBlackCount() > 0u)) {
        FishboneSeenEver = true;
    }

    clearCandidate = (FishboneSeenEver &&
                      ((uint32_t)(nowMs - FishboneFollowStartTimeMs) >=
                       FISHBONE_MIN_FOLLOW_MS) &&
                      (GetFrontBlackCount() == 0u) &&
                      (GetRearBlackCount() == 0u));

    UpdateStableFlag(&FishboneClearStableFlag, clearCandidate,
                     FISHBONE_CLEAR_STABLE_MS, nowMs);
    if (FishboneClearStableFlag.stable) {
        FishboneClearSeen = true;
    }
}

static bool BumperHitStable(ES_Event event)
{
    return (event.EventType == EV_BUMPER_HIT);
}

static bool FrontWideBlackStable(void)
{
    return FrontWideBlackStableFlag.stable;
}

static bool FrontAnyBlackStable(void)
{
    return FrontAnyBlackStableFlag.stable;
}

static bool RearWideBlackStable(void)
{
    return RearWideBlackStableFlag.stable;
}

static bool RearAnyBlackStable(void)
{
    return RearAnyBlackStableFlag.stable;
}

static bool IsLeftEdgeLocked(void)
{
    return LeftEdgeLockStableFlag.stable;
}

static bool IsRightEdgeLocked(void)
{
    return RightEdgeLockStableFlag.stable;
}

static bool IsFishboneAcquired(void)
{
    return FrontMiddleStableFlag.stable;
}

static bool IsFishboneClearStable(void)
{
    return FishboneClearStableFlag.stable;
}

static bool IsLeftEdgeISZCandidate(void)
{
    return (((RouteMode == ROUTE_LEFT_PRIMARY) ||
             (RouteMode == ROUTE_LEFT_RETRY) ||
             (RouteMode == ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY)) &&
            IszArmed &&
            (TapeGuardMode == TAPE_GUARD_NORMAL) &&
            FrontWideBlackStable());
}

static bool IsRightEdgeISZCandidate(void)
{
    return ((RouteMode == ROUTE_RIGHT_FALLBACK) &&
            IszArmed &&
            (TapeGuardMode == TAPE_GUARD_NORMAL) &&
            FrontWideBlackStable());
}

static bool IsFishboneISZCandidate(void)
{
    return (((RouteMode == ROUTE_CENTER_FISHBONE) ||
             (RouteMode == ROUTE_CENTER_FROM_RIGHT_RECOVERY)) &&
            FishboneClearSeen &&
            (TapeGuardMode == TAPE_GUARD_NORMAL) &&
            (FrontIszCandidateStableFlag.stable || FrontAnyBlackStable()));
}

static void EnterISZCandidate(IszApproachLane_t lane)
{
    IszApproachLane = lane;
    printf("\r\nNAV ISZ candidate: %s\r\n", LaneNames[IszApproachLane]);
    PostTopEvent(EV_ISZ_ENTRY_CANDIDATE);
}

static NavState_t BeginFinalLeftAttempt(void)
{
    SetRouteMode(ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY);
    IszApproachLane = ISZ_FROM_LEFT_EDGE;
    SetTapeGuardMode(TAPE_GUARD_FRONT_WIDE_SUPPRESSED);
    return NavReacquireLeftEdgeFinal;
}

static NavState_t HandleLeftEdgeBumper(void)
{
    switch (RouteMode) {
    case ROUTE_LEFT_PRIMARY:
        SetRouteMode(ROUTE_CENTER_FISHBONE);
        IszApproachLane = ISZ_FROM_CENTER_FISHBONE;
        return NavRecBackupAfterLeftBump;

    case ROUTE_LEFT_RETRY:
        SetRouteMode(ROUTE_RIGHT_FALLBACK);
        IszApproachLane = ISZ_FROM_RIGHT_EDGE;
        SetTapeGuardMode(TAPE_GUARD_FRONT_WIDE_SUPPRESSED);
        return NavRecBackupBeforeRightFallback;

    case ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY:
        printf("\r\nNAV failed: final-left bumper\r\n");
        return NavFailed;

    default:
        return NavFailed;
    }
}

static NavState_t HandleFishboneBumper(void)
{
    switch (RouteMode) {
    case ROUTE_CENTER_FISHBONE:
        SetRouteMode(ROUTE_LEFT_RETRY);
        IszApproachLane = ISZ_FROM_LEFT_EDGE;
        return NavRecBackupAfterFishboneBump;

    case ROUTE_CENTER_FROM_RIGHT_RECOVERY:
        SetRouteMode(ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY);
        IszApproachLane = ISZ_FROM_LEFT_EDGE;
        SetTapeGuardMode(TAPE_GUARD_FRONT_WIDE_SUPPRESSED);
        return NavRecBackupAfterCenterRecoveryBump;

    default:
        return NavFailed;
    }
}

static NavState_t HandleRightEdgeBumper(void)
{
    if (RouteMode == ROUTE_RIGHT_FALLBACK) {
        SetRouteMode(ROUTE_CENTER_FROM_RIGHT_RECOVERY);
        IszApproachLane = ISZ_FROM_CENTER_FISHBONE;
        SetTapeGuardMode(TAPE_GUARD_FRONT_WIDE_SUPPRESSED);
        return NavRecBackupAfterRightFallbackBump;
    }
    return NavFailed;
}

static void EnterNavState(const char *name)
{
    StateEntryTimeMs = ES_Timer_GetTime();
    printf("\r\nNav state: %s route=%s guard=%u armed=%u\r\n",
           name, RouteNames[RouteMode], (unsigned int)TapeGuardMode,
           (unsigned int)IszArmed);
}

static void UpdateCreepRearConfirmation(void)
{
    if (RearWideBlackStable()) {
        RearWideStableDuringCreep = true;
    }

    if (RearAnyBlackStable() && !RearAnySeenDuringCreep) {
        RearAnySeenDuringCreep = true;
        ES_Timer_InitTimer(ISZ_EXTRA_CREEP_TIMER, ISZ_EXTRA_CREEP_MS);
    }
}

static void EnterLeftEdgeCruise(NavState_t state)
{
    if (state == NavLeftEdgeCruise) {
        SetRouteMode(ROUTE_LEFT_PRIMARY);
    } else if (state == NavLeftEdgeCruiseRetry) {
        SetRouteMode(ROUTE_LEFT_RETRY);
    } else {
        SetRouteMode(ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY);
    }

    IszApproachLane = ISZ_FROM_LEFT_EDGE;
    SetTapeGuardMode(TAPE_GUARD_NORMAL);
    DriveLeftEdgeControl();
    StartPatternCheck();
}

static void EnterFishboneFollow(NavState_t state)
{
    (void)state;
    ResetFishboneTracking();
    DriveFishboneFollow();
    StartPatternCheck();
}

static NavState_t NextFishboneFollowState(void)
{
    if (RouteMode == ROUTE_CENTER_FROM_RIGHT_RECOVERY) {
        return NavFishboneFollowFromRightRecovery;
    }
    return NavFishboneFollow;
}

static NavState_t NextPostFishboneState(void)
{
    if (RouteMode == ROUTE_CENTER_FROM_RIGHT_RECOVERY) {
        return NavPostFishboneISZSeekFromRightRecovery;
    }
    return NavPostFishboneISZSeek;
}

static bool HandleLeftEdgeCruiseEvent(ES_Event event,
                                      NavState_t *nextState,
                                      uint8_t *makeTransition)
{
    if (event.EventType == ES_ENTRY) {
        EnterNavState(StateNames[CurrentState]);
        EnterLeftEdgeCruise(CurrentState);
        return true;
    }

    if (BumperHitStable(event)) {
        *nextState = HandleLeftEdgeBumper();
        *makeTransition = TRUE;
        return true;
    }

    if (IsLeftEdgeISZCandidate()) {
        EnterISZCandidate(ISZ_FROM_LEFT_EDGE);
        *nextState = NavCreepIntoISZ;
        *makeTransition = TRUE;
        return true;
    }

    if (IsTapeEvent(event.EventType) ||
        IsBeaconSteeringEvent(event.EventType) ||
        IsPatternCheckEvent(event)) {
        DriveLeftEdgeControl();
        if (IsPatternCheckEvent(event)) {
            StartPatternCheck();
        }
        return true;
    }

    return false;
}

static bool HandleFishboneAcquireEvent(ES_Event event,
                                       NavState_t *nextState,
                                       uint8_t *makeTransition)
{
    if (event.EventType == ES_ENTRY) {
        EnterNavState(StateNames[CurrentState]);
        DriveFishboneAcquire();
        if (RouteMode == ROUTE_CENTER_FROM_RIGHT_RECOVERY) {
            ES_Timer_InitTimer(FISHBONE_ACQUIRE_TIMER,
                               FISHBONE_FROM_RIGHT_ACQUIRE_TIMEOUT_MS);
        } else {
            ES_Timer_InitTimer(FISHBONE_ACQUIRE_TIMER,
                               FISHBONE_ACQUIRE_TIMEOUT_MS);
        }
        StartPatternCheck();
        return true;
    }

    if (BumperHitStable(event)) {
        *nextState = HandleFishboneBumper();
        *makeTransition = TRUE;
        return true;
    }

    if (IsFishboneAcquired()) {
        SetTapeGuardMode(TAPE_GUARD_NORMAL);
        ResetFishboneTracking();
        *nextState = NextFishboneFollowState();
        *makeTransition = TRUE;
        return true;
    }

    if ((event.EventType == ES_TIMEOUT) &&
        (event.EventParam == FISHBONE_ACQUIRE_TIMER)) {
        if (RouteMode == ROUTE_CENTER_FROM_RIGHT_RECOVERY) {
            *nextState = BeginFinalLeftAttempt();
        } else if (RouteMode == ROUTE_CENTER_FISHBONE) {
            SetRouteMode(ROUTE_LEFT_RETRY);
            IszApproachLane = ISZ_FROM_LEFT_EDGE;
            *nextState = NavReacquireLeftEdgeFromMiddle;
        } else {
            *nextState = NavFailed;
        }
        *makeTransition = TRUE;
        return true;
    }

    if (IsTapeEvent(event.EventType) ||
        IsBeaconSteeringEvent(event.EventType) ||
        IsPatternCheckEvent(event)) {
        DriveFishboneAcquire();
        if (IsPatternCheckEvent(event)) {
            StartPatternCheck();
        }
        return true;
    }

    return false;
}

static bool HandleFishboneFollowEvent(ES_Event event,
                                      NavState_t *nextState,
                                      uint8_t *makeTransition)
{
    if (event.EventType == ES_ENTRY) {
        EnterNavState(StateNames[CurrentState]);
        EnterFishboneFollow(CurrentState);
        return true;
    }

    if (BumperHitStable(event)) {
        *nextState = HandleFishboneBumper();
        *makeTransition = TRUE;
        return true;
    }

    UpdateFishboneTracking();
    if (IsFishboneClearStable()) {
        FishboneClearSeen = true;
        *nextState = NextPostFishboneState();
        *makeTransition = TRUE;
        return true;
    }

    if (IsTapeEvent(event.EventType) ||
        IsBeaconSteeringEvent(event.EventType) ||
        IsPatternCheckEvent(event)) {
        DriveFishboneFollow();
        if (IsPatternCheckEvent(event)) {
            StartPatternCheck();
        }
        return true;
    }

    return false;
}

static bool HandlePostFishboneISZEvent(ES_Event event,
                                       NavState_t *nextState,
                                       uint8_t *makeTransition)
{
    if (event.EventType == ES_ENTRY) {
        EnterNavState(StateNames[CurrentState]);
        DrivePostFishboneISZSeek();
        ES_Timer_InitTimer(POST_FISHBONE_ISZ_TIMER, NAV_ISZ_SEARCH_MS);
        StartPatternCheck();
        return true;
    }

    if (BumperHitStable(event)) {
        *nextState = HandleFishboneBumper();
        *makeTransition = TRUE;
        return true;
    }

    if (IsFishboneISZCandidate()) {
        EnterISZCandidate(ISZ_FROM_CENTER_FISHBONE);
        *nextState = NavCreepIntoISZ;
        *makeTransition = TRUE;
        return true;
    }

    if ((event.EventType == ES_TIMEOUT) &&
        (event.EventParam == POST_FISHBONE_ISZ_TIMER)) {
        if (RouteMode == ROUTE_CENTER_FROM_RIGHT_RECOVERY) {
            *nextState = BeginFinalLeftAttempt();
        } else {
            SetRouteMode(ROUTE_LEFT_RETRY);
            IszApproachLane = ISZ_FROM_LEFT_EDGE;
            *nextState = NavReacquireLeftEdgeFromMiddle;
        }
        *makeTransition = TRUE;
        return true;
    }

    if (IsTapeEvent(event.EventType) ||
        IsBeaconSteeringEvent(event.EventType) ||
        IsPatternCheckEvent(event)) {
        DrivePostFishboneISZSeek();
        if (IsPatternCheckEvent(event)) {
            StartPatternCheck();
        }
        return true;
    }

    return false;
}

uint8_t InitNavigateHSM(void)
{
    CurrentState = NavInitPState;
    RouteMode = ROUTE_LEFT_PRIMARY;
    TapeGuardMode = TAPE_GUARD_NORMAL;
    IszApproachLane = ISZ_FROM_LEFT_EDGE;
    TapeFeature_Reset(&TapeEstimator);
    HasCompletedInitialLeftEdgeAcquisition = false;
    IszArmed = false;
    FishboneSeenEver = false;
    FishboneClearSeen = false;
    RearAnySeenDuringCreep = false;
    RearWideStableDuringCreep = false;
    BeaconScanRetries = 0u;
    BeaconScanSign = 1;
    LastPrintedFrontMask = 0xffu;
    LastPrintedRearMask = 0xffu;
    LastBeaconSeenTimeMs = ES_Timer_GetTime();
    RouteArmStartTimeMs = ES_Timer_GetTime();
    FishboneFollowStartTimeMs = ES_Timer_GetTime();
    StateEntryTimeMs = ES_Timer_GetTime();
    ResetAllStableFlags();
    RefreshTapeFeature();
    RefreshBeaconStability();
    printf("\r\nNAV route: %s\r\n", RouteNames[RouteMode]);
    return (RunNavigateHSM(INIT_EVENT).EventType == ES_NO_EVENT);
}

ES_Event RunNavigateHSM(ES_Event event)
{
    ES_Event ThisEvent = event;
    uint8_t makeTransition = FALSE;
    NavState_t nextState = CurrentState;

    ES_Tattle();

    RefreshTapeFeature();
    RefreshBeaconStability();
    UpdateIszArmed();

    if ((ThisEvent.EventType == ES_TIMEOUT) &&
        (ThisEvent.EventParam == NAV_TOTAL_GUARD_TIMER) &&
        (CurrentState != NavFailed) &&
        (CurrentState != NavISZConfirmed)) {
        nextState = NavFailed;
        makeTransition = TRUE;
        event.EventType = ES_NO_EVENT;
    }

    if (makeTransition == FALSE) {
        switch (CurrentState) {
        case NavInitPState:
            if (ThisEvent.EventType == ES_INIT) {
                ES_Timer_InitTimer(NAV_TOTAL_GUARD_TIMER,
                                   NAV_MAX_TOTAL_TIME_MS);
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
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (BeaconLockedStableFlag.stable) {
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
                    ES_Timer_InitTimer(NAV_BEACON_SCAN_TIMER,
                                       NAV_BEACON_SCAN_MS);
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsPatternCheckEvent(ThisEvent)) {
                DriveSpinScan();
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavCenterBeacon:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveCenterBeacon();
                ES_Timer_InitTimer(REC_RECENTER_TIMER,
                                   REC_RECENTER_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (ThisEvent.EventType == EV_BEACON_LOST) {
                nextState = NavSpinFindBeacon;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BeaconIsCenteredEnough()) {
                printf("\r\nNav event: beacon centered; heading canonicalized\r\n");
                nextState = NavBackSearchRearBoundary;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_RECENTER_TIMER)) {
                DriveCenterBeacon();
                ES_Timer_InitTimer(REC_RECENTER_TIMER,
                                   REC_RECENTER_TIMEOUT_MS);
                event.EventType = ES_NO_EVENT;
            } else if (IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveCenterBeacon();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavBackSearchRearBoundary:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveBackwardSearchRearBoundary();
                ES_Timer_InitTimer(NAV_BACK_SEARCH_TIMER,
                                   NAV_EDGE_SEEK_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (RearBoundaryStableFlag.stable) {
                nextState = NavRearBoundaryCaptureLeft;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_BACK_SEARCH_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveBackwardSearchRearBoundary();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsBumperEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRearBoundaryCaptureLeft:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveRearBoundaryCaptureLeft();
                ES_Timer_InitTimer(NAV_REAR_CAPTURE_TIMER,
                                   NAV_EDGE_SEEK_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (FrontLeftContactStableFlag.stable) {
                nextState = NavLeftEdgeApproachSlow;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_REAR_CAPTURE_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveRearBoundaryCaptureLeft();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsBumperEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavLeftEdgeApproachSlow:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveLeftEdgeApproachSlow();
                ES_Timer_InitTimer(NAV_EDGE_APPROACH_TIMER,
                                   NAV_EDGE_APPROACH_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (LeftFrontOffsetStableFlag.stable || IsLeftEdgeLocked()) {
                nextState = NavCornerExitForward;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_EDGE_APPROACH_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveLeftEdgeApproachSlow();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsBumperEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavCornerExitForward:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveCornerExitForward();
                ES_Timer_InitTimer(NAV_CORNER_EXIT_TIMER,
                                   NAV_CORNER_EXIT_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsLeftEdgeLocked()) {
                nextState = NavLeftEdgeLock;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_CORNER_EXIT_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveCornerExitForward();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsBumperEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavLeftEdgeLock:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_Stop();
                ES_Timer_InitTimer(NAV_EDGE_APPROACH_TIMER,
                                   EDGE_LOCK_DEBOUNCE_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsLeftEdgeLocked()) {
                SetRouteMode(ROUTE_LEFT_PRIMARY);
                IszApproachLane = ISZ_FROM_LEFT_EDGE;
                SetTapeGuardMode(TAPE_GUARD_NORMAL);
                HasCompletedInitialLeftEdgeAcquisition = true;
                ResetRouteArmTimers();
                nextState = NavLeftEdgeCruise;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_EDGE_APPROACH_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavLeftEdgeCruise:
        case NavLeftEdgeCruiseRetry:
        case NavLeftEdgeCruiseFinal:
            if (HandleLeftEdgeCruiseEvent(ThisEvent,
                                          &nextState,
                                          &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecBackupAfterLeftBump:
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
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBumperEvent(ThisEvent.EventType)) {
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
            } else if (BumperHitStable(ThisEvent)) {
                nextState = NavFailed;
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
                ES_Timer_InitTimer(REC_RECENTER_TIMER,
                                   REC_RECENTER_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (BeaconIsCenteredEnough() ||
                       ((ThisEvent.EventType == ES_TIMEOUT) &&
                        (ThisEvent.EventParam == REC_RECENTER_TIMER))) {
                nextState = NavFishboneAcquire;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveCenterBeacon();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavFishboneAcquire:
        case NavFishboneAcquireFromRight:
            if (HandleFishboneAcquireEvent(ThisEvent,
                                           &nextState,
                                           &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavFishboneFollow:
        case NavFishboneFollowFromRightRecovery:
            if (HandleFishboneFollowEvent(ThisEvent,
                                          &nextState,
                                          &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavPostFishboneISZSeek:
        case NavPostFishboneISZSeekFromRightRecovery:
            if (HandlePostFishboneISZEvent(ThisEvent,
                                           &nextState,
                                           &makeTransition)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecBackupAfterFishboneBump:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_SetBodyVelocity(REC_BACKUP_SPEED, 0, 0);
                ES_Timer_InitTimer(REC_BACKUP_TIMER, REC_BACKUP_MS);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_BACKUP_TIMER)) {
                nextState = NavReacquireLeftEdgeFromMiddle;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBumperEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavReacquireLeftEdgeFromMiddle:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                SetRouteMode(ROUTE_LEFT_RETRY);
                IszApproachLane = ISZ_FROM_LEFT_EDGE;
                SetTapeGuardMode(TAPE_GUARD_NORMAL);
                DriveReacquireLeftEdge();
                ES_Timer_InitTimer(LEFT_REACQUIRE_TIMER,
                                   LEFT_EDGE_REACQUIRE_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsLeftEdgeLocked()) {
                SetRouteMode(ROUTE_LEFT_RETRY);
                IszApproachLane = ISZ_FROM_LEFT_EDGE;
                SetTapeGuardMode(TAPE_GUARD_NORMAL);
                ResetRouteArmTimers();
                nextState = NavLeftEdgeCruiseRetry;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                nextState = HandleLeftEdgeBumper();
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == LEFT_REACQUIRE_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveReacquireLeftEdge();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecBackupBeforeRightFallback:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_SetBodyVelocity(REC_BACKUP_SPEED, 0, 0);
                ES_Timer_InitTimer(REC_BACKUP_TIMER, REC_BACKUP_MS);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_BACKUP_TIMER)) {
                nextState = NavCrossToRightEdge;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBumperEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavCrossToRightEdge:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                SetTapeGuardMode(TAPE_GUARD_FRONT_WIDE_SUPPRESSED);
                DriveCrossToRight();
                ES_Timer_InitTimer(RIGHT_EDGE_ACQUIRE_TIMER,
                                   RIGHT_EDGE_ACQUIRE_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsRightEdgeLocked()) {
                nextState = NavRightEdgeLock;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (FrontRightContactStableFlag.stable ||
                       RightFrontOffsetStableFlag.stable) {
                nextState = NavRightEdgeApproachSlow;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == RIGHT_EDGE_ACQUIRE_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveCrossToRight();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRightEdgeApproachSlow:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                DriveRightEdgeApproachSlow();
                ES_Timer_InitTimer(RIGHT_EDGE_ACQUIRE_TIMER,
                                   RIGHT_EDGE_ACQUIRE_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (RightFrontOffsetStableFlag.stable || IsRightEdgeLocked()) {
                nextState = NavRightEdgeLock;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == RIGHT_EDGE_ACQUIRE_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveRightEdgeApproachSlow();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRightEdgeLock:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_Stop();
                ES_Timer_InitTimer(RIGHT_EDGE_ACQUIRE_TIMER,
                                   RIGHT_EDGE_ACQUIRE_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsRightEdgeLocked()) {
                SetRouteMode(ROUTE_RIGHT_FALLBACK);
                IszApproachLane = ISZ_FROM_RIGHT_EDGE;
                SetTapeGuardMode(TAPE_GUARD_NORMAL);
                ResetRouteArmTimers();
                nextState = NavRightEdgeCruise;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == RIGHT_EDGE_ACQUIRE_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRightEdgeCruise:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                SetRouteMode(ROUTE_RIGHT_FALLBACK);
                IszApproachLane = ISZ_FROM_RIGHT_EDGE;
                SetTapeGuardMode(TAPE_GUARD_NORMAL);
                DriveRightEdgeControl();
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                nextState = HandleRightEdgeBumper();
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsRightEdgeISZCandidate()) {
                EnterISZCandidate(ISZ_FROM_RIGHT_EDGE);
                nextState = NavCreepIntoISZ;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveRightEdgeControl();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecBackupAfterRightFallbackBump:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_SetBodyVelocity(REC_BACKUP_SPEED, 0, 0);
                ES_Timer_InitTimer(REC_BACKUP_TIMER, REC_BACKUP_MS);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_BACKUP_TIMER)) {
                nextState = NavStrafeLeftFromRightToFishbone;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBumperEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavStrafeLeftFromRightToFishbone:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                SetTapeGuardMode(TAPE_GUARD_FRONT_WIDE_SUPPRESSED);
                DriveStrafeLeftFromRightToFishbone();
                ES_Timer_InitTimer(FISHBONE_ACQUIRE_TIMER,
                                   FISHBONE_FROM_RIGHT_ACQUIRE_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsFishboneAcquired()) {
                nextState = NavFishboneAcquireFromRight;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                nextState = HandleFishboneBumper();
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == FISHBONE_ACQUIRE_TIMER)) {
                nextState = BeginFinalLeftAttempt();
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveStrafeLeftFromRightToFishbone();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavRecBackupAfterCenterRecoveryBump:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_SetBodyVelocity(REC_BACKUP_SPEED, 0, 0);
                ES_Timer_InitTimer(REC_BACKUP_TIMER, REC_BACKUP_MS);
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == REC_BACKUP_TIMER)) {
                nextState = NavReacquireLeftEdgeFinal;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBumperEvent(ThisEvent.EventType)) {
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavReacquireLeftEdgeFinal:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                SetRouteMode(ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY);
                IszApproachLane = ISZ_FROM_LEFT_EDGE;
                SetTapeGuardMode(TAPE_GUARD_FRONT_WIDE_SUPPRESSED);
                DriveReacquireLeftEdge();
                ES_Timer_InitTimer(FINAL_LEFT_EDGE_ACQUIRE_TIMER,
                                   FINAL_LEFT_EDGE_ACQUIRE_TIMEOUT_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsLeftEdgeLocked()) {
                SetRouteMode(ROUTE_LEFT_FINAL_AFTER_RIGHT_RECOVERY);
                IszApproachLane = ISZ_FROM_LEFT_EDGE;
                SetTapeGuardMode(TAPE_GUARD_NORMAL);
                ResetRouteArmTimers();
                nextState = NavLeftEdgeCruiseFinal;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                printf("\r\nNAV failed: final-left bumper\r\n");
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == FINAL_LEFT_EDGE_ACQUIRE_TIMER)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       IsPatternCheckEvent(ThisEvent)) {
                DriveReacquireLeftEdge();
                if (IsPatternCheckEvent(ThisEvent)) {
                    StartPatternCheck();
                }
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavCreepIntoISZ:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                RearAnySeenDuringCreep = false;
                RearWideStableDuringCreep = false;
                Drive_SetBodyVelocity(NAV_ISZ_CREEP_SPEED, 0,
                                      BeaconYawCorrectionSmall());
                ES_Timer_InitTimer(NAV_ISZ_CREEP_TIMER, NAV_ISZ_CREEP_MS);
                StartPatternCheck();
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType) ||
                       IsBeaconSteeringEvent(ThisEvent.EventType) ||
                       (IsPatternCheckEvent(ThisEvent) &&
                        !RearAnySeenDuringCreep)) {
                UpdateCreepRearConfirmation();
                if (RearWideStableDuringCreep) {
                    printf("\r\nNAV ISZ entry confirmed\r\n");
                    nextState = NavISZCenterSettle;
                    makeTransition = TRUE;
                } else {
                    Drive_SetBodyVelocity(NAV_ISZ_CREEP_SPEED, 0,
                                          BeaconYawCorrectionSmall());
                    if (IsPatternCheckEvent(ThisEvent) &&
                        !RearAnySeenDuringCreep) {
                        StartPatternCheck();
                    }
                }
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == ISZ_EXTRA_CREEP_TIMER) &&
                       RearAnySeenDuringCreep) {
                printf("\r\nNAV ISZ entry confirmed\r\n");
                nextState = NavISZCenterSettle;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == NAV_ISZ_CREEP_TIMER)) {
                UpdateCreepRearConfirmation();
                if (RearWideStableDuringCreep || RearAnySeenDuringCreep) {
                    printf("\r\nNAV ISZ entry confirmed\r\n");
                    nextState = NavISZCenterSettle;
                } else {
                    printf("\r\nNAV failed: creep timeout no rear evidence\r\n");
                    nextState = NavFailed;
                }
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (BumperHitStable(ThisEvent)) {
                nextState = NavFailed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavISZCenterSettle:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                StopPatternCheck();
                if ((ISZ_CENTER_SETTLE_ENABLE == 0) ||
                    (IszApproachLane == ISZ_FROM_CENTER_FISHBONE) ||
                    (ISZ_CENTER_BACKUP_MS == 0 &&
                     IszApproachLane == ISZ_FROM_CENTER_FISHBONE)) {
                    Drive_Stop();
                    printf("\r\nNAV ISZ settle complete\r\n");
                    nextState = NavISZConfirmed;
                    makeTransition = TRUE;
                } else if (IszApproachLane == ISZ_FROM_LEFT_EDGE) {
                    Drive_SetBodyVelocity(-ISZ_BACKUP_SPEED,
                                          ISZ_STRAFE_RIGHT_TO_CENTER,
                                          BeaconYawCorrectionSmall());
                    ES_Timer_InitTimer(ISZ_CENTER_SETTLE_TIMER,
                                       ISZ_LEFT_TO_CENTER_MS);
                } else {
                    Drive_SetBodyVelocity(-ISZ_BACKUP_SPEED,
                                          ISZ_STRAFE_LEFT_TO_CENTER,
                                          BeaconYawCorrectionSmall());
                    ES_Timer_InitTimer(ISZ_CENTER_SETTLE_TIMER,
                                       ISZ_RIGHT_TO_CENTER_MS);
                }
                event.EventType = ES_NO_EVENT;
            } else if ((ThisEvent.EventType == ES_TIMEOUT) &&
                       (ThisEvent.EventParam == ISZ_CENTER_SETTLE_TIMER)) {
                Drive_Stop();
                printf("\r\nNAV ISZ settle complete\r\n");
                nextState = NavISZConfirmed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            } else if (IsTapeEvent(ThisEvent.EventType)) {
                if ((uint32_t)(ES_Timer_GetTime() - StateEntryTimeMs) >
                    ISZ_MAX_CENTER_SETTLE_MS) {
                    Drive_Stop();
                    printf("\r\nNAV ISZ settle complete\r\n");
                    nextState = NavISZConfirmed;
                    makeTransition = TRUE;
                }
                event.EventType = ES_NO_EVENT;
            } else if (IsBumperEvent(ThisEvent.EventType)) {
                Drive_Stop();
                nextState = NavISZConfirmed;
                makeTransition = TRUE;
                event.EventType = ES_NO_EVENT;
            }
            break;

        case NavISZConfirmed:
            if (ThisEvent.EventType == ES_ENTRY) {
                EnterNavState(StateNames[CurrentState]);
                Drive_Stop();
                SetTapeGuardMode(TAPE_GUARD_NORMAL);
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
                SetTapeGuardMode(TAPE_GUARD_NORMAL);
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
