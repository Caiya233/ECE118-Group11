#include "BOARD.h"
#include "LauncherTest.h"
#include "PinMap.h"
#include "RC_Servo.h"
#include "TestConfig.h"
#include "pwm.h"

static uint8_t IrEmitterEnabled;

void LauncherTest_Init(void)
{
    PIN_LAUNCHER_ENABLE_TRIS = 0;
    PIN_IR_EMITTER_GATE_TRIS = 0;

    PIN_LAUNCHER_ENABLE_LAT = 0;
    PIN_IR_EMITTER_GATE_LAT = 0;
    IrEmitterEnabled = FALSE;

    PWM_AddPins(PIN_LAUNCHER_FLYWHEEL_PWM);
    RC_Init();
    RC_AddPins(BALL_SERVER_SERVO_RC);
    LauncherTest_StopAll();
}

void LauncherTest_StopAll(void)
{
    LauncherTest_StopFlywheel();
    LauncherTest_BallServerHome();
    LauncherTest_SetIrEmitter(FALSE);
}

void LauncherTest_SetFlywheel(unsigned int pwm)
{
    if (pwm == 0u) {
        LauncherTest_StopFlywheel();
    } else {
        if (pwm > DRIVE_PWM_MAX) {
            pwm = DRIVE_PWM_MAX;
        }
        PIN_LAUNCHER_ENABLE_LAT = 1;
        PWM_SetDutyCycle(PIN_LAUNCHER_FLYWHEEL_PWM, pwm);
    }
}

void LauncherTest_StopFlywheel(void)
{
    PIN_LAUNCHER_ENABLE_LAT = 0;
    PWM_SetDutyCycle(PIN_LAUNCHER_FLYWHEEL_PWM, 0);
}

void LauncherTest_BallServerHome(void)
{
    RC_SetPulseTime(BALL_SERVER_SERVO_RC, BALL_SERVER_HOME_US);
}

void LauncherTest_BallServerServePosition(void)
{
    RC_SetPulseTime(BALL_SERVER_SERVO_RC, BALL_SERVER_SERVE_US);
}

void LauncherTest_SetIrEmitter(uint8_t enabled)
{
    IrEmitterEnabled = enabled;
    PIN_IR_EMITTER_GATE_LAT = (enabled != FALSE);
}

uint8_t LauncherTest_IsIrEmitterEnabled(void)
{
    return IrEmitterEnabled;
}
