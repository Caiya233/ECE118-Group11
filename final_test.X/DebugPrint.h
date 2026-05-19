#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <stdint.h>

void DebugPrint_Init(void);
void DebugPrint_SetQuiet(uint8_t quiet);
void DebugPrint_SetVerbose(uint8_t verbose);
uint8_t DebugPrint_IsQuiet(void);
uint8_t DebugPrint_IsVerbose(void);
void DebugPrint_Line(const char *text);

#endif
