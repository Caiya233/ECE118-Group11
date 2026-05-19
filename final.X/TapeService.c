#include "BOARD.h"
#include "AD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "PinMap.h"
#include "ProjectConfig.h"
#include "TapeService.h"
#include "TopHSM.h"

#define NUM_TAPE_SENSORS 7

static uint8_t MyPriority;
static uint8_t TapeMask;

static const unsigned int TapePins[NUM_TAPE_SENSORS] = {
    PIN_TAPE_FRONT_L_ADC,
    PIN_TAPE_FRONT_LM_ADC,
    PIN_TAPE_FRONT_M_ADC,
    PIN_TAPE_FRONT_MR_ADC,
    PIN_TAPE_FRONT_R_ADC,
    PIN_TAPE_REAR_L_ADC,
    PIN_TAPE_REAR_R_ADC
};

static const ES_EventTyp_t TapeEvents[NUM_TAPE_SENSORS] = {
    EV_TAPE_FRONT_L,
    EV_TAPE_FRONT_LM,
    EV_TAPE_FRONT_M,
    EV_TAPE_FRONT_MR,
    EV_TAPE_FRONT_R,
    EV_TAPE_REAR_L,
    EV_TAPE_REAR_R
};

static void PostTapeEvent(ES_EventTyp_t type, uint16_t param)
{
    ES_Event event;
    event.EventType = type;
    event.EventParam = param;
    PostTopHSM(event);
}

static uint8_t CountBits(uint8_t mask)
{
    uint8_t count = 0;

    while (mask != 0u) {
        if ((mask & 0x01u) != 0u) {
            count++;
        }
        mask >>= 1;
    }

    return count;
}

static uint8_t BuildFrontMask(uint8_t sensorMask)
{
    return (uint8_t)(sensorMask & (TAPE_F_L_BIT |
                                   TAPE_F_LM_BIT |
                                   TAPE_F_M_BIT |
                                   TAPE_F_RM_BIT |
                                   TAPE_F_R_BIT));
}

static uint8_t BuildRearMask(uint8_t sensorMask)
{
    uint8_t rearMask = 0;

    if ((sensorMask & (1u << 5)) != 0u) {
        rearMask |= TAPE_R_LM_BIT;
    }
    if ((sensorMask & (1u << 6)) != 0u) {
        rearMask |= TAPE_R_RM_BIT;
    }

    return rearMask;
}

static uint8_t IsFrontCross(uint8_t frontMask)
{
    return (CountBits(frontMask) >= FRONT_WIDE_MIN_COUNT);
}

static uint8_t IsFrontAllBlack(uint8_t frontMask)
{
    return ((frontMask & (TAPE_F_L_BIT | TAPE_F_LM_BIT | TAPE_F_M_BIT |
                          TAPE_F_RM_BIT | TAPE_F_R_BIT)) ==
            (TAPE_F_L_BIT | TAPE_F_LM_BIT | TAPE_F_M_BIT |
             TAPE_F_RM_BIT | TAPE_F_R_BIT));
}

static void CheckTapeSensors(void)
{
    uint8_t i;
    uint8_t newMask = TapeMask;

    for (i = 0; i < NUM_TAPE_SENSORS; i++) {
        unsigned int value = AD_ReadADPin(TapePins[i]);
        uint8_t bit = (uint8_t)(1u << i);

        if ((TapeMask & bit) != 0u) {
            if (value < TAPE_EXIT_THRESH) {
                newMask &= (uint8_t)~bit;
            }
        } else if (value > TAPE_ENTER_THRESH) {
            newMask |= bit;
        }
    }

    if (newMask != TapeMask) {
        uint8_t changed = (uint8_t)(newMask ^ TapeMask);
        uint8_t oldFrontMask = BuildFrontMask(TapeMask);
        uint8_t oldRearMask = BuildRearMask(TapeMask);
        uint8_t newFrontMask = BuildFrontMask(newMask);
        uint8_t newRearMask = BuildRearMask(newMask);

        TapeMask = newMask;
        PostTapeEvent(EV_TAPE_MASK_CHANGED,
                      (uint16_t)(newFrontMask | ((uint16_t)newRearMask << 8)));

        if (TapeMask == 0u) {
            PostTapeEvent(EV_TAPE_LOST, 0);
        } else {
            for (i = 0; i < NUM_TAPE_SENSORS; i++) {
                if ((changed & (1u << i)) != 0u) {
                    PostTapeEvent(TapeEvents[i], TapeMask);
                    break;
                }
            }
            if ((IsFrontCross(oldFrontMask) == FALSE) &&
                (IsFrontCross(newFrontMask) != FALSE)) {
                PostTapeEvent(EV_FRONT_WIDE_BLACK, newFrontMask);
                PostTapeEvent(EV_TAPE_FRONT_CROSS, newFrontMask);
            }
            if ((IsFrontAllBlack(oldFrontMask) == FALSE) &&
                (IsFrontAllBlack(newFrontMask) != FALSE)) {
                PostTapeEvent(EV_FRONT_ALL_BLACK, newFrontMask);
            }
            if ((oldRearMask == 0u) && (newRearMask != 0u)) {
                PostTapeEvent(EV_TAPE_REAR_CROSS, newRearMask);
            }
        }
    }
}

uint8_t InitTapeService(uint8_t priority)
{
    ES_Event event;

    MyPriority = priority;
    TapeMask = 0;
    AD_AddPins(PIN_TAPE_ADC_MASK);

    event.EventType = ES_INIT;
    event.EventParam = 0;
    return ES_PostToService(MyPriority, event);
}

uint8_t PostTapeService(ES_Event event)
{
    return ES_PostToService(MyPriority, event);
}

ES_Event RunTapeService(ES_Event event)
{
    ES_Event returnEvent;
    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    if (event.EventType == ES_INIT) {
        ES_Timer_InitTimer(TAPE_TIMER, TAPE_POLL_MS);
    } else if ((event.EventType == ES_TIMEOUT) &&
               (event.EventParam == TAPE_TIMER)) {
        CheckTapeSensors();
        ES_Timer_InitTimer(TAPE_TIMER, TAPE_POLL_MS);
    }

    return returnEvent;
}

uint8_t Tape_GetFrontMask(void)
{
    return BuildFrontMask(TapeMask);
}

uint8_t Tape_GetRearMask(void)
{
    return BuildRearMask(TapeMask);
}
