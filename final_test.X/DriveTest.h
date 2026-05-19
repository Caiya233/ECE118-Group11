#ifndef DRIVE_TEST_H
#define DRIVE_TEST_H

#include <stdint.h>

void DriveTest_Init(void);
void DriveTest_Stop(void);
void DriveTest_SetRaw(int frontLeft, int frontRight, int rearLeft, int rearRight);
void DriveTest_SetBodyVelocity(int forward, int strafe, int yaw);

#endif
