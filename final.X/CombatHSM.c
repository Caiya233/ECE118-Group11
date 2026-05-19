#include "BOARD.h"
#include "DriveService.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "LauncherService.h"
#include "CombatHSM.h"
#include "ProjectConfig.h"

typedef enum {
    CombatInitPState,
    CombatAim,
    CombatSpinup,
    CombatServeOne,
    CombatReacquire
} CombatState_t;

static const char *StateNames[] = {
    "CombatInitPState",
    "CombatAim",
    "CombatSpinup",
    "CombatServeOne",
    "CombatReacquire"
};

static CombatState_t CurrentState = CombatInitPState;

uint8_t InitCombatHSM(void)
{
    CurrentState = CombatInitPState;
    return (RunCombatHSM(INIT_EVENT).EventType == ES_NO_EVENT);
}

ES_Event RunCombatHSM(ES_Event event)
{
    ES_Event ThisEvent = event;
    uint8_t makeTransition = FALSE;
    CombatState_t nextState = CurrentState;

    ES_Tattle();

    switch (CurrentState) {
    case CombatInitPState:
        if (event.EventType == ES_INIT) {
            nextState = CombatAim;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case CombatAim:
        if (event.EventType == ES_ENTRY) {
            Drive_Stop();
            Launcher_Stop();
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_BEACON_LEFT_STRONG) {
            Drive_SetTank(-220, 220);
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_BEACON_RIGHT_STRONG) {
            Drive_SetTank(220, -220);
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_BEACON_CENTERED) {
            nextState = CombatSpinup;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case CombatSpinup:
        if (event.EventType == ES_ENTRY) {
            Drive_Stop();
            Launcher_StartFlywheel(FLYWHEEL_COMBAT_PWM);
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_FLYWHEEL_READY) {
            nextState = CombatServeOne;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case CombatServeOne:
        if (event.EventType == ES_ENTRY) {
            Launcher_ServeOneBall();
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_BALL_SERVER_DONE) {
            nextState = CombatReacquire;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case CombatReacquire:
        if (event.EventType == ES_ENTRY) {
            Drive_Stop();
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_BEACON_LEFT_STRONG) {
            Drive_SetTank(-180, 180);
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_BEACON_RIGHT_STRONG) {
            Drive_SetTank(180, -180);
            event.EventType = ES_NO_EVENT;
        } else if (event.EventType == EV_BEACON_CENTERED) {
            nextState = CombatServeOne;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;
    }

    if (makeTransition == TRUE) {
        RunCombatHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunCombatHSM(ENTRY_EVENT);
    }

    ES_Tail();
    return event;
}
