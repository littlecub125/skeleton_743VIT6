/*
 * pwm.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#ifndef INC_HW_DRIVER_CORE_PWM_H_
#define INC_HW_DRIVER_CORE_PWM_H_

#include "hw_def.h"

bool pwmInit(TIM_HandleTypeDef *h_tim_list[]);
bool pwmOpen(PWMChName_t name, uint8_t channel);
bool pwmWrite(PWMChName_t name, uint32_t freq);
uint32_t pwmRead(PWMChName_t name);

#endif /* INC_HW_DRIVER_CORE_PWM_H_ */
