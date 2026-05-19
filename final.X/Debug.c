#include "Debug.h"
#include "ProjectConfig.h"
#include <stdio.h>

#if DEBUG_USE_LED_BANKS
#include "LED.h"
#endif

static DebugTopState_t CurrentDebugState = DEBUG_TOP_WAIT;

static const char *TopStateNames[] = {
    "TopWaitForStart",
    "TopNavigate",
    "TopCombat",
    "TopRecovery",
    "TopFault"
};

void Debug_Init(void)
{
#if DEBUG_USE_LED_BANKS
    LED_Init();
    LED_AddBanks(LED_BANK1);
#endif
    printf("\r\nFinal Project debug ready\r\n");
}

void Debug_SetTopState(DebugTopState_t state)
{
    CurrentDebugState = state;
#if DEBUG_USE_LED_BANKS
    LED_SetBank(LED_BANK1, (unsigned char)(1u << CurrentDebugState));
#endif
    if (CurrentDebugState <= DEBUG_TOP_FAULT) {
        printf("\r\nTop state: %s\r\n", TopStateNames[CurrentDebugState]);
    } else {
        printf("\r\nTop state = %u\r\n", (unsigned int)CurrentDebugState);
    }
}

void Debug_PrintEvent(const char *owner, const char *eventName, uint16_t param)
{
    printf("\r\n%s: %s %u\r\n", owner, eventName, (unsigned int)param);
}

void Debug_PrintStateName(const char *owner, const char *stateName)
{
    printf("\r\n%s state: %s\r\n", owner, stateName);
}
