#include "BOARD.h"
#include "ServoTest.h"
#include "TestConfig.h"
#include <stdio.h>

void ServoTest_Init(void)
{
#if TEST_SERVO_ENABLED
    RC_Init();
    RC_AddPins(TEST_SERVO_PIN);
    ServoTest_SetPulse(TEST_SERVO_CENTER_US);
#endif
}

void ServoTest_Stop(void)
{
#if TEST_SERVO_ENABLED
    RC_RemovePins(TEST_SERVO_PIN);
#endif
}

uint8_t ServoTest_IsAvailable(void)
{
#if TEST_SERVO_ENABLED
    return TRUE;
#else
    return FALSE;
#endif
}

void ServoTest_SetPulse(unsigned int pulseUs)
{
#if TEST_SERVO_ENABLED
    if (pulseUs < TEST_SERVO_MIN_US) {
        pulseUs = TEST_SERVO_MIN_US;
    } else if (pulseUs > TEST_SERVO_MAX_US) {
        pulseUs = TEST_SERVO_MAX_US;
    }
    RC_SetPulseTime(TEST_SERVO_PIN, pulseUs);
#else
    (void)pulseUs;
    printf("\r\nTODO: no servo pin configured; set TEST_SERVO_ENABLED and TEST_SERVO_PIN.\r\n");
#endif
}
