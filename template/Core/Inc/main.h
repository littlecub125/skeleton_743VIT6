/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PB02_WS25Q64_QSPI_IO2_Pin GPIO_PIN_2
#define PB02_WS25Q64_QSPI_IO2_GPIO_Port GPIOE
#define PE03_VDD_LED_GPIO_O_Pin GPIO_PIN_3
#define PE03_VDD_LED_GPIO_O_GPIO_Port GPIOE
#define PC13_VDD_LED_SW_GPIO_I_Pin GPIO_PIN_13
#define PC13_VDD_LED_SW_GPIO_I_GPIO_Port GPIOC
#define PB02_W25Q64_QSPI_CLK_Pin GPIO_PIN_2
#define PB02_W25Q64_QSPI_CLK_GPIO_Port GPIOB
#define PE08_RELAY_CTRL_Pin GPIO_PIN_8
#define PE08_RELAY_CTRL_GPIO_Port GPIOE
#define PB10_ESP_UART_TX_Pin GPIO_PIN_8
#define PB10_ESP_UART_TX_GPIO_Port GPIOD
#define PB11_ESP_UART_RX_Pin GPIO_PIN_9
#define PB11_ESP_UART_RX_GPIO_Port GPIOD
#define PD11_WS25Q64_QSPI_IO0_Pin GPIO_PIN_11
#define PD11_WS25Q64_QSPI_IO0_GPIO_Port GPIOD
#define PD12_WS25Q64_QSPI_IO1_Pin GPIO_PIN_12
#define PD12_WS25Q64_QSPI_IO1_GPIO_Port GPIOD
#define PD13_WS25Q64_QSPI_IO3_Pin GPIO_PIN_13
#define PD13_WS25Q64_QSPI_IO3_GPIO_Port GPIOD
#define PA09_CLI_UART_TX_Pin GPIO_PIN_9
#define PA09_CLI_UART_TX_GPIO_Port GPIOA
#define PA10_CLI_UART_RX_Pin GPIO_PIN_10
#define PA10_CLI_UART_RX_GPIO_Port GPIOA
#define PD06_W25Q64_SPI_CS_O_Pin GPIO_PIN_6
#define PD06_W25Q64_SPI_CS_O_GPIO_Port GPIOD
#define PD7_W25Q64_SPI_MOSI_Pin GPIO_PIN_7
#define PD7_W25Q64_SPI_MOSI_GPIO_Port GPIOD
#define P3B_W25Q64_SPI_SCK_Pin GPIO_PIN_3
#define P3B_W25Q64_SPI_SCK_GPIO_Port GPIOB
#define PB4_W25Q64_SPI_MISO_Pin GPIO_PIN_4
#define PB4_W25Q64_SPI_MISO_GPIO_Port GPIOB
#define PB06_WS25Q64_QSPI_NCS_Pin GPIO_PIN_6
#define PB06_WS25Q64_QSPI_NCS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
