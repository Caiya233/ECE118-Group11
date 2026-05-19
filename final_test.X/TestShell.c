#include "BOARD.h"
#include "BatteryTest.h"
#include "DebugPrint.h"
#include "DriveTest.h"
#include "LauncherTest.h"
#include "PinMap.h"
#include "SensorTest.h"
#include "ServoTest.h"
#include "TestConfig.h"
#include "TestShell.h"
#include "serial.h"
#include "timers.h"
#include <stdio.h>

static uint8_t QuietMode;
static uint8_t VerboseMode;

static void TimedDriveMs(const char *name, int forward, int strafe, int yaw,
                         unsigned int durationMs);

static void EmergencyStop(void)
{
    DriveTest_Stop();
    LauncherTest_StopAll();
    printf("\r\nEMERGENCY STOP: all actuators off\r\n");
}

static void PrintMenu(void)
{
    printf("\r\n=== final_test.X hardware menu ===\r\n");
    printf("? menu | s emergency stop | p pin/config | v battery | q quiet | d verbose\r\n");
    printf("Drive: f forward | b back | l strafe left | r strafe right | y yaw left | u yaw right | x stop\r\n");
    printf("Cal: ] right-11 strafe | [ left-11 strafe | C ISZ creep\r\n");
    printf("Launcher: F flywheel low | G flywheel med | H flywheel stop | O server home | P server pulse | W=P alias | E IR toggle\r\n");
    printf("Servo: 1 min | 2 center | 3 max | 4 slow sweep\r\n");
    printf("Sensors: T tape+features repeat | B beacon once | M bumpers | A all once | R all repeat\r\n");
    printf("All motion commands are low-power, timed, and auto-stop.\r\n");
}

static void PrintConfig(void)
{
    printf("\r\nDrive hardware: DRV8814 PHASE/ENABLE, PWM enables FL=%u FR=%u RL=%u RR=%u\r\n",
           PIN_DRIVE_FRONT_LEFT_PWM, PIN_DRIVE_FRONT_RIGHT_PWM,
           PIN_DRIVE_REAR_LEFT_PWM, PIN_DRIVE_REAR_RIGHT_PWM);
    printf("Drive PHASE pins: FL=X03 RL=X04 FR=X05 RR=X06\r\n");
    printf("Drive signs: FL=%d FR=%d RL=%d RR=%d test_pwm=%u test_ms=%u\r\n",
           DRIVE_FL_SIGN, DRIVE_FR_SIGN, DRIVE_RL_SIGN, DRIVE_RR_SIGN,
           DRIVE_TEST_PWM, DRIVE_TEST_MS);
    printf("Tape thresholds: enter=%u exit=%u cross_min=%u\r\n",
           TAPE_ENTER_THRESH, TAPE_EXIT_THRESH, TAPE_FRONT_CROSS_MIN_COUNT);
    printf("Beacon: lock_thresh=%u center_tol=%u\r\n",
           BEACON_LOCK_THRESH, BEACON_CENTER_TOL);
    printf("Launcher: DS3658 InputA PWM=%u fly_low=%u fly_med=%u combat=%u server_home=%u server_serve=%u hold_ms=%u return_ms=%u\r\n",
           PIN_LAUNCHER_FLYWHEEL_PWM,
           FLYWHEEL_TEST_PWM_LOW, FLYWHEEL_TEST_PWM_MED,
           FLYWHEEL_COMBAT_PWM, BALL_SERVER_HOME_US, BALL_SERVER_SERVE_US,
           BALL_SERVER_SERVE_HOLD_MS, BALL_SERVER_RETURN_MS);
    printf("Servo enabled=%u pin=0x%04x min=%u center=%u max=%u\r\n",
           TEST_SERVO_ENABLED, TEST_SERVO_PIN, TEST_SERVO_MIN_US,
           TEST_SERVO_CENTER_US, TEST_SERVO_MAX_US);
}

static uint8_t HandleStopKey(char key)
{
    if (key == 's') {
        EmergencyStop();
        return TRUE;
    }
    if (key == 'x') {
        DriveTest_Stop();
        printf("\r\nDrive stop\r\n");
        return TRUE;
    }
    return FALSE;
}

static void WaitWithStop(unsigned int durationMs)
{
    unsigned int start = TIMERS_GetTime();

    while ((TIMERS_GetTime() - start) < durationMs) {
        char key = GetChar();
        if (HandleStopKey(key) != FALSE) {
            return;
        }
    }
}

