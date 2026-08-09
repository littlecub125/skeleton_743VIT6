/*
 * hw_def.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#ifndef INC_HW_HW_DEF_H_
#define INC_HW_HW_DEF_H_


#include "main.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

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
/* @formatter:off */
typedef enum
{
  HW_UART_CH_CLI,
  HW_UART_MAX_CH,
} UartPortName_t;
/* @formatter:on */
#endif

#ifdef  _USE_HW_GPIO

typedef enum
{
  GPIO_PIN_MAX,
} GpioPinName_t;

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
} AdcChNAME_t;

#endif
#endif /* INC_HW_HW_DEF_H_ */
