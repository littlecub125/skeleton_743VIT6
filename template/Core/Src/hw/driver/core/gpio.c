/*
 * gpio.c
 *
 *  Created on: 2026. 8. 8.
 *      Author: Yubin Kim
 */
#include "gpio.h"

//-- Definition
//

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pin;
  GpioType_t type;
  GPIO_PinState default_state;
  const char *name;
} gpio_tbl_t;

//-- Functions
//

//-- Variables
//
/* @formatter:off */
static gpio_tbl_t gpio_tbl[HW_GPIO_PIN_IO_MAX] = {
  { PE03_VDD_LED_GPIO_O_GPIO_Port, PE03_VDD_LED_GPIO_O_Pin, GPIO_TYPE_OUTPUT, GPIO_PIN_RESET, "VDD_LED" },
  { PC13_VDD_LED_SW_GPIO_I_GPIO_Port, PC13_VDD_LED_SW_GPIO_I_Pin, GPIO_TYPE_INPUT, GPIO_PIN_RESET, "VDD_LED_SW" },
  { PD06_W25Q64_SPI_CS_GPIO_Port, PD06_W25Q64_SPI_CS_Pin, GPIO_TYPE_OUTPUT, GPIO_PIN_SET, "W25Q64_CS"},
};
/* @formatter:on */
bool gpioInit(void)
{
  return true;
}

bool gpioSetPin(GpioPinName_t pin, GPIO_PinState state)
{
  if (pin >= HW_GPIO_PIN_IO_MAX || gpio_tbl[pin].type != GPIO_TYPE_OUTPUT)
    return false;
  if (gpioGetOutputStatus(pin) != state)
  {
    HAL_GPIO_WritePin(gpio_tbl[pin].port, gpio_tbl[pin].pin, state);
  }
  return true;
}

bool gpioTogglePin(GpioPinName_t pin)
{
  if (pin >= HW_GPIO_PIN_IO_MAX || gpio_tbl[pin].type != GPIO_TYPE_OUTPUT)
    return false;
  HAL_GPIO_TogglePin(gpio_tbl[pin].port, gpio_tbl[pin].pin);
  return true;
}

GPIO_PinState gpioGetOutputStatus(GpioPinName_t pin)
{
  if (pin >= HW_GPIO_PIN_IO_MAX || gpio_tbl[pin].type != GPIO_TYPE_OUTPUT)
    return GPIO_PIN_RESET;
  return HAL_GPIO_ReadPin(gpio_tbl[pin].port, gpio_tbl[pin].pin);
}

GPIO_PinState gpioGetInputStatus(GpioPinName_t pin)
{
  if (pin >= HW_GPIO_PIN_IO_MAX || gpio_tbl[pin].type != GPIO_TYPE_INPUT)
    return GPIO_PIN_RESET;
  return HAL_GPIO_ReadPin(gpio_tbl[pin].port, gpio_tbl[pin].pin);
}
