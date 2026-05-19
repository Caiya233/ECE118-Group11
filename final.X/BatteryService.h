#ifndef BATTERY_SERVICE_H
#define BATTERY_SERVICE_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitBatteryService(uint8_t priority);
uint8_t PostBatteryService(ES_Event event);
ES_Event RunBatteryService(ES_Event event);

#endif
