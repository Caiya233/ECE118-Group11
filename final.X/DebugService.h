#ifndef DEBUG_SERVICE_H
#define DEBUG_SERVICE_H

#include <stdint.h>
#include "Debug.h"

uint8_t InitDebugService(void);
void DebugService_SetTopState(DebugTopState_t state);

#endif
