#ifndef RECOVERY_HSM_H
#define RECOVERY_HSM_H

#include "ES_Configure.h"
#include "ES_Framework.h"

uint8_t InitRecoveryHSM(void);
ES_Event RunRecoveryHSM(ES_Event event);

#endif
