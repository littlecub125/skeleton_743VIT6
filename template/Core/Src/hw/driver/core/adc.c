/*
 * adc.c
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#include "adc.h"
#include "info.h"

//-- Definition
//
#define TS_CAL1  (*(uint16_t*)0x1FF1E820)   // 30°C 기준 raw
#define TS_CAL2  (*(uint16_t*)0x1FF1E840)   // 110°C 기준 raw

typedef struct
{
  ADC_HandleTypeDef *h_adc;
  bool is_open;
} adc_tbl_t;

//-- Functions
//
static void cliAdc(int argc, char *argv[]);

//-- Variables
//
static adc_tbl_t adc_tbl[HW_ADC_CH_MAX];

bool adcInit(ADC_HandleTypeDef *h_adc_list[])
{
  for (int i = 0; i < HW_ADC_CH_MAX; i++)
  {
    adc_tbl[i].h_adc = h_adc_list[i];
    adc_tbl[i].is_open = false;
  }
  cliAdd("adc", cliAdc);
  return true;
}

bool adcOpen(AdcChName_t name)
{
  if (name >= HW_ADC_CH_MAX || adc_tbl[name].h_adc == NULL)
  {
    return false;
  }
  adc_tbl[name].is_open = true;
  return true;
}

uint16_t adcRead(AdcChName_t ch)
{
  if (ch != HW_ADC_CH_VBAT && ch != HW_ADC_CH_TEMP)
    return 0;

  ADC_HandleTypeDef *h_adc = adc_tbl[ch].h_adc;

  HAL_ADC_Start(h_adc);

  HAL_ADC_PollForConversion(h_adc, 10);
  uint16_t vbat_raw = HAL_ADC_GetValue(h_adc);   // Rank 1

  HAL_ADC_PollForConversion(h_adc, 10);
  uint16_t temp_raw = HAL_ADC_GetValue(h_adc);   // Rank 2

  HAL_ADC_Stop(h_adc);

  return (ch == HW_ADC_CH_VBAT) ? vbat_raw : temp_raw;
}

int32_t adcReadVoltage(AdcChName_t ch)
{
  uint16_t raw = adcRead(ch);
  float ret = 0;

  if (ch == HW_ADC_CH_VBAT)
  {
    // raw = 읽은 값
    // 3300 = 입력전압
    // 실제전압 = ADC 핀 전압 * 4
    ret = ((int32_t) raw * 3300.0f * 4.0f / 65535.0f);  // 16bit 기준(4095면 12bit)
  }
  else if (ch == HW_ADC_CH_TEMP)
  {
    ret = (int32_t) (raw - TS_CAL1) * (110.0f - 30.0f)
        / (int32_t) (TS_CAL2 - TS_CAL1) + 30.0f;
  }

  return ret;
}


static void cliAdc(int argc, char *argv[])
{
  bool ret = false;

  if (argc == 2)
  {
    if (cliCheck(argv[1], "read"))
    {
      int32_t adc_read[HW_ADC_CH_MAX] = { 0, };
      for (int i = 0; i < HW_ADC_CH_MAX; i++)
      {
        adc_read[i] = adcReadVoltage(i);
      }
      cliPrintf("V_BAT : %d mV", adc_read[0]);
      cliPrintf("V_TEMP : %d C", adc_read[1]);
      ret = true;
    }
    else if (cliCheck(argv[1], "raw"))
    {
      uint16_t vbat_raw = adcRead(HW_ADC_CH_VBAT);
      uint16_t temp_raw = adcRead(HW_ADC_CH_TEMP);
      cliPrintf("vbat_raw=%u", vbat_raw);
      cliPrintf("temp_raw=%u", temp_raw);
      ret = true;
    }
    else if (cliCheck(argv[1], "temp"))
    {
      cliPrintf("TS_CAL1=%u", TS_CAL1);
      cliPrintf("TS_CAL2=%u", TS_CAL2);

    }
  }


  if (ret == false)
  {
    cliPrintf("adc read");
  }
}
