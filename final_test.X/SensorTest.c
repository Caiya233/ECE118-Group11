#include "BOARD.h"
#include "AD.h"
#include "PinMap.h"
#include "SensorTest.h"
#include "TestConfig.h"
#include "TapeFeature.h"
#include "timers.h"
#include <stdio.h>

#define BUMP_FRONT_L 0x01
#define BUMP_FRONT_R 0x02
#define BUMP_LEFT_F  0x04
#define BUMP_LEFT_R  0x08
#define BUMP_RIGHT_F 0x10
#define BUMP_RIGHT_R 0x20
#define BUMP_REAR_L  0x40
#define BUMP_REAR_R  0x80

static uint8_t Active(uint8_t pinValue)
{
    return (pinValue == BUMPER_ACTIVE_LEVEL);
}

static void InitBumperPins(void)
{
    PIN_BUMPER_FRONT_L_TRIS = 1;
    PIN_BUMPER_FRONT_R_TRIS = 1;
    PIN_BUMPER_LEFT_F_TRIS = 1;
    PIN_BUMPER_LEFT_R_TRIS = 1;
    PIN_BUMPER_RIGHT_F_TRIS = 1;
    PIN_BUMPER_RIGHT_R_TRIS = 1;
    PIN_BUMPER_REAR_L_TRIS = 1;
    PIN_BUMPER_REAR_R_TRIS = 1;
}

static uint8_t ReadBumperMask(void)
{
    uint8_t mask = 0;

    if (Active(PIN_BUMPER_FRONT_L_BIT)) {
        mask |= BUMP_FRONT_L;
    }
    if (Active(PIN_BUMPER_FRONT_R_BIT)) {
        mask |= BUMP_FRONT_R;
    }
    if (Active(PIN_BUMPER_LEFT_F_BIT)) {
        mask |= BUMP_LEFT_F;
    }
    if (Active(PIN_BUMPER_LEFT_R_BIT)) {
        mask |= BUMP_LEFT_R;
    }
    if (Active(PIN_BUMPER_RIGHT_F_BIT)) {
        mask |= BUMP_RIGHT_F;
    }
    if (Active(PIN_BUMPER_RIGHT_R_BIT)) {
        mask |= BUMP_RIGHT_R;
    }
    if (Active(PIN_BUMPER_REAR_L_BIT)) {
        mask |= BUMP_REAR_L;
    }
    if (Active(PIN_BUMPER_REAR_R_BIT)) {
        mask |= BUMP_REAR_R;
    }

    return mask;
}

static uint8_t TapeActive(unsigned int value)
{
    return (value > TAPE_ENTER_THRESH);
}

static TapeFeatureEstimator_t FeatureEstimator;
static uint8_t RearWideActive;
static unsigned int RearWideSinceMs;

static unsigned int RearWideStableMs(const TapeFeature_t *feature,
                                     unsigned int nowMs)
{
    if (feature->rearWideBlack) {
        if (RearWideActive == FALSE) {
            RearWideActive = TRUE;
            RearWideSinceMs = nowMs;
        }
        return (unsigned int)(nowMs - RearWideSinceMs);
    }

    RearWideActive = FALSE;
    RearWideSinceMs = nowMs;
    return 0;
}

void SensorTest_Init(void)
{
    InitBumperPins();
    TapeFeature_Reset(&FeatureEstimator);
    RearWideActive = FALSE;
    RearWideSinceMs = 0;
    AD_AddPins(PIN_TAPE_ADC_MASK | PIN_BEACON_ADC_MASK |
               PIN_TRACKWIRE_ADC_MASK | PIN_BATTERY_ADC);
}

SensorSnapshot_t SensorTest_ReadSnapshot(void)
{
    SensorSnapshot_t s;
    unsigned int best;

    s.timeMs = TIMERS_GetTime();
    s.tapeFrontL = AD_ReadADPin(PIN_TAPE_FRONT_L_ADC);
    s.tapeFrontLm = AD_ReadADPin(PIN_TAPE_FRONT_LM_ADC);
    s.tapeFrontM = AD_ReadADPin(PIN_TAPE_FRONT_M_ADC);
    s.tapeFrontMr = AD_ReadADPin(PIN_TAPE_FRONT_MR_ADC);
    s.tapeFrontR = AD_ReadADPin(PIN_TAPE_FRONT_R_ADC);
    s.tapeRearL = AD_ReadADPin(PIN_TAPE_REAR_L_ADC);
    s.tapeRearR = AD_ReadADPin(PIN_TAPE_REAR_R_ADC);

    s.tapeFrontMask = 0;
    if (TapeActive(s.tapeFrontL)) {
        s.tapeFrontMask |= TAPE_F_L_BIT;
    }
    if (TapeActive(s.tapeFrontLm)) {
        s.tapeFrontMask |= TAPE_F_LM_BIT;
    }
    if (TapeActive(s.tapeFrontM)) {
        s.tapeFrontMask |= TAPE_F_M_BIT;
    }
    if (TapeActive(s.tapeFrontMr)) {
        s.tapeFrontMask |= TAPE_F_RM_BIT;
    }
    if (TapeActive(s.tapeFrontR)) {
        s.tapeFrontMask |= TAPE_F_R_BIT;
    }

    s.tapeRearMask = 0;
    if (TapeActive(s.tapeRearL)) {
        s.tapeRearMask |= TAPE_R_LM_BIT;
    }
    if (TapeActive(s.tapeRearR)) {
        s.tapeRearMask |= TAPE_R_RM_BIT;
    }

    s.beaconLeft = AD_ReadADPin(PIN_BEACON_LEFT_ADC);
    s.beaconCenter = AD_ReadADPin(PIN_BEACON_CENTER_ADC);
    s.beaconRight = AD_ReadADPin(PIN_BEACON_RIGHT_ADC);
    s.beaconError = (int)s.beaconRight - (int)s.beaconLeft;

    best = s.beaconLeft;
    if (s.beaconCenter > best) {
        best = s.beaconCenter;
    }
    if (s.beaconRight > best) {
        best = s.beaconRight;
    }
    s.beaconLocked = (best > BEACON_LOCK_THRESH);

    s.bumperMask = ReadBumperMask();
    s.batteryRaw = AD_ReadADPin(PIN_BATTERY_ADC);
    s.batteryMv = s.batteryRaw * BATTERY_ADC_MV_PER_COUNT;

    return s;
}

