#include "BOARD.h"
#include "DriveTest.h"
#include "PinMap.h"
#include "TestConfig.h"
#include "pwm.h"

static int AbsInt(int value)
{
    if (value < 0) {
        return -value;
    }
    return value;
}

static unsigned int DutyFromSigned(int duty)
{
    if (duty < 0) {
        duty = -duty;
    }
    if (duty > DRIVE_PWM_MAX) {
        duty = DRIVE_PWM_MAX;
    }
    if ((duty > 0) && (duty < DRIVE_PWM_MIN_EFFECTIVE)) {
        duty = DRIVE_PWM_MIN_EFFECTIVE;
    }
    return (unsigned int)duty;
}

static void NormalizeWheelCommands(int *fl, int *fr, int *rl, int *rr)
{
    int max = AbsInt(*fl);

    if (AbsInt(*fr) > max) {
        max = AbsInt(*fr);
    }
    if (AbsInt(*rl) > max) {
        max = AbsInt(*rl);
    }
    if (AbsInt(*rr) > max) {
        max = AbsInt(*rr);
    }

    if (max > DRIVE_PWM_MAX) {
        *fl = (*fl * DRIVE_PWM_MAX) / max;
        *fr = (*fr * DRIVE_PWM_MAX) / max;
        *rl = (*rl * DRIVE_PWM_MAX) / max;
        *rr = (*rr * DRIVE_PWM_MAX) / max;
    }
}

static void InitDrivePins(void)
{
    PIN_DRIVE_FRONT_LEFT_IN1_TRIS = 0;
    PIN_DRIVE_FRONT_LEFT_IN2_TRIS = 0;
    PIN_DRIVE_FRONT_RIGHT_IN1_TRIS = 0;
    PIN_DRIVE_FRONT_RIGHT_IN2_TRIS = 0;
    PIN_DRIVE_REAR_LEFT_IN1_TRIS = 0;
    PIN_DRIVE_REAR_LEFT_IN2_TRIS = 0;
    PIN_DRIVE_REAR_RIGHT_IN1_TRIS = 0;
    PIN_DRIVE_REAR_RIGHT_IN2_TRIS = 0;

    PIN_DRIVE_FRONT_LEFT_IN1_LAT = 0;
    PIN_DRIVE_FRONT_LEFT_IN2_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_IN1_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_IN2_LAT = 0;
    PIN_DRIVE_REAR_LEFT_IN1_LAT = 0;
    PIN_DRIVE_REAR_LEFT_IN2_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_IN1_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_IN2_LAT = 0;
}

void DriveTest_Init(void)
{
    InitDrivePins();
    PWM_Init();
    PWM_SetFrequency(PWM_1KHZ);
    PWM_AddPins(PIN_DRIVE_PWM_MASK);
    DriveTest_Stop();
}

void DriveTest_Stop(void)
{
    PWM_SetDutyCycle(PIN_DRIVE_FRONT_LEFT_PWM, 0);
    PWM_SetDutyCycle(PIN_DRIVE_FRONT_RIGHT_PWM, 0);
    PWM_SetDutyCycle(PIN_DRIVE_REAR_LEFT_PWM, 0);
    PWM_SetDutyCycle(PIN_DRIVE_REAR_RIGHT_PWM, 0);

    PIN_DRIVE_FRONT_LEFT_IN1_LAT = 0;
    PIN_DRIVE_FRONT_LEFT_IN2_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_IN1_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_IN2_LAT = 0;
    PIN_DRIVE_REAR_LEFT_IN1_LAT = 0;
    PIN_DRIVE_REAR_LEFT_IN2_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_IN1_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_IN2_LAT = 0;
}

void DriveTest_SetRaw(int frontLeft, int frontRight, int rearLeft, int rearRight)
{
    PIN_DRIVE_FRONT_LEFT_IN1_LAT = (frontLeft >= 0);
    PIN_DRIVE_FRONT_LEFT_IN2_LAT = (frontLeft < 0);
    PIN_DRIVE_FRONT_RIGHT_IN1_LAT = (frontRight >= 0);
    PIN_DRIVE_FRONT_RIGHT_IN2_LAT = (frontRight < 0);
    PIN_DRIVE_REAR_LEFT_IN1_LAT = (rearLeft >= 0);
    PIN_DRIVE_REAR_LEFT_IN2_LAT = (rearLeft < 0);
    PIN_DRIVE_REAR_RIGHT_IN1_LAT = (rearRight >= 0);
    PIN_DRIVE_REAR_RIGHT_IN2_LAT = (rearRight < 0);

    PWM_SetDutyCycle(PIN_DRIVE_FRONT_LEFT_PWM, DutyFromSigned(frontLeft));
    PWM_SetDutyCycle(PIN_DRIVE_FRONT_RIGHT_PWM, DutyFromSigned(frontRight));
    PWM_SetDutyCycle(PIN_DRIVE_REAR_LEFT_PWM, DutyFromSigned(rearLeft));
    PWM_SetDutyCycle(PIN_DRIVE_REAR_RIGHT_PWM, DutyFromSigned(rearRight));
}

void DriveTest_SetBodyVelocity(int forward, int strafe, int yaw)
{
    int fl = (forward + strafe + yaw) * DRIVE_FL_SIGN;
    int fr = (forward - strafe - yaw) * DRIVE_FR_SIGN;
    int rl = (forward - strafe + yaw) * DRIVE_RL_SIGN;
    int rr = (forward + strafe - yaw) * DRIVE_RR_SIGN;

    NormalizeWheelCommands(&fl, &fr, &rl, &rr);
    DriveTest_SetRaw(fl, fr, rl, rr);
}
