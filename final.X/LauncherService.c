#include "BOARD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "LauncherService.h"
#include "PinMap.h"
#include "ProjectConfig.h"
#include "RC_Servo.h"
#include "TopHSM.h"
#include "pwm.h"

static uint8_t MyPriority;
static uint8_t BallServerReturning;

static void PostTopEvent(ES_EventTyp_t type)
{
    ES_Event event;
    event.EventType = type;
    event.EventParam = 0;
    PostTopHSM(event);
}

static void InitLauncherPins(void)
{
#if LAUNCHER_ENABLE_AVAILABLE
    PIN_LAUNCHER_ENABLE_TRIS = 0;
#endif
    PIN_IR_EMITTER_GATE_TRIS = 0;

#if LAUNCHER_ENABLE_AVAILABLE
    PIN_LAUNCHER_ENABLE_LAT = 0;
#endif
    PIN_IR_EMITTER_GATE_LAT = 0;
}

static uint16_t ClampFlywheelDuty(uint16_t duty)
{
    if (duty > DRIVE_PWM_MAX) {
        return DRIVE_PWM_MAX;
    }
    return duty;
}

uint8_t InitLauncherService(uint8_t priority)
{
    ES_Event event;

    MyPriority = priority;
    BallServerReturning = FALSE;
    InitLauncherPins();
    PWM_AddPins(PIN_LAUNCHER_FLYWHEEL_PWM);
    PWM_SetDutyCycle(PIN_LAUNCHER_FLYWHEEL_PWM, 0);
    RC_Init();
    RC_AddPins(BALL_SERVER_SERVO_RC);
    Launcher_BallServerHome();

    event.EventType = ES_INIT;
    event.EventParam = 0;
    return ES_PostToService(MyPriority, event);
}

uint8_t PostLauncherService(ES_Event event)
{
    return ES_PostToService(MyPriority, event);
}

ES_Event RunLauncherService(ES_Event event)
{
    ES_Event returnEvent;
    returnEvent.EventType = ES_NO_EVENT;
    returnEvent.EventParam = 0;

    if ((event.EventType == ES_TIMEOUT) &&
        (event.EventParam == FLYWHEEL_TIMER)) {
        PostTopEvent(EV_FLYWHEEL_READY);
    } else if ((event.EventType == ES_TIMEOUT) &&
               (event.EventParam == BALL_SERVER_TIMER)) {
        if (BallServerReturning == FALSE) {
            BallServerReturning = TRUE;
            Launcher_BallServerHome();
            ES_Timer_InitTimer(BALL_SERVER_TIMER, BALL_SERVER_RETURN_MS);
        } else {
            BallServerReturning = FALSE;
            PostTopEvent(EV_BALL_SERVER_DONE);
            PostTopEvent(EV_BALL_FIRED);
        }
    }

    return returnEvent;
}

void Launcher_SetFlywheelPWM(uint16_t duty)
{
    duty = ClampFlywheelDuty(duty);
#if LAUNCHER_ENABLE_AVAILABLE
    PIN_LAUNCHER_ENABLE_LAT = (duty != 0u);
#endif
    PWM_SetDutyCycle(PIN_LAUNCHER_FLYWHEEL_PWM, duty);
}

void Launcher_StartFlywheel(uint16_t duty)
{
    Launcher_SetFlywheelPWM(duty);
    ES_Timer_InitTimer(FLYWHEEL_TIMER, FLYWHEEL_SPINUP_MS);
}

void Launcher_StopFlywheel(void)
{
    Launcher_SetFlywheelPWM(0);
}

void Launcher_BallServerHome(void)
{
    RC_SetPulseTime(BALL_SERVER_SERVO_RC, BALL_SERVER_HOME_US);
}

void Launcher_BallServerServePosition(void)
{
    RC_SetPulseTime(BALL_SERVER_SERVO_RC, BALL_SERVER_SERVE_US);
}

void Launcher_ServeOneBall(void)
{
    BallServerReturning = FALSE;
    Launcher_BallServerServePosition();
    ES_Timer_InitTimer(BALL_SERVER_TIMER, BALL_SERVER_SERVE_HOLD_MS);
}

void Launcher_Stop(void)
{
    BallServerReturning = FALSE;
#if LAUNCHER_ENABLE_AVAILABLE
    PIN_LAUNCHER_ENABLE_LAT = 0;
#endif
    PIN_IR_EMITTER_GATE_LAT = 0;
    PWM_SetDutyCycle(PIN_LAUNCHER_FLYWHEEL_PWM, 0);
    Launcher_BallServerHome();
}
