/*
 * fs90r.h
 *
 *  Created on: 2026. 8. 30.
 *      Author: Yubin Kim
 */


//-- Definition
//


//-- Functions
//


//-- Variables 
//

#ifndef INC_HW_DRIVER_FS90R_H_
#define INC_HW_DRIVER_FS90R_H_
/* @formatter:off */

#include "hw_def.h"

typedef enum
{
  FS90R_CMD_STOP,
  FS90R_CMD_CW_MOVE,
  FS90R_CMD_CCW_MOVE,
} FS90R_Cmd_t;

bool fs90rInit(void);
bool fs90rCmdSet(FS90R_Cmd_t cmd);
FS90R_Cmd_t fs90rGetCurrentCmd(void);

/* @formatter:on */

#endif /* INC_HW_DRIVER_FS90R_H_ */
