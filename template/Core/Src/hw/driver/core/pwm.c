/*
 * pwm.c
 *
 *  Created on: 2026. 8. 8.
 *      Author: Yubin Kim
 */

#include "pwm.h"

//-- Definition
//
typedef struct
{
  TIM_HandleTypeDef *h_tim;
  uint8_t channel;
  bool is_open;
} pwm_tbl_t;

//-- Functions
//


//-- Variables 
//
static pwm_tbl_t pwm_tbl[HW_PWM_CH_MAX];

bool pwmInit(TIM_HandleTypeDef *h_tim_list[])
{
  for (int i = 0; i < HW_PWM_CH_MAX; i++)
  {
    pwm_tbl[i].h_tim = h_tim_list[i];
    pwm_tbl[i].is_open = false;
  }
  return true;
}

bool pwmOpen(PWMChName_t name, uint8_t channel)
{
  if (name >= HW_PWM_CH_MAX || pwm_tbl[name].h_tim == NULL)
  {
    return false;
  }
  pwm_tbl[name].channel = channel;
  HAL_TIM_PWM_Start(pwm_tbl[name].h_tim, pwm_tbl[name].channel);
  pwm_tbl[name].is_open = true;
  return true;
}
bool pwmWrite(PWMChName_t name, uint32_t freq)
{
  bool ret = true;
  __HAL_TIM_SET_COMPARE(pwm_tbl[name].h_tim, pwm_tbl[name].channel, freq);
  return ret;
}
uint32_t pwmRead(PWMChName_t name)
{
  uint32_t current_pulse = __HAL_TIM_GET_COMPARE(pwm_tbl[name].h_tim,
      pwm_tbl[name].channel);
  return current_pulse;
}

