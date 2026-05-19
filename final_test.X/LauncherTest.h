#ifndef LAUNCHER_TEST_H
#define LAUNCHER_TEST_H

#include <stdint.h>

void LauncherTest_Init(void);
void LauncherTest_StopAll(void);
void LauncherTest_SetFlywheel(unsigned int pwm);
void LauncherTest_StopFlywheel(void);
void LauncherTest_BallServerHome(void);
void LauncherTest_BallServerServePosition(void);
void LauncherTest_SetIrEmitter(uint8_t enabled);
uint8_t LauncherTest_IsIrEmitterEnabled(void);

#endif
