#include "BOARD.h"
#include "BatteryTest.h"
#include "SensorTest.h"
#include <stdio.h>

void BatteryTest_Print(void)
{
    SensorSnapshot_t s = SensorTest_ReadSnapshot();

    printf("\r\nbattery_raw=%u battery_mv=%u\r\n", s.batteryRaw, s.batteryMv);
}
