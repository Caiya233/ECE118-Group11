# Final Project Pin Map

This map follows `PinMap_planned_v2_single_flywheel.h` and the code-facing
aliases in `PinMap.h`.

## PWM Outputs

| Function | ECE118 pin |
| --- | --- |
| Front left drive PWM | `PWM_PORTX11` |
| Front right drive PWM | `PWM_PORTY04` |
| Rear left drive PWM | `PWM_PORTY10` |
| Rear right drive PWM | `PWM_PORTY12` |
| Single launcher flywheel PWM | `PWM_PORTZ06` |

The launcher uses one flywheel/index-wheel PWM output plus one ball-server
servo. There is no separate indexing motor.

## Digital Outputs

| Function | Pin |
| --- | --- |
| Front left H-bridge IN1 / IN2 | `PORTX03` / `PORTX04` |
| Front right H-bridge IN1 / IN2 | `PORTX05` / `PORTX06` |
| Rear left H-bridge IN1 / IN2 | `PORTX07` / `PORTX08` |
| Rear right H-bridge IN1 / IN2 | `PORTX09` / `PORTX10` |
| Launcher enable / kill | `PORTX12` |
| IR emitter gate | `PORTZ07` |

## Analog Inputs

| Function | ADC pin |
| --- | --- |
| Tape F_L | `AD_PORTV3` |
| Tape F_LM | `AD_PORTV4` |
| Tape F_M | `AD_PORTV5` |
| Tape F_RM | `AD_PORTV6` |
| Tape F_R | `AD_PORTV7` |
| Tape R_LM | `AD_PORTV8` |
| Tape R_RM | `AD_PORTW3` |
| Beacon left | `AD_PORTW4` |
| Beacon center | `AD_PORTW5` |
| Beacon right | `AD_PORTW6` |
| Battery sense | `AD_PORTW7` |
| Coil diagnostic only | `AD_PORTW8` |

The coil input is diagnostic only and must not be used as obstacle-position evidence.

## Digital Inputs

| Function | Pin |
| --- | --- |
| Front left bumper | `PORTY03` |
| Front right bumper | `PORTY05` |
| Left front bumper | `PORTY07` |
| Left rear bumper | `PORTY08` |
| Right front bumper | `PORTY09` |
| Right rear bumper | `PORTY11` |
| Rear left bumper | `PORTZ03` |
| Rear right bumper | `PORTZ04` |
| Start button | `PORTZ05` |

## Ball-Server Servo

`BALL_SERVER_SERVO_RC` is `RC_PORTY06`. Competition firing code serves one
ball by moving this servo to `BALL_SERVER_SERVE_US`, then returning it to
`BALL_SERVER_HOME_US` while the flywheel stays running.
