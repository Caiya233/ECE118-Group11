#ifndef LAUNCHER_SERVICE_H
#define LAUNCHER_SERVICE_H

#include <stdint.h>
#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitLauncherService(uint8_t priority);
uint8_t PostLauncherService(ES_Event event);
ES_Event RunLauncherService(ES_Event event);

void Launcher_SetFlywheelPWM(uint16_t duty);
void Launcher_StartFlywheel(uint16_t duty);
void Launcher_StopFlywheel(void);
void Launcher_BallServerHome(void);
void Launcher_BallServerServePosition(void);
void Launcher_ServeOneBall(void);
void Launcher_Stop(void);

#endif
