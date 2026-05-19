#ifndef BUMPER_SERVICE_H
#define BUMPER_SERVICE_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitBumperService(uint8_t priority);
uint8_t PostBumperService(ES_Event event);
ES_Event RunBumperService(ES_Event event);

#endif
