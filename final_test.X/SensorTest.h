#ifndef SENSOR_TEST_H
#define SENSOR_TEST_H

#include <stdint.h>

typedef struct {
    unsigned int timeMs;
    uint8_t tapeFrontMask;
    uint8_t tapeRearMask;
    unsigned int tapeFrontL;
    unsigned int tapeFrontLm;
    unsigned int tapeFrontM;
    unsigned int tapeFrontMr;
    unsigned int tapeFrontR;
    unsigned int tapeRearL;
    unsigned int tapeRearR;
    unsigned int beaconLeft;
    unsigned int beaconCenter;
    unsigned int beaconRight;
    int beaconError;
    uint8_t beaconLocked;
    uint8_t bumperMask;
    unsigned int batteryRaw;
    unsigned int batteryMv;
} SensorSnapshot_t;

void SensorTest_Init(void);
SensorSnapshot_t SensorTest_ReadSnapshot(void);
void SensorTest_PrintTape(void);
void SensorTest_PrintTapeFeature(void);
void SensorTest_PrintBeacon(void);
void SensorTest_PrintBumpers(void);
void SensorTest_PrintSnapshot(void);

#endif
