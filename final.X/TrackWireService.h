#ifndef TRACKWIRE_SERVICE_H
#define TRACKWIRE_SERVICE_H

#include "ES_Configure.h"
#include "ES_Framework.h"

#define TRACKWIRE_LEFT_MASK  0x01
#define TRACKWIRE_RIGHT_MASK 0x02
#define TRACKWIRE_BOTH_MASK  (TRACKWIRE_LEFT_MASK | TRACKWIRE_RIGHT_MASK)

uint8_t InitTrackWireService(uint8_t priority);
uint8_t PostTrackWireService(ES_Event event);
ES_Event RunTrackWireService(ES_Event event);

#endif