static uint8_t WaitRepeatInterval(void)
{
    unsigned int start = TIMERS_GetTime();

    while ((TIMERS_GetTime() - start) < SENSOR_REPEAT_MS) {
        char key = GetChar();
        if (key != 0) {
            HandleStopKey(key);
            return TRUE;
        }
    }

    return FALSE;
}

static void TimedDrive(const char *name, int forward, int strafe, int yaw)
{
    TimedDriveMs(name, forward, strafe, yaw, DRIVE_TEST_MS);
}

static void TimedDriveMs(const char *name, int forward, int strafe, int yaw,
                         unsigned int durationMs)
{
    int fl = (forward + strafe + yaw) * DRIVE_FL_SIGN;
    int fr = (forward - strafe - yaw) * DRIVE_FR_SIGN;
    int rl = (forward - strafe + yaw) * DRIVE_RL_SIGN;
    int rr = (forward + strafe - yaw) * DRIVE_RR_SIGN;

    printf("\r\nDrive %s: f=%d s=%d y=%d raw fl=%d fr=%d rl=%d rr=%d ms=%u\r\n",
           name, forward, strafe, yaw, fl, fr, rl, rr, durationMs);
    DriveTest_SetBodyVelocity(forward, strafe, yaw);
    WaitWithStop(durationMs);
    DriveTest_Stop();
    printf("Drive %s complete: stopped\r\n", name);
}

static void ServeOneBallTest(void)
{
    printf("\r\nBall-server serve pulse: serve_us=%u hold_ms=%u home_us=%u return_ms=%u\r\n",
           BALL_SERVER_SERVE_US, BALL_SERVER_SERVE_HOLD_MS,
           BALL_SERVER_HOME_US, BALL_SERVER_RETURN_MS);
    LauncherTest_BallServerServePosition();
    WaitWithStop(BALL_SERVER_SERVE_HOLD_MS);
    LauncherTest_BallServerHome();
    WaitWithStop(BALL_SERVER_RETURN_MS);
    printf("Ball-server pulse complete: returned home_us=%u\r\n",
           BALL_SERVER_HOME_US);
}

static void SweepServo(void)
{
    unsigned int pulse;

    if (ServoTest_IsAvailable() == FALSE) {
        ServoTest_SetPulse(TEST_SERVO_CENTER_US);
        return;
    }

    for (pulse = TEST_SERVO_MIN_US; pulse <= TEST_SERVO_MAX_US;
         pulse += TEST_SERVO_SWEEP_STEP_US) {
        printf("\r\nServo pulse=%u us\r\n", pulse);
        ServoTest_SetPulse(pulse);
        WaitWithStop(TEST_SERVO_SWEEP_STEP_MS);
    }
    for (pulse = TEST_SERVO_MAX_US; pulse >= TEST_SERVO_MIN_US;
         pulse -= TEST_SERVO_SWEEP_STEP_US) {
        printf("\r\nServo pulse=%u us\r\n", pulse);
        ServoTest_SetPulse(pulse);
        WaitWithStop(TEST_SERVO_SWEEP_STEP_MS);
        if (pulse < (TEST_SERVO_MIN_US + TEST_SERVO_SWEEP_STEP_US)) {
            break;
        }
    }
}

static void RepeatTape(void)
{
    printf("\r\nTape repeat: press any key to stop, s for emergency stop\r\n");
    while (1) {
        char key = GetChar();
        if (key != 0) {
            HandleStopKey(key);
            break;
        }
        SensorTest_PrintTape();
        if (WaitRepeatInterval() != FALSE) {
            break;
        }
    }
}

static void RepeatAll(void)
{
    printf("\r\nAll sensor repeat: press any key to stop, s for emergency stop\r\n");
    while (1) {
        char key = GetChar();
        if (key != 0) {
            HandleStopKey(key);
            break;
        }
        SensorTest_PrintSnapshot();
        if (WaitRepeatInterval() != FALSE) {
            break;
        }
    }
}

