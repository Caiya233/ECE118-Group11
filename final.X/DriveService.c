#include "BOARD.h"
#include "DriveService.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "PinMap.h"
#include "ProjectConfig.h"
#include "pwm.h"

static uint8_t MyPriority;

static unsigned int DutyFromSigned(int16_t duty)
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

static int AbsInt(int value)
{
    if (value < 0) {
        return -value;
    }
    return value;
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

static void SetDrivePinsSafe(void)
{
    PIN_DRIVE_FRONT_LEFT_PWM_TRIS = 0;
    PIN_DRIVE_FRONT_RIGHT_PWM_TRIS = 0;
    PIN_DRIVE_REAR_LEFT_PWM_TRIS = 0;
    PIN_DRIVE_REAR_RIGHT_PWM_TRIS = 0;
    PIN_DRIVE_FRONT_LEFT_PWM_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_PWM_LAT = 0;
    PIN_DRIVE_REAR_LEFT_PWM_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_PWM_LAT = 0;

    PIN_DRIVE_FRONT_LEFT_DIR1_TRIS = 0;
    PIN_DRIVE_FRONT_LEFT_DIR2_TRIS = 0;
    PIN_DRIVE_REAR_LEFT_DIR1_TRIS = 0;
    PIN_DRIVE_REAR_LEFT_DIR2_TRIS = 0;
    PIN_DRIVE_FRONT_RIGHT_DIR1_TRIS = 0;
    PIN_DRIVE_FRONT_RIGHT_DIR2_TRIS = 0;
    PIN_DRIVE_REAR_RIGHT_DIR1_TRIS = 0;
    PIN_DRIVE_REAR_RIGHT_DIR2_TRIS = 0;

    PIN_DRIVE_FRONT_LEFT_DIR1_LAT = 0;
    PIN_DRIVE_FRONT_LEFT_DIR2_LAT = 0;
    PIN_DRIVE_REAR_LEFT_DIR1_LAT = 0;
    PIN_DRIVE_REAR_LEFT_DIR2_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_DIR1_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_DIR2_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_DIR1_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_DIR2_LAT = 0;
}

uint8_t InitDriveService(uint8_t priority)
{
    ES_Event event;

    MyPriority = priority;
    SetDrivePinsSafe();

    PWM_Init();
    PWM_SetFrequency(PWM_1KHZ);
    PWM_AddPins(PIN_DRIVE_PWM_MASK);
    Drive_Stop();

    event.EventType = ES_INIT;
    event.EventParam = 0;
    return ES_PostToService(MyPriority, event);
}

uint8_t PostDriveService(ES_Event event)
{
    return ES_PostToService(MyPriority, event);
}

ES_Event RunDriveService(ES_Event event)
{
    ES_Event returnEvent;
    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    if (event.EventType == ES_INIT) {
        Drive_Stop();
    }

    return returnEvent;
}

void Drive_Stop(void)
{
    PWM_SetDutyCycle(PIN_DRIVE_FRONT_LEFT_PWM, 0);
    PWM_SetDutyCycle(PIN_DRIVE_FRONT_RIGHT_PWM, 0);
    PWM_SetDutyCycle(PIN_DRIVE_REAR_LEFT_PWM, 0);
    PWM_SetDutyCycle(PIN_DRIVE_REAR_RIGHT_PWM, 0);

    PIN_DRIVE_FRONT_LEFT_DIR1_LAT = 0;
    PIN_DRIVE_FRONT_LEFT_DIR2_LAT = 0;
    PIN_DRIVE_REAR_LEFT_DIR1_LAT = 0;
    PIN_DRIVE_REAR_LEFT_DIR2_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_DIR1_LAT = 0;
    PIN_DRIVE_FRONT_RIGHT_DIR2_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_DIR1_LAT = 0;
    PIN_DRIVE_REAR_RIGHT_DIR2_LAT = 0;
}

void Drive_SetTank(int16_t leftDuty, int16_t rightDuty)
{
    Drive_SetRaw(leftDuty * DRIVE_FL_SIGN, rightDuty * DRIVE_FR_SIGN,
                 leftDuty * DRIVE_RL_SIGN, rightDuty * DRIVE_RR_SIGN);
}

void Drive_SetBodyVelocity(int forward, int strafe, int yaw)
{
    int fl = (forward + strafe + yaw) * DRIVE_FL_SIGN;
    int fr = (forward - strafe - yaw) * DRIVE_FR_SIGN;
    int rl = (forward - strafe + yaw) * DRIVE_RL_SIGN;
    int rr = (forward + strafe - yaw) * DRIVE_RR_SIGN;

    // Mecanum signs must be verified on the real robot. If pure yaw or pure strafe
    // produces translation/rotation error, fix motor polarity or roller orientation first.
    NormalizeWheelCommands(&fl, &fr, &rl, &rr);
    Drive_SetRaw(fl, fr, rl, rr);
}

void Drive_SetRaw(int16_t frontLeft, int16_t frontRight,
                  int16_t rearLeft, int16_t rearRight)
{
    PIN_DRIVE_FRONT_LEFT_DIR1_LAT = (frontLeft >= 0);
    PIN_DRIVE_FRONT_LEFT_DIR2_LAT = (frontLeft < 0);
    PIN_DRIVE_REAR_LEFT_DIR1_LAT = (rearLeft >= 0);
    PIN_DRIVE_REAR_LEFT_DIR2_LAT = (rearLeft < 0);
    PIN_DRIVE_FRONT_RIGHT_DIR1_LAT = (frontRight >= 0);
    PIN_DRIVE_FRONT_RIGHT_DIR2_LAT = (frontRight < 0);
    PIN_DRIVE_REAR_RIGHT_DIR1_LAT = (rearRight >= 0);
    PIN_DRIVE_REAR_RIGHT_DIR2_LAT = (rearRight < 0);

    PWM_SetDutyCycle(PIN_DRIVE_FRONT_LEFT_PWM, DutyFromSigned(frontLeft));
    PWM_SetDutyCycle(PIN_DRIVE_FRONT_RIGHT_PWM, DutyFromSigned(frontRight));
    PWM_SetDutyCycle(PIN_DRIVE_REAR_LEFT_PWM, DutyFromSigned(rearLeft));
    PWM_SetDutyCycle(PIN_DRIVE_REAR_RIGHT_PWM, DutyFromSigned(rearRight));
}
