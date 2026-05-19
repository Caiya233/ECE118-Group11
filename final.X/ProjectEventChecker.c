#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "PinMap.h"
#include "ProjectEventChecker.h"
#include "TopHSM.h"

uint8_t CheckProjectEvents(void)
{
    static uint8_t initialized = FALSE;
    static uint8_t lastStart = 0;
    uint8_t nowStart;

    PIN_START_BUTTON_TRIS = 1;
    nowStart = (PIN_START_BUTTON_BIT != 0);

    if (initialized == FALSE) {
        lastStart = nowStart;
        initialized = TRUE;
        return FALSE;
    }

    if ((nowStart != 0) && (lastStart == 0)) {
        ES_Event event;
        event.EventType = EV_START_BUTTON;
        event.EventParam = 0;
        PostTopHSM(event);
        lastStart = nowStart;
        return TRUE;
    }

    lastStart = nowStart;
    return FALSE;
}