static void HandleCommand(char key)
{
    switch (key) {
    case 0:
        break;
    case '?':
        PrintMenu();
        break;
    case 's':
        EmergencyStop();
        break;
    case 'p':
        PrintConfig();
        break;
    case 'v':
        BatteryTest_Print();
        break;
    case 'q':
        QuietMode = (QuietMode == FALSE);
        DebugPrint_SetQuiet(QuietMode);
        printf("\r\nquiet=%u\r\n", QuietMode);
        break;
    case 'd':
        VerboseMode = (VerboseMode == FALSE);
        DebugPrint_SetVerbose(VerboseMode);
        printf("\r\nverbose=%u\r\n", VerboseMode);
        break;
    case 'f':
        TimedDrive("forward", DRIVE_TEST_PWM, 0, 0);
        break;
    case 'b':
        TimedDrive("backward", -DRIVE_TEST_PWM, 0, 0);
        break;
    case 'l':
        TimedDrive("strafe_left", 0, -DRIVE_TEST_PWM, 0);
        break;
    case 'r':
        TimedDrive("strafe_right", 0, DRIVE_TEST_PWM, 0);
        break;
    case 'y':
        TimedDrive("yaw_left", 0, 0, -DRIVE_TEST_PWM);
        break;
    case 'u':
        TimedDrive("yaw_right", 0, 0, DRIVE_TEST_PWM);
        break;
    case 'x':
        DriveTest_Stop();
        printf("\r\nDrive stopped\r\n");
        break;
    case ']':
        TimedDriveMs("right_11_cal", 0, REC_STRAFE_11IN_SPEED, 0,
                     REC_RIGHT_11IN_MS);
        break;
    case '[':
        TimedDriveMs("left_11_cal", 0, -REC_STRAFE_11IN_SPEED, 0,
                     REC_RIGHT_11IN_MS);
        break;
    case 'C':
        TimedDriveMs("isz_creep", NAV_ISZ_CREEP_SPEED, 0, 0,
                     NAV_ISZ_CREEP_MS);
        break;
    case 'F':
        LauncherTest_SetFlywheel(FLYWHEEL_TEST_PWM_LOW);
        printf("\r\nSingle flywheel low test pwm=%u; driver enabled\r\n",
               FLYWHEEL_TEST_PWM_LOW);
        break;
    case 'G':
        LauncherTest_SetFlywheel(FLYWHEEL_TEST_PWM_MED);
        printf("\r\nSingle flywheel medium test pwm=%u; driver enabled\r\n",
               FLYWHEEL_TEST_PWM_MED);
        break;
    case 'H':
        LauncherTest_StopFlywheel();
        printf("\r\nSingle flywheel stopped\r\n");
        break;
    case 'O':
        LauncherTest_BallServerHome();
        printf("\r\nBall-server home/closed pulse_us=%u\r\n",
               BALL_SERVER_HOME_US);
        break;
    case 'P':
        ServeOneBallTest();
        break;
    case 'W':
        printf("\r\nCompatibility alias: W runs P ball-server pulse.\r\n");
        ServeOneBallTest();
        break;
    case 'E':
        LauncherTest_SetIrEmitter(!LauncherTest_IsIrEmitterEnabled());
        printf("\r\nIR emitter enabled=%u\r\n",
               LauncherTest_IsIrEmitterEnabled());
        break;
    case '1':
        printf("\r\nServo min pulse=%u us\r\n", TEST_SERVO_MIN_US);
        ServoTest_SetPulse(TEST_SERVO_MIN_US);
        break;
    case '2':
        printf("\r\nServo center pulse=%u us\r\n", TEST_SERVO_CENTER_US);
        ServoTest_SetPulse(TEST_SERVO_CENTER_US);
        break;
    case '3':
        printf("\r\nServo max pulse=%u us\r\n", TEST_SERVO_MAX_US);
        ServoTest_SetPulse(TEST_SERVO_MAX_US);
        break;
    case '4':
        SweepServo();
        break;
    case 'T':
        RepeatTape();
        break;
    case 'B':
        SensorTest_PrintBeacon();
        break;
    case 'M':
        SensorTest_PrintBumpers();
        break;
    case 'A':
        SensorTest_PrintSnapshot();
        break;
    case 'R':
        RepeatAll();
        break;
    default:
        printf("\r\nUnknown command '%c'. Press ? for menu.\r\n", key);
        break;
    }
}

void TestShell_Init(void)
{
    QuietMode = FALSE;
    VerboseMode = FALSE;
    TIMERS_Init();
    DebugPrint_Init();
    DriveTest_Init();
    LauncherTest_Init();
    ServoTest_Init();
    SensorTest_Init();
    EmergencyStop();
    PrintMenu();
}

void TestShell_Run(void)
{
    while (1) {
        HandleCommand(GetChar());
    }
}
