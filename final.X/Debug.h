#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

typedef enum {
    DEBUG_TOP_WAIT = 0,
    DEBUG_TOP_NAVIGATE,
    DEBUG_TOP_COMBAT,
    DEBUG_TOP_RECOVERY,
    DEBUG_TOP_FAULT
} DebugTopState_t;

void Debug_Init(void);
void Debug_SetTopState(DebugTopState_t state);
void Debug_PrintEvent(const char *owner, const char *eventName, uint16_t param);
void Debug_PrintStateName(const char *owner, const char *stateName);

#endif
