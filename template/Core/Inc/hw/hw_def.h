/*
 * hw_def.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#ifndef INC_HW_HW_DEF_H_
#define INC_HW_HW_DEF_H_


#include "main.h"

//#define _USE_HW_RTOS
//#define _USE_HW_MICROS
//#define _USE_HW_QSPI
//#define _USE_HW_FLASH
//#define _USE_HW_NVS
#define _USE_HW_UART
#define _USE_HW_GPIO
#define _USE_HW_PWM
#define _USE_HW_I2C
#define _USE_HW_SPI
#define _USE_HW_ADC

#ifdef  _USE_HW_UART
#define   UART_RX_BUF_LENGTH      256
#define   UART_TX_BUF_LENGTH      256
/* @formatter:off */
typedef enum
{
  HW_UART_CH_CLI,
  HW_UART_MAX_CH,
} UART_NAME;
/* @formatter:on */
#endif

#ifdef  _USE_HW_GPIO

typedef enum
{
  GPIO_PIN_MAX,
} GPIO_PIN_NAME;

#endif

#ifdef  _USE_HW_PWM

#endif

#ifdef  _USE_HW_I2C

#endif

#ifdef  _USE_HW_SPI

#endif

#ifdef  _USE_HW_ADC
typedef enum
{
  ADC_CH_MAX,
} ADC_CH_NAME;

#endif
#endif /* INC_HW_HW_DEF_H_ */
