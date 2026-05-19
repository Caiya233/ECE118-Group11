#include "BOARD.h"
#include "DebugService.h"

static uint8_t DebugReady = FALSE;

uint8_t InitDebugService(void)
{
    if (DebugReady == FALSE) {
        Debug_Init();
        DebugReady = TRUE;
    }
    return TRUE;
}

void DebugService_SetTopState(DebugTopState_t state)
{
    if (DebugReady == FALSE) {
        InitDebugService();
    }
    Debug_SetTopState(state);
}
