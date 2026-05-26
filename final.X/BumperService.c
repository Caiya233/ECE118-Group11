#include "BOARD.h"
#include "BumperService.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "PinMap.h"
#include "ProjectConfig.h"
#include "TopHSM.h"

#define BUMP_FRONT_L 0x01
#define BUMP_FRONT_R 0x02
#define BUMP_LEFT_F  0x04
#define BUMP_LEFT_R  0x08
#define BUMP_RIGHT_F 0x10
#define BUMP_RIGHT_R 0x20
#define BUMP_REAR_L  0x40
#define BUMP_REAR_R  0x80

static uint8_t MyPriority;
static uint8_t LastSample;
static uint8_t StableMask;
static uint8_t DebounceCount;

static uint8_t Active(uint8_t pinValue)
{
    return (pinValue == BUMPER_ACTIVE_LEVEL);
}

static void InitBumperPins(void)
{
    PIN_BUMPER_FRONT_L_TRIS = 1;
    PIN_BUMPER_FRONT_R_TRIS = 1;
    PIN_BUMPER_LEFT_F_TRIS = 1;
    PIN_BUMPER_LEFT_R_TRIS = 1;
    PIN_BUMPER_RIGHT_F_TRIS = 1;
    PIN_BUMPER_RIGHT_R_TRIS = 1;
    PIN_BUMPER_REAR_L_TRIS = 1;
    PIN_BUMPER_REAR_R_TRIS = 1;
}

static uint8_t ReadBumpers(void)
{
    uint8_t mask = 0;

    if (Active(PIN_BUMPER_FRONT_L_BIT)) {
        mask |= BUMP_FRONT_L;
    }
    if (Active(PIN_BUMPER_FRONT_R_BIT)) {
        mask |= BUMP_FRONT_R;
    }
    if (Active(PIN_BUMPER_LEFT_F_BIT)) {
        mask |= BUMP_LEFT_F;
    }
    if (Active(PIN_BUMPER_LEFT_R_BIT)) {
        mask |= BUMP_LEFT_R;
    }
    if (Active(PIN_BUMPER_RIGHT_F_BIT)) {
        mask |= BUMP_RIGHT_F;
    }
    if (Active(PIN_BUMPER_RIGHT_R_BIT)) {
        mask |= BUMP_RIGHT_R;
    }
    if (Active(PIN_BUMPER_REAR_L_BIT)) {
        mask |= BUMP_REAR_L;
    }
    if (Active(PIN_BUMPER_REAR_R_BIT)) {
        mask |= BUMP_REAR_R;
    }

    return mask;
}

static void PostBumperGroup(uint8_t mask)
{
    ES_Event event;
    event.EventParam = mask;

    event.EventType = EV_BUMPER_HIT;
    PostTopHSM(event);

    if ((mask & (BUMP_FRONT_L | BUMP_FRONT_R)) != 0u) {
        event.EventType = EV_BUMPER_FRONT;
    } else if ((mask & (BUMP_LEFT_F | BUMP_LEFT_R)) != 0u) {
        event.EventType = EV_BUMPER_LEFT;
    } else if ((mask & (BUMP_RIGHT_F | BUMP_RIGHT_R)) != 0u) {
        event.EventType = EV_BUMPER_RIGHT;
    } else if ((mask & (BUMP_REAR_L | BUMP_REAR_R)) != 0u) {
        event.EventType = EV_BUMPER_REAR;
    } else {
        return;
    }

    PostTopHSM(event);
}

static void CheckBumpers(void)
{
    uint8_t sample = ReadBumpers();

    if (sample == LastSample) {
        if (DebounceCount < BUMPER_DEBOUNCE_TICKS) {
            DebounceCount++;
        }
    } else {
        DebounceCount = 0;
        LastSample = sample;
    }

    if ((DebounceCount >= BUMPER_DEBOUNCE_TICKS) &&
        (sample != StableMask)) {
        uint8_t changedOn = (uint8_t)(sample & (uint8_t)~StableMask);
        StableMask = sample;
        if (changedOn != 0u) {
            PostBumperGroup(changedOn);
        }
    }
}

uint8_t InitBumperService(uint8_t priority)
{
    ES_Event event;

    MyPriority = priority;
    InitBumperPins();
    LastSample = ReadBumpers();
    StableMask = LastSample;
    DebounceCount = 0;

    event.EventType = ES_INIT;
    event.EventParam = 0;
    return ES_PostToService(MyPriority, event);
}

uint8_t PostBumperService(ES_Event event)
{
    return ES_PostToService(MyPriority, event);
}

ES_Event RunBumperService(ES_Event event)
{
    ES_Event returnEvent;
    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    if (event.EventType == ES_INIT) {
        ES_Timer_InitTimer(BUMPER_TIMER, BUMPER_POLL_MS);
    } else if ((event.EventType == ES_TIMEOUT) &&
               (event.EventParam == BUMPER_TIMER)) {
        CheckBumpers();
        ES_Timer_InitTimer(BUMPER_TIMER, BUMPER_POLL_MS);
    }

    return returnEvent;
}
