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
#include <string.h>
#include <stdlib.h>

//#define _USE_HW_RTOS
//#define _USE_HW_MICROS
#define _USE_HW_QSPI
//#define _USE_HW_FLASH
//#define _USE_HW_NVS
#define _USE_HW_UART
#define _USE_HW_GPIO
#define _USE_HW_PWM
#define _USE_HW_I2C
#define _USE_HW_SPI
#define _USE_HW_ADC
#define _USE_HW_W25Q64
#define _USE_HW_SDMMC

#ifdef  _USE_HW_UART
/* @formatter:off */
typedef enum
{
  HW_UART_CH_CLI,
  HW_UART_CH_ESP,
  HW_UART_MAX_CH,
} UartPortName_t;

#endif

#ifdef  _USE_HW_GPIO

typedef enum
{
  HW_GPIO_PIN_O_VDD_LED,
  HW_GPIO_PIN_I_VDD_LED_SW,
  HW_GPIO_PIN_O_W25Q64_CS,
  HW_GPIO_PIN_O_RELAY_CTRL,
  HW_GPIO_PIN_IO_MAX,
} GpioPinName_t;


#endif

#ifdef  _USE_HW_PWM
typedef enum
{
  HW_PWM_CH_MOTOR_1,
  HW_PWM_CH_MAX
} PWMChName_t;
#endif

#ifdef  _USE_HW_I2C
typedef enum
{
  HW_I2C_CH_DISPLAY_SSD_1306,
  HW_I2C_MAX_CH
} I2CChName_t;
#endif

#ifdef  _USE_HW_SPI
typedef enum
{
  HW_SPI_CH_FLASH,
  HW_SPI_MAX_CH,
} SpiChName_t;;
#endif

#ifdef  _USE_HW_QSPI
typedef enum
{
  HW_QSPI_CH_FLASH,
  HW_QSPI_MAX_CH,
} QspiChName_t;;
#endif

#ifdef  _USE_HW_ADC
typedef enum
{
  HW_ADC_CH_VBAT,
  HW_ADC_CH_TEMP,
  HW_ADC_CH_MAX,
} AdcChName_t;

#endif

#ifdef _USE_HW_SDMMC
typedef enum
{
  HW_SDMMC_CH_MICROSD,
  HW_SDMMC_MAX_CH,
} SdmmcChName_t;
#endif

/* @formatter:on */
#endif /* INC_HW_HW_DEF_H_ */
