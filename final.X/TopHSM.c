#include "BOARD.h"
#include "CombatHSM.h"
#include "DebugService.h"
#include "DriveService.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "LauncherService.h"
#include "NavigateHSM.h"
#include "RecoveryHSM.h"
#include "TopHSM.h"

typedef enum {
    TopInitPState,
    TopWaitForStart,
    TopNavigate,
    TopCombat,
    TopRecovery,
    TopFault
} TopHSMState_t;

static const char *StateNames[] = {
    "TopInitPState",
    "TopWaitForStart",
    "TopNavigate",
    "TopCombat",
    "TopRecovery",
    "TopFault"
};

static TopHSMState_t CurrentState = TopInitPState;
static uint8_t MyPriority;
static uint8_t IszConfirmed;
static uint8_t ReturnToCombatAfterRecovery;

static uint8_t IsBumperEvent(ES_EventTyp_t eventType)
{
    return ((eventType == EV_BUMPER_HIT) ||
            (eventType == EV_BUMPER_FRONT) ||
            (eventType == EV_BUMPER_LEFT) ||
            (eventType == EV_BUMPER_RIGHT) ||
            (eventType == EV_BUMPER_REAR));
}

static uint8_t IsRecoveryEvent(ES_EventTyp_t eventType)
{
    return (IsBumperEvent(eventType) ||
            (eventType == EV_BOUNDARY_DETECTED) ||
            (eventType == EV_TAPE_PANIC) ||
            (eventType == EV_NAV_FAILED));
}

static uint8_t IsNavigateRecoveryEvent(ES_EventTyp_t eventType)
{
    return ((eventType == EV_BOUNDARY_DETECTED) ||
            (eventType == EV_TAPE_PANIC) ||
            (eventType == EV_NAV_FAILED));
}

uint8_t InitTopHSM(uint8_t priority)
{
    MyPriority = priority;
    CurrentState = TopInitPState;
    IszConfirmed = FALSE;
    ReturnToCombatAfterRecovery = FALSE;
    return ES_PostToService(MyPriority, INIT_EVENT);
}

uint8_t PostTopHSM(ES_Event event)
{
    return ES_PostToService(MyPriority, event);
}

ES_Event RunTopHSM(ES_Event event)
{
    ES_Event ThisEvent = event;
    uint8_t makeTransition = FALSE;
    TopHSMState_t nextState = CurrentState;

    ES_Tattle();

    switch (CurrentState) {
    case TopInitPState:
        if (event.EventType == ES_INIT) {
            InitDebugService();
            nextState = TopWaitForStart;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case TopWaitForStart:
        if (event.EventType == ES_ENTRY) {
            Drive_Stop();
            Launcher_Stop();
            IszConfirmed = FALSE;
            ReturnToCombatAfterRecovery = FALSE;
            DebugService_SetTopState(DEBUG_TOP_WAIT);
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_START_BUTTON) {
            nextState = TopNavigate;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case TopNavigate:
        if (event.EventType == ES_ENTRY) {
            InitNavigateHSM();
            Launcher_Stop();
            DebugService_SetTopState(DEBUG_TOP_NAVIGATE);
            event.EventType = ES_NO_EVENT;
            break;
        }
        event = RunNavigateHSM(event);
        if (event.EventType == EV_ISZ_CONFIRMED) {
            IszConfirmed = TRUE;
            ReturnToCombatAfterRecovery = FALSE;
            nextState = TopCombat;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        } else if (IsNavigateRecoveryEvent(event.EventType) != FALSE) {
            ReturnToCombatAfterRecovery = FALSE;
            nextState = TopRecovery;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case TopCombat:
        if (ThisEvent.EventType == ES_EXIT) {
            event = RunCombatHSM(event);
            Launcher_Stop();
            event.EventType = ES_NO_EVENT;
            break;
        }
        if (event.EventType == ES_ENTRY) {
            InitCombatHSM();
            DebugService_SetTopState(DEBUG_TOP_COMBAT);
            event.EventType = ES_NO_EVENT;
            break;
        }
        event = RunCombatHSM(event);
        if ((event.EventType == EV_BEACON_LOST) ||
            (event.EventType == EV_MAG_EMPTY)) {
            ReturnToCombatAfterRecovery = FALSE;
            nextState = TopNavigate;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        } else if (IsRecoveryEvent(event.EventType) != FALSE) {
            ReturnToCombatAfterRecovery = IszConfirmed;
            nextState = TopRecovery;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case TopRecovery:
        if (event.EventType == ES_ENTRY) {
            Drive_Stop();
            Launcher_Stop();
            InitRecoveryHSM();
            DebugService_SetTopState(DEBUG_TOP_RECOVERY);
            event.EventType = ES_NO_EVENT;
            break;
        }
        event = RunRecoveryHSM(event);
        if (event.EventType == EV_RECOVERY_DONE) {
            if ((IszConfirmed != FALSE) &&
                (ReturnToCombatAfterRecovery != FALSE)) {
                nextState = TopCombat;
            } else {
                nextState = TopNavigate;
            }
            ReturnToCombatAfterRecovery = FALSE;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case TopFault:
        if (event.EventType == ES_ENTRY) {
            Drive_Stop();
            Launcher_Stop();
            DebugService_SetTopState(DEBUG_TOP_FAULT);
            event.EventType = ES_NO_EVENT;
        }
        break;
    }

    if ((event.EventType == EV_LOW_BATTERY) || (event.EventType == EV_FAULT)) {
        nextState = TopFault;
        makeTransition = TRUE;
        event.EventType = ES_NO_EVENT;
    }

    if (makeTransition == TRUE) {
        RunTopHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunTopHSM(ENTRY_EVENT);
    }

    ES_Tail();
    return event;
}
