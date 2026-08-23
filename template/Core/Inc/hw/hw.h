/*
 * hw.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#ifndef INC_HW_DRIVER_HW_H_
#define INC_HW_DRIVER_HW_H_

#include "hw_def.h"
#include "uart.h"
#include "gpio.h"
#include "spi.h"
#include "qspi.h"
#include "adc.h"
#include "i2c.h"
#include "ssd1306.h"
#include "w25q64.h"

typedef struct
{
  UART_HandleTypeDef *h_uart[HW_UART_MAX_CH];
  I2C_HandleTypeDef *h_i2c[HW_I2C_MAX_CH];
  SPI_HandleTypeDef *h_spi[HW_SPI_MAX_CH];
  QSPI_HandleTypeDef *h_qspi[HW_QSPI_MAX_CH];
  ADC_HandleTypeDef *h_adc[HW_ADC_CH_MAX];
} hw_config_t;

bool hwInit(hw_config_t *p_cfg);
bool hwIsInit(void);

#endif /* INC_HW_DRIVER_HW_H_ */
