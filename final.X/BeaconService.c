#include "BOARD.h"
#include "AD.h"
#include "BeaconService.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "PinMap.h"
#include "ProjectConfig.h"
#include "TopHSM.h"

static uint8_t MyPriority;
static uint8_t BeaconLocked;
static uint32_t LastSeenTime;
static int LastBeaconLeft;
static int LastBeaconCenter;
static int LastBeaconRight;

static void PostBeaconEvent(ES_EventTyp_t type, uint16_t param)
{
    ES_Event event;
    event.EventType = type;
    event.EventParam = param;
    PostTopHSM(event);
}

static void CheckBeacon(void)
{
    unsigned int left = AD_ReadADPin(PIN_BEACON_LEFT_ADC);
    unsigned int center = AD_ReadADPin(PIN_BEACON_CENTER_ADC);
    unsigned int right = AD_ReadADPin(PIN_BEACON_RIGHT_ADC);
    unsigned int best = left;
    ES_EventTyp_t bestEvent = EV_BEACON_LEFT_STRONG;

    LastBeaconLeft = (int)left;
    LastBeaconCenter = (int)center;
    LastBeaconRight = (int)right;

    if (center > best) {
        best = center;
        bestEvent = EV_BEACON_CENTERED;
    }
    if (right > best) {
        best = right;
        bestEvent = EV_BEACON_RIGHT_STRONG;
    }

    if (best > BEACON_LOCK_THRESH) {
        LastSeenTime = ES_Timer_GetTime();
        if (BeaconLocked == FALSE) {
            BeaconLocked = TRUE;
            PostBeaconEvent(EV_BEACON_LOCKED, (uint16_t)best);
        }
        PostBeaconEvent(bestEvent, (uint16_t)best);
    } else if ((BeaconLocked != FALSE) &&
               ((ES_Timer_GetTime() - LastSeenTime) > BEACON_LOST_MS)) {
        BeaconLocked = FALSE;
        PostBeaconEvent(EV_BEACON_LOST, 0);
    }
}

uint8_t InitBeaconService(uint8_t priority)
{
    ES_Event event;

    MyPriority = priority;
    BeaconLocked = FALSE;
    LastSeenTime = 0;
    LastBeaconLeft = 0;
    LastBeaconCenter = 0;
    LastBeaconRight = 0;
    AD_AddPins(PIN_BEACON_ADC_MASK);

    event.EventType = ES_INIT;
    event.EventParam = 0;
    return ES_PostToService(MyPriority, event);
}

uint8_t PostBeaconService(ES_Event event)
{
    return ES_PostToService(MyPriority, event);
}

ES_Event RunBeaconService(ES_Event event)
{
    ES_Event returnEvent;
    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    if (event.EventType == ES_INIT) {
        ES_Timer_InitTimer(BEACON_TIMER, BEACON_POLL_MS);
    } else if ((event.EventType == ES_TIMEOUT) &&
               (event.EventParam == BEACON_TIMER)) {
        CheckBeacon();
        ES_Timer_InitTimer(BEACON_TIMER, BEACON_POLL_MS);
    }

    return returnEvent;
}

int Beacon_GetBearingError(void)
{
    int left = LastBeaconLeft;
    int right = LastBeaconRight;

    return right - left;
}

uint8_t Beacon_IsLocked(void)
{
    return BeaconLocked;
}