void SensorTest_PrintTape(void)
{
    SensorSnapshot_t s = SensorTest_ReadSnapshot();
    TapeFeature_t feature;
    unsigned int rearWideStableMs;

    printf("\r\ntime_ms=%u tape_front_mask=0x%02x tape_rear_mask=0x%02x\r\n",
           s.timeMs, s.tapeFrontMask, s.tapeRearMask);
    printf("tape_raw fl=%u flm=%u fm=%u fmr=%u fr=%u rl=%u rr=%u\r\n",
           s.tapeFrontL, s.tapeFrontLm, s.tapeFrontM, s.tapeFrontMr,
           s.tapeFrontR, s.tapeRearL, s.tapeRearR);
    TapeFeature_Update(&FeatureEstimator, s.tapeFrontMask, s.tapeRearMask,
                       s.timeMs, &feature);
    rearWideStableMs = RearWideStableMs(&feature, s.timeMs);
    printf("tape_feature Llock=%u Rlock=%u Lsoft=%u Rsoft=%u Ldanger=%u Rdanger=%u ",
           (unsigned int)feature.leftEdgeLocked,
           (unsigned int)feature.rightEdgeLocked,
           (unsigned int)feature.leftEdgeSoftLost,
           (unsigned int)feature.rightEdgeSoftLost,
           (unsigned int)feature.leftBoundaryDanger,
           (unsigned int)feature.rightBoundaryDanger);
    printf("wide=%u all=%u rearAny=%u rearWide=%u rearWideMs=%u rearConfirm=%u latErr=%d headErr=%d conf=%d\r\n",
           (unsigned int)feature.frontWideBlack,
           (unsigned int)feature.frontAllBlack,
           (unsigned int)feature.rearAnyBlack,
           (unsigned int)feature.rearWideBlack,
           rearWideStableMs,
           (unsigned int)(rearWideStableMs >= REAR_WIDE_CONFIRM_MS),
           feature.lateralError, feature.headingError, feature.confidence);
}

void SensorTest_PrintTapeFeature(void)
{
    SensorTest_PrintTape();
}

void SensorTest_PrintBeacon(void)
{
    SensorSnapshot_t s = SensorTest_ReadSnapshot();

    printf("\r\ntime_ms=%u beacon_left=%u beacon_center=%u beacon_right=%u ",
           s.timeMs, s.beaconLeft, s.beaconCenter, s.beaconRight);
    printf("beacon_error=%d beacon_locked=%u\r\n",
           s.beaconError, s.beaconLocked);
}

void SensorTest_PrintBumpers(void)
{
    SensorSnapshot_t s = SensorTest_ReadSnapshot();

    printf("\r\ntime_ms=%u bumper_mask=0x%02x\r\n", s.timeMs, s.bumperMask);
}

void SensorTest_PrintSnapshot(void)
{
    SensorSnapshot_t s = SensorTest_ReadSnapshot();
    TapeFeature_t feature;
    unsigned int rearWideStableMs;

    printf("\r\ntime_ms=%u\r\n", s.timeMs);
    printf("tape_front_mask=0x%02x tape_rear_mask=0x%02x\r\n",
           s.tapeFrontMask, s.tapeRearMask);
    TapeFeature_Update(&FeatureEstimator, s.tapeFrontMask, s.tapeRearMask,
                       s.timeMs, &feature);
    rearWideStableMs = RearWideStableMs(&feature, s.timeMs);
    printf("tape_feature Llock=%u Rlock=%u Ldanger=%u Rdanger=%u wide=%u all=%u rearAny=%u rearWide=%u rearWideMs=%u rearConfirm=%u\r\n",
           (unsigned int)feature.leftEdgeLocked,
           (unsigned int)feature.rightEdgeLocked,
           (unsigned int)feature.leftBoundaryDanger,
           (unsigned int)feature.rightBoundaryDanger,
           (unsigned int)feature.frontWideBlack,
           (unsigned int)feature.frontAllBlack,
           (unsigned int)feature.rearAnyBlack,
           (unsigned int)feature.rearWideBlack,
           rearWideStableMs,
           (unsigned int)(rearWideStableMs >= REAR_WIDE_CONFIRM_MS));
    printf("beacon_left=%u beacon_center=%u beacon_right=%u beacon_error=%d beacon_locked=%u\r\n",
           s.beaconLeft, s.beaconCenter, s.beaconRight,
           s.beaconError, s.beaconLocked);
    printf("bumper_mask=0x%02x\r\n", s.bumperMask);
    printf("battery_raw=%u battery_mv=%u\r\n", s.batteryRaw, s.batteryMv);
}
