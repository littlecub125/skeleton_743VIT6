/*
 * adc.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#ifndef INC_HW_DRIVER_CORE_ADC_H_
#define INC_HW_DRIVER_CORE_ADC_H_

#include "hw_def.h"

bool adcInit(ADC_HandleTypeDef *h_adc_list[]);
uint16_t adcRead(AdcChName_t ch);
float adcReadVoltage(AdcChName_t ch);

#endif /* INC_HW_DRIVER_CORE_ADC_H_ */
