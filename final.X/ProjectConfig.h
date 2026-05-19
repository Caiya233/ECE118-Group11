#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

/* Main drive settings. */
#define DRIVE_PWM_MAX              1000
#define DRIVE_PWM_MIN_EFFECTIVE    180
#define DRIVE_TEST_PWM             220
#define DRIVE_TEST_MS              500

/* Initial motor polarity guesses; tune with final_test.X. */
#define DRIVE_FL_SIGN              1
#define DRIVE_FR_SIGN              1
#define DRIVE_RL_SIGN              1
#define DRIVE_RR_SIGN              1

/* Tape masks. */
#define TAPE_F_L_BIT               0x01u
#define TAPE_F_LM_BIT              0x02u
#define TAPE_F_M_BIT               0x04u
#define TAPE_F_RM_BIT              0x08u
#define TAPE_F_R_BIT               0x10u
#define TAPE_R_LM_BIT              0x01u
#define TAPE_R_RM_BIT              0x02u

/* Tape sensor hysteresis and feature thresholds. */
#define TAPE_ENTER_THRESH          650
#define TAPE_EXIT_THRESH           580
#define TAPE_FRONT_CROSS_MIN_COUNT 3
#define FRONT_WIDE_MIN_COUNT              3
#define FRONT_ALL_BLACK_PANIC_MS          250
#define EDGE_LOCK_DEBOUNCE_MS             100
#define EDGE_LOST_GRACE_MS                300
#define REAR_WIDE_CONFIRM_MS              100

/* Beacon settings. */
#define BEACON_LOCK_THRESH         700
#define BEACON_LOST_MS             300
#define BEACON_CENTER_TOL          40
#define BEACON_YAW_GAIN_NUM        1
#define BEACON_YAW_GAIN_DEN        3

/* Legal path navigation settings. Positive strafe is robot-right. */
#define DRIVE_STRAFE_POSITIVE_IS_RIGHT 1
#define NAV_YAW_SCAN_SPEED         220
#define NAV_YAW_CORRECT_MAX        260
#define NAV_BEACON_SCAN_MS         1500
#define NAV_BEACON_SCAN_MAX_RETRIES 4

#define STRAFE_LEFT_SPEED          (-220)
#define STRAFE_RIGHT_SPEED         220
#define STRAFE_INWARD_FROM_LEFT    STRAFE_RIGHT_SPEED
#define STRAFE_INWARD_FROM_RIGHT   STRAFE_LEFT_SPEED

#define NAV_EXIT_START_MS          700
#define NAV_EXIT_FORWARD_SPEED     220

#define NAV_EDGE_FORWARD           320
#define NAV_EDGE_FORWARD_SLOW      200
#define NAV_STRAFE_LEFT_SEARCH     STRAFE_LEFT_SPEED
#define NAV_STRAFE_RIGHT_SEARCH    STRAFE_RIGHT_SPEED
#define NAV_STRAFE_LEFT_SAFE       STRAFE_LEFT_SPEED
#define NAV_STRAFE_RIGHT_SAFE      STRAFE_RIGHT_SPEED
#define NAV_EDGE_SEEK_TIMEOUT_MS   1800
#define NAV_EDGE_SEARCH_TIMEOUT_MS 900

#define REC_BACKUP_MS              400
#define REC_BACKUP_SPEED           (-240)
#define REC_RIGHT_11IN_MS          900
#define REC_LEFT_REACQUIRE_TIMEOUT_MS 2200
#define REC_STRAFE_11IN_SPEED      300
#define REC_RECENTER_TIMEOUT_MS    650

#define CROSS_TO_RIGHT_SUPPRESS_MS 2000
#define CROSS_TO_RIGHT_MAX_MS      5500
#define CROSS_TO_RIGHT_STRAFE_SPEED 320
#define CROSS_TO_RIGHT_FORWARD_BIAS 140

#define NAV_MIN_ISZ_TIME_MS        2500
#define NAV_ISZ_CREEP_MS           900
#define NAV_ISZ_CREEP_SPEED        160
#define ISZ_EXTRA_CREEP_MS         350
#define NAV_ISZ_SEARCH_MS          3000
#define NAV_INTERIOR_FORWARD_TIMEOUT_MS 4500
#define NAV_MAX_TOTAL_TIME_MS      15000

/* Other sensor settings. */
#define TRACKWIRE_THRESH           500
#define BUMPER_DEBOUNCE_TICKS      5

/* Launcher and ball-server timing. */
#define FLYWHEEL_TEST_PWM_LOW      250
#define FLYWHEEL_TEST_PWM_MED      450
#define FLYWHEEL_COMBAT_PWM        650
#define FLYWHEEL_SPINUP_MS         1200

#define BALL_SERVER_HOME_US        1300
#define BALL_SERVER_SERVE_US       1700
#define BALL_SERVER_SERVE_HOLD_MS  250
#define BALL_SERVER_RETURN_MS      250

/* Generic top-level recovery timing. */
#define RECOVERY_BACKUP_MS         400
#define RECOVERY_TURN_MS           550
#define RECOVERY_STRAFE_MS         350
#define RECOVERY_SPEED             300
#define OBSTACLE_CLEAR_MAX_MS      5000

/* Polling rates for early testing. */
#define TAPE_POLL_MS               25
#define TRACKWIRE_POLL_MS          25
#define BUMPER_POLL_MS             10
#define BEACON_POLL_MS             25
#define BATTERY_POLL_MS            500

/* This number is a safe first guess and should be tested. */
#define LOW_BATTERY_THRESH         350
#define BATTERY_LOW_THRESH         LOW_BATTERY_THRESH
#define BATTERY_CRITICAL_THRESH    300

/* Bumpers are assumed active high for now. */
#define BUMPER_ACTIVE_LEVEL        1

/* Leave LED bank debug off when motors are wired to the shield pins. */
#define DEBUG_USE_LED_BANKS        0

#endif
