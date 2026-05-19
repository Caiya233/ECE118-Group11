#ifndef TOP_HSM_H
#define TOP_HSM_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitTopHSM(uint8_t priority);
uint8_t PostTopHSM(ES_Event event);
ES_Event RunTopHSM(ES_Event event);

#endif
