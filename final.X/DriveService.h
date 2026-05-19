#ifndef DRIVE_SERVICE_H
#define DRIVE_SERVICE_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitDriveService(uint8_t priority);
uint8_t PostDriveService(ES_Event event);
ES_Event RunDriveService(ES_Event event);

void Drive_Stop(void);
void Drive_SetTank(int16_t leftDuty, int16_t rightDuty);
void Drive_SetBodyVelocity(int forward, int strafe, int yaw);
void Drive_SetRaw(int16_t frontLeft, int16_t frontRight,
                  int16_t rearLeft, int16_t rearRight);

#endif
