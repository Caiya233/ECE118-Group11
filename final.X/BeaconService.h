#ifndef BEACON_SERVICE_H
#define BEACON_SERVICE_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitBeaconService(uint8_t priority);
uint8_t PostBeaconService(ES_Event event);
ES_Event RunBeaconService(ES_Event event);

int Beacon_GetBearingError(void);
uint8_t Beacon_IsLocked(void);

#endif
