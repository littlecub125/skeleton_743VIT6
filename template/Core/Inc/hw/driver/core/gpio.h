/*
 * gpio.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#ifndef INC_HW_DRIVER_CORE_GPIO_H_
#define INC_HW_DRIVER_CORE_GPIO_H_

#include "hw_def.h"
/* @formatter:off */
typedef enum
{
  GPIO_TYPE_INPUT,
  GPIO_TYPE_OUTPUT,
} GpioType_t;

/* @formatter:on */

bool gpioInit(void);

bool gpioSetPin(GpioPinName_t pin, GPIO_PinState state);

bool gpioTogglePin(GpioPinName_t pin);

GPIO_PinState gpioGetOutputStatus(GpioPinName_t pin);

GPIO_PinState gpioGetInputStatus(GpioPinName_t pin);

#endif /* INC_HW_DRIVER_CORE_GPIO_H_ */
