#include "BOARD.h"
#include "DebugPrint.h"
#include <stdio.h>

static uint8_t QuietMode;
static uint8_t VerboseMode;

void DebugPrint_Init(void)
{
    QuietMode = FALSE;
    VerboseMode = FALSE;
}

void DebugPrint_SetQuiet(uint8_t quiet)
{
    QuietMode = quiet;
}

void DebugPrint_SetVerbose(uint8_t verbose)
{
    VerboseMode = verbose;
}

uint8_t DebugPrint_IsQuiet(void)
{
    return QuietMode;
}

uint8_t DebugPrint_IsVerbose(void)
{
    return VerboseMode;
}

void DebugPrint_Line(const char *text)
{
    if (QuietMode == FALSE) {
        printf("%s\r\n", text);
    }
}
