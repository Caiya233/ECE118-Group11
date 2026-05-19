#include "BOARD.h"
#include "AD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "PinMap.h"
#include "ProjectConfig.h"
#include "TopHSM.h"
#include "TrackWireService.h"

static uint8_t MyPriority;
static uint8_t TrackMask;

static void PostTrackEvent(ES_EventTyp_t type, uint16_t param)
{
    ES_Event event;
    event.EventType = type;
    event.EventParam = param;
    PostTopHSM(event);
}

static void CheckTrackWire(void)
{
    uint8_t newMask = 0;

    if (AD_ReadADPin(PIN_TRACKWIRE_LEFT_ADC) > TRACKWIRE_THRESH) {
        newMask |= TRACKWIRE_LEFT_MASK;
    }
    if (AD_ReadADPin(PIN_TRACKWIRE_RIGHT_ADC) > TRACKWIRE_THRESH) {
        newMask |= TRACKWIRE_RIGHT_MASK;
    }

    if (newMask != TrackMask) {
        TrackMask = newMask;
        /* Coil/track-wire changes are diagnostic only, never obstacle position. */
        if (TrackMask == TRACKWIRE_BOTH_MASK) {
            PostTrackEvent(EV_TRACKWIRE_BOTH, TrackMask);
        } else if ((TrackMask & TRACKWIRE_LEFT_MASK) != 0u) {
            PostTrackEvent(EV_TRACKWIRE_LEFT, TrackMask);
        } else if ((TrackMask & TRACKWIRE_RIGHT_MASK) != 0u) {
            PostTrackEvent(EV_TRACKWIRE_RIGHT, TrackMask);
        }
    }
}

uint8_t InitTrackWireService(uint8_t priority)
{
    ES_Event event;

    MyPriority = priority;
    TrackMask = 0;
    AD_AddPins(PIN_TRACKWIRE_ADC_MASK);

    event.EventType = ES_INIT;
    event.EventParam = 0;
    return ES_PostToService(MyPriority, event);
}

uint8_t PostTrackWireService(ES_Event event)
{
    return ES_PostToService(MyPriority, event);
}

ES_Event RunTrackWireService(ES_Event event)
{
    ES_Event returnEvent;
    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    if (event.EventType == ES_INIT) {
        ES_Timer_InitTimer(TRACKWIRE_TIMER, TRACKWIRE_POLL_MS);
    } else if ((event.EventType == ES_TIMEOUT) &&
               (event.EventParam == TRACKWIRE_TIMER)) {
        CheckTrackWire();
        ES_Timer_InitTimer(TRACKWIRE_TIMER, TRACKWIRE_POLL_MS);
    }

    return returnEvent;
}
