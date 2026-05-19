# Final Test Pin Map

This test project mirrors `final.X/PinMap.h` and
`PinMap_planned_v2_single_flywheel.h` so calibration exercises the same planned
wiring as the competition image.

## PWM Outputs

| Function | ECE118 pin |
| --- | --- |
| Front left DRV8814 ENABLE PWM | `PWM_PORTX11` |
| Front right DRV8814 ENABLE PWM | `PWM_PORTY04` |
| Rear left DRV8814 ENABLE PWM | `PWM_PORTY10` |
| Rear right DRV8814 ENABLE PWM | `PWM_PORTY12` |
| DS3658 Input A, single flywheel/index wheel PWM | `PWM_PORTZ06` |

## Digital Outputs

| Function | Pin |
| --- | --- |
| Front left DRV8814 PHASE | `PORTX03` |
| Rear left DRV8814 PHASE | `PORTX04` |
| Front right DRV8814 PHASE | `PORTX05` |
| Rear right DRV8814 PHASE | `PORTX06` |
| Optional launcher enable / kill | `PORTX12` |
| IR emitter gate | `PORTZ07` |

The drive tests assume the DRV8814 boards expose `AENBL`, `APHASE`, `BENBL`,
and `BPHASE`. If the physical boards expose `IN1`/`IN2` instead, stop and
update `PinMap.h` plus `DriveTest.c` before running motors.

The flywheel/index wheel test drives a low-side DS3658 load:
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

## Calibration Notes

The `O` command homes the ball-server servo. The `P` command performs one
serve pulse and returns home; `W` is only a documented compatibility alias for
`P`. The `]` and `[` commands run the calibrated 11-inch strafe timing.
