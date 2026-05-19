#ifndef SERVO_TEST_H
#define SERVO_TEST_H

#include <stdint.h>

void ServoTest_Init(void);
void ServoTest_Stop(void);
uint8_t ServoTest_IsAvailable(void);
void ServoTest_SetPulse(unsigned int pulseUs);

#endif
