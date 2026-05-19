#ifndef PINMAP_PLANNED_V2_SINGLE_FLYWHEEL_H
#define PINMAP_PLANNED_V2_SINGLE_FLYWHEEL_H

#include "BOARD.h"
#include "IO_Ports.h"
#include "AD.h"
#include "pwm.h"
#include "RC_Servo.h"

// ============================================================
// Drive hardware: two DRV8814 dual H-bridge boards in PHASE/ENABLE mode.
// PWM outputs connect to AENBL/BENBL. PORTX outputs connect to APHASE/BPHASE.
// If the physical board exposes IN1/IN2 instead, update PinMap.h before use.
// ============================================================
#define DRIVE_FL_ENABLE_PWM       PWM_PORTX11
#define DRIVE_FR_ENABLE_PWM       PWM_PORTY04
#define DRIVE_RL_ENABLE_PWM       PWM_PORTY10
#define DRIVE_RR_ENABLE_PWM       PWM_PORTY12

#define DRIVE_FL_PWM              DRIVE_FL_ENABLE_PWM
#define DRIVE_FR_PWM              DRIVE_FR_ENABLE_PWM
#define DRIVE_RL_PWM              DRIVE_RL_ENABLE_PWM
#define DRIVE_RR_PWM              DRIVE_RR_ENABLE_PWM

#define DRIVE_FL_PHASE_PORT       PORTX
#define DRIVE_FL_PHASE_PIN        PIN3
#define DRIVE_RL_PHASE_PORT       PORTX
#define DRIVE_RL_PHASE_PIN        PIN4
#define DRIVE_FR_PHASE_PORT       PORTX
#define DRIVE_FR_PHASE_PIN        PIN5
#define DRIVE_RR_PHASE_PORT       PORTX
#define DRIVE_RR_PHASE_PIN        PIN6

#define PINMAP_DRIVE_PHASE_PORTX  (PIN3 | PIN4 | PIN5 | PIN6)
#define PINMAP_DRIVE_PWM_ALL      (DRIVE_FL_PWM | DRIVE_FR_PWM | DRIVE_RL_PWM | DRIVE_RR_PWM)

// ============================================================
// Launcher: DS3658 low-side flywheel/index wheel, ball-server servo, emitter
// ============================================================
// The current mechanism has ONE flywheel/index wheel. It provides launch speed
// and the ball-indexing/transport function. There is no separate indexing motor.
#define LAUNCHER_FLYWHEEL_PWM     PWM_PORTZ06
#define LAUNCHER_ENABLE_PORT      PORTX
#define LAUNCHER_ENABLE_PIN       PIN12    // optional external enable/kill, default OFF

// Bottom ping-pong ball server servo. It releases/presents one ball, then returns home.
#define BALL_SERVER_SERVO_RC      RC_PORTY06

#define IR_EMITTER_PORT           PORTZ
#define IR_EMITTER_PIN            PIN7

// ============================================================
// Tape sensors: analog inputs
// ============================================================
#define TAPE_F_L_AD               AD_PORTV3
#define TAPE_F_LM_AD              AD_PORTV4
#define TAPE_F_M_AD               AD_PORTV5
#define TAPE_F_RM_AD              AD_PORTV6
#define TAPE_F_R_AD               AD_PORTV7
#define TAPE_R_LM_AD              AD_PORTV8
#define TAPE_R_RM_AD              AD_PORTW3

#define PINMAP_TAPE_AD_ALL        (TAPE_F_L_AD | TAPE_F_LM_AD | TAPE_F_M_AD | \
                                   TAPE_F_RM_AD | TAPE_F_R_AD | TAPE_R_LM_AD | \
                                   TAPE_R_RM_AD)

// Tape feature bit positions for software masks
#define TAPE_F_L_BIT              (1 << 0)
#define TAPE_F_LM_BIT             (1 << 1)
#define TAPE_F_M_BIT              (1 << 2)
#define TAPE_F_RM_BIT             (1 << 3)
#define TAPE_F_R_BIT              (1 << 4)
#define TAPE_R_LM_BIT             (1 << 0)
#define TAPE_R_RM_BIT             (1 << 1)

// ============================================================
// Beacon detectors: analog inputs
// ============================================================
#define BEACON_LEFT_AD            AD_PORTW4
#define BEACON_CENTER_AD          AD_PORTW5
#define BEACON_RIGHT_AD           AD_PORTW6
#define PINMAP_BEACON_AD_ALL      (BEACON_LEFT_AD | BEACON_CENTER_AD | BEACON_RIGHT_AD)

// ============================================================
// Battery / optional diagnostic
// ============================================================
#define BATTERY_AD                AD_PORTW7
#define COIL_RAW_AD               AD_PORTW8  // optional diagnostic only, not obstacle logic

#define PINMAP_ALL_ANALOG_INPUTS  (PINMAP_TAPE_AD_ALL | PINMAP_BEACON_AD_ALL | \
                                   BATTERY_AD | COIL_RAW_AD)

// ============================================================
// Bumpers and start button: digital inputs
// ============================================================
#define BUMPER_FRONT_LEFT_PORT    PORTY
#define BUMPER_FRONT_LEFT_PIN     PIN3
#define BUMPER_FRONT_RIGHT_PORT   PORTY
#define BUMPER_FRONT_RIGHT_PIN    PIN5
#define BUMPER_LEFT_FRONT_PORT    PORTY
#define BUMPER_LEFT_FRONT_PIN     PIN7
#define BUMPER_LEFT_REAR_PORT     PORTY
#define BUMPER_LEFT_REAR_PIN      PIN8
#define BUMPER_RIGHT_FRONT_PORT   PORTY
#define BUMPER_RIGHT_FRONT_PIN    PIN9
#define BUMPER_RIGHT_REAR_PORT    PORTY
#define BUMPER_RIGHT_REAR_PIN     PIN11
#define BUMPER_REAR_LEFT_PORT     PORTZ
#define BUMPER_REAR_LEFT_PIN      PIN3
#define BUMPER_REAR_RIGHT_PORT    PORTZ
#define BUMPER_REAR_RIGHT_PIN     PIN4

#define START_BUTTON_PORT         PORTZ
#define START_BUTTON_PIN          PIN5

#define PINMAP_BUMPER_PORTY_INPUTS (PIN3 | PIN5 | PIN7 | PIN8 | PIN9 | PIN11)
#define PINMAP_BUMPER_PORTZ_INPUTS (PIN3 | PIN4)
#define PINMAP_START_PORTZ_INPUTS  (PIN5)

// ============================================================
// Aggregate outputs
// ============================================================
#define PINMAP_PORTX_OUTPUTS      (PINMAP_DRIVE_PHASE_PORTX | LAUNCHER_ENABLE_PIN)
#define PINMAP_PORTZ_OUTPUTS      (IR_EMITTER_PIN)
#define PINMAP_ALL_PWM_OUTPUTS    (PINMAP_DRIVE_PWM_ALL | LAUNCHER_FLYWHEEL_PWM)

#endif // PINMAP_PLANNED_V2_SINGLE_FLYWHEEL_H
