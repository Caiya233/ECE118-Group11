#include "AD.h"
#include "BOARD.h"
#include "BatteryService.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "PinMap.h"
#include "ProjectConfig.h"
#include "TopHSM.h"

static uint8_t MyPriority;
static uint8_t LowBatteryPosted;

uint8_t InitBatteryService(uint8_t priority)
{
    ES_Event event;

    MyPriority = priority;
    LowBatteryPosted = FALSE;
    AD_AddPins(PIN_BATTERY_ADC);

    event.EventType = ES_INIT;
    event.EventParam = 0;
    return ES_PostToService(MyPriority, event);
}

uint8_t PostBatteryService(ES_Event event)
{
    return ES_PostToService(MyPriority, event);
}

ES_Event RunBatteryService(ES_Event event)
{
    ES_Event returnEvent;
    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    if (event.EventType == ES_INIT) {
        ES_Timer_InitTimer(BATTERY_TIMER, BATTERY_POLL_MS);
    } else if ((event.EventType == ES_TIMEOUT) &&
               (event.EventParam == BATTERY_TIMER)) {
        unsigned int battery = AD_ReadADPin(PIN_BATTERY_ADC);

        if ((battery < LOW_BATTERY_THRESH) && (LowBatteryPosted == FALSE)) {
            ES_Event lowEvent;
            lowEvent.EventType = EV_LOW_BATTERY;
            lowEvent.EventParam = (uint16_t)battery;
            PostTopHSM(lowEvent);
            LowBatteryPosted = TRUE;
        } else if (battery >= LOW_BATTERY_THRESH) {
            LowBatteryPosted = FALSE;
        }

        ES_Timer_InitTimer(BATTERY_TIMER, BATTERY_POLL_MS);
    }

    return returnEvent;
}
