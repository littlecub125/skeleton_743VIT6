/*
 * qspi.h
 *
 *  Created on: 2026. 8. 10.
 *      Author: Yubin Kim
 */


//-- Definition
//


//-- Functions
//


//-- Variables 
//

#ifndef INC_HW_DRIVER_CORE_QSPI_H_
#define INC_HW_DRIVER_CORE_QSPI_H_

#include "hw_def.h"

bool qspiInit(QSPI_HandleTypeDef *h_qspi_list[]);
bool qspiOpen(QspiChName_t name);
bool qspiIsOpen(QspiChName_t name);
bool qspiCommand(QspiChName_t name, QSPI_CommandTypeDef *p_cmd,
    const uint8_t *p_tx, uint8_t *p_rx,
    uint32_t timeout);

#endif /* INC_HW_DRIVER_CORE_QSPI_H_ */
