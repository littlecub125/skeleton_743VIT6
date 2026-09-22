/*
 * rc522.h
 *
 *  Created on: 2026. 9. 10.
 *      Author: Yubin Kim
 */


//-- Definition
//


//-- Functions
//


//-- Variables 
//

#ifndef INC_HW_DRIVER_DEVICE_RC522_H_
#define INC_HW_DRIVER_DEVICE_RC522_H_

#include "hw_def.h"

bool rc522Init(void);
bool rc522IsCardPresent(void);
bool rc522ReadUid(uint8_t *p_uid, uint8_t *p_uid_len);

#endif /* INC_HW_DRIVER_DEVICE_RC522_H_ */
