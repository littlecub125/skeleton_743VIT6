/*
 * spi.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#ifndef INC_HW_DRIVER_CORE_SPI_H_
#define INC_HW_DRIVER_CORE_SPI_H_

#include "hw_def.h"

bool spiInit(SPI_HandleTypeDef *h_spi_list[]);
bool spiOpen(SpiChName_t ch);
bool spiTransfer(SpiChName_t ch, const uint8_t *p_tx, uint8_t *p_rx,
    uint32_t length, uint32_t timeout);

#endif /* INC_HW_DRIVER_CORE_SPI_H_ */
