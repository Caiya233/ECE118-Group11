#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#include "RC_Servo.h"

/* Initial safe guesses; tune on hardware with the serial menu. */
#define DRIVE_PWM_MAX                  1000
#define DRIVE_PWM_MIN_EFFECTIVE        180
#define DRIVE_TEST_PWM                 220
#define DRIVE_TEST_MS                  500

#define DRIVE_FL_SIGN                  1
#define DRIVE_FR_SIGN                  1
#define DRIVE_RL_SIGN                  1
#define DRIVE_RR_SIGN                  1

#define TAPE_F_L_BIT                   0x01u
#define TAPE_F_LM_BIT                  0x02u
#define TAPE_F_M_BIT                   0x04u
#define TAPE_F_RM_BIT                  0x08u
#define TAPE_F_R_BIT                   0x10u
#define TAPE_R_LM_BIT                  0x01u
#define TAPE_R_RM_BIT                  0x02u

#define TAPE_ENTER_THRESH              650
#define TAPE_EXIT_THRESH               580
#define TAPE_FRONT_CROSS_MIN_COUNT     3
#define FRONT_WIDE_MIN_COUNT           3
#define EDGE_LOCK_DEBOUNCE_MS          100
#define EDGE_LOST_GRACE_MS             300
#define REAR_WIDE_CONFIRM_MS           100

#define BEACON_LOCK_THRESH             700
#define BEACON_CENTER_TOL              40

#define FLYWHEEL_TEST_PWM_LOW          250
#define FLYWHEEL_TEST_PWM_MED          450
#define FLYWHEEL_COMBAT_PWM            650
#define FLYWHEEL_SPINUP_MS             1200

#define BALL_SERVER_HOME_US            1300
#define BALL_SERVER_SERVE_US           1700
#define BALL_SERVER_SERVE_HOLD_MS      250
#define BALL_SERVER_RETURN_MS          250

#define REC_STRAFE_11IN_SPEED          300
#define REC_RIGHT_11IN_MS              900
#define NAV_ISZ_CREEP_SPEED            160
#define NAV_ISZ_CREEP_MS               900

#define TEST_SERVO_ENABLED             0
#define TEST_SERVO_PIN                 RC_PORTZ08
#define TEST_SERVO_MIN_US              1100
#define TEST_SERVO_CENTER_US           1500
#define TEST_SERVO_MAX_US              1900
#define TEST_SERVO_SWEEP_STEP_US       25
#define TEST_SERVO_SWEEP_STEP_MS       35

#define SENSOR_REPEAT_MS               350
#define BATTERY_ADC_MV_PER_COUNT       32

#define BUMPER_ACTIVE_LEVEL            1

#endif
