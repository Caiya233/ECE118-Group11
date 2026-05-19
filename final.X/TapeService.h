#ifndef TAPE_SERVICE_H
#define TAPE_SERVICE_H

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "ProjectConfig.h"

#define TAPE_FRONT_L_BIT     TAPE_F_L_BIT
#define TAPE_FRONT_LM_BIT    TAPE_F_LM_BIT
#define TAPE_FRONT_M_BIT     TAPE_F_M_BIT
#define TAPE_FRONT_MR_BIT    TAPE_F_RM_BIT
#define TAPE_FRONT_R_BIT     TAPE_F_R_BIT

#define TAPE_REAR_L_BIT      TAPE_R_LM_BIT
#define TAPE_REAR_R_BIT      TAPE_R_RM_BIT

uint8_t InitTapeService(uint8_t priority);
uint8_t PostTapeService(ES_Event event);
ES_Event RunTapeService(ES_Event event);

uint8_t Tape_GetFrontMask(void);
uint8_t Tape_GetRearMask(void);

#endif
