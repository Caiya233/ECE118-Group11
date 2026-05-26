# Final Project Pin Map

This map follows `PinMap_planned_v2_single_flywheel.h` and the code-facing
aliases in `PinMap.h`.

## PWM Outputs

| Function | ECE118 pin |
| --- | --- |
| Front left H-bridge `ENB` | `PWM_PORTY10` |
| Front right H-bridge `ENA` | `PWM_PORTY04` |
| Rear left H-bridge `ENA` | `PWM_PORTX11` |
| Rear right H-bridge `ENB` | `PWM_PORTY12` |
| DS3658 Input A, single flywheel/index wheel PWM | `PWM_PORTZ06` |

The launcher uses one flywheel/index-wheel PWM output plus one ball-server
servo. There is no separate indexing motor.

## Digital Outputs

| Function | Pin |
| --- | --- |
| Front left H-bridge `IN3` / direction 1 | `PORTX10` |
| Front left H-bridge `IN4` / direction 2 | `PORTX08` |
| Rear left H-bridge `IN1` / direction 1 | `PORTX03` |
| Rear left H-bridge `IN2` / direction 2 | `PORTX04` |
| Front right H-bridge `IN1` / direction 1 | `PORTX05` |
| Front right H-bridge `IN2` / direction 2 | `PORTX06` |
| Rear right H-bridge `IN3` / direction 1 | `PORTZ03` |
| Rear right H-bridge `IN4` / direction 2 | `PORTX12` |
| IR emitter gate | `PORTZ07` |

The drive code uses the same ENA/IN1/IN2/IN3/IN4/ENB signed-duty output layer as
`final_test.X`. For each wheel, direction 1 high and direction 2 low is the
software "forward" command. `X7` and `X9` are intentionally unused. `X12` is
rear-right `IN4`, so the optional launcher enable/kill line is unavailable in
this wiring.

The launcher flywheel/index wheel is driven as a low-side DS3658 load:
motor high side to fused `+12 V`, motor low side to DS3658 Output A, and
`PWM_PORTZ06` to DS3658 Input A. Close the DS3658 clamp jumper for the
inductive motor load.

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
| Rear left bumper | `PORTZ11` |
| Rear right bumper | `PORTZ04` |
| Start button | `PORTZ05` |

## Ball-Server Servo

`BALL_SERVER_SERVO_RC` is `RC_PORTY06`. Competition firing code serves one
ball by moving this servo to `BALL_SERVER_SERVE_US`, then returning it to
`BALL_SERVER_HOME_US` while the flywheel stays running.
