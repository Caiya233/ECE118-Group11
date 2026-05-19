#include "BOARD.h"
#include "DriveService.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "LauncherService.h"
#include "ProjectConfig.h"
#include "RecoveryHSM.h"
#include "TopHSM.h"

typedef enum {
    RecoveryInitPState,
    RecoveryBackup,
    RecoveryTurn,
    RecoveryDone
} RecoveryState_t;

static const char *StateNames[] = {
    "RecoveryInitPState",
    "RecoveryBackup",
    "RecoveryTurn",
    "RecoveryDone"
};

static RecoveryState_t CurrentState = RecoveryInitPState;

uint8_t InitRecoveryHSM(void)
{
    CurrentState = RecoveryInitPState;
    return (RunRecoveryHSM(INIT_EVENT).EventType == ES_NO_EVENT);
}

ES_Event RunRecoveryHSM(ES_Event event)
{
    ES_Event ThisEvent = event;
    uint8_t makeTransition = FALSE;
    RecoveryState_t nextState = CurrentState;

    ES_Tattle();

    switch (CurrentState) {
    case RecoveryInitPState:
        if (event.EventType == ES_INIT) {
            nextState = RecoveryBackup;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case RecoveryBackup:
        if (event.EventType == ES_ENTRY) {
            Launcher_Stop();
            Drive_SetTank(-300, -300);
            ES_Timer_InitTimer(RECOVERY_TIMER, RECOVERY_BACKUP_MS);
            event.EventType = ES_NO_EVENT;
        } else if ((event.EventType == ES_TIMEOUT) &&
                   (event.EventParam == RECOVERY_TIMER)) {
            nextState = RecoveryTurn;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case RecoveryTurn:
        if (event.EventType == ES_ENTRY) {
            Drive_SetTank(300, -300);
            ES_Timer_InitTimer(RECOVERY_TIMER, RECOVERY_TURN_MS);
            event.EventType = ES_NO_EVENT;
        } else if ((event.EventType == ES_TIMEOUT) &&
                   (event.EventParam == RECOVERY_TIMER)) {
            nextState = RecoveryDone;
            makeTransition = TRUE;
            event.EventType = ES_NO_EVENT;
        }
        break;

    case RecoveryDone:
        if (event.EventType == ES_ENTRY) {
            ES_Event doneEvent;
            Drive_Stop();
            doneEvent.EventType = EV_RECOVERY_DONE;
            doneEvent.EventParam = 0;
            PostTopHSM(doneEvent);
            event.EventType = ES_NO_EVENT;
        }
        break;
    }

    if (makeTransition == TRUE) {
        RunRecoveryHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunRecoveryHSM(ENTRY_EVENT);
    }

    ES_Tail();
    return event;
}
