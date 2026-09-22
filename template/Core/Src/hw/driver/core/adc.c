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
extern DMA_HandleTypeDef hdma_adc3;
#define ADC_DMA_BUF_BASE  ((volatile uint16_t *)0x38000000)
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
static void adc3BdmaWorkaround(void);

//-- Variables
//
static volatile uint16_t *const adc_dma_buf = ADC_DMA_BUF_BASE;
static adc_tbl_t adc_tbl[HW_ADC_CH_MAX];

bool adcInit(ADC_HandleTypeDef *h_adc_list[])
{
  for (int i = 0; i < HW_ADC_CH_MAX; i++)
  {
    adc_tbl[i].h_adc = h_adc_list[i];
    adc_tbl[i].is_open = false;
  }

  int i = 0;
  while (i < HW_ADC_CH_MAX)
  {
    if (adc_tbl[i].h_adc == NULL)
    {
      i++;
      continue;
    }

    int ch_count = 1;
    while (i + ch_count < HW_ADC_CH_MAX
        && adc_tbl[i + ch_count].h_adc == adc_tbl[i].h_adc)
    {
      ch_count++;
    }

    if (HAL_ADC_Start_DMA(adc_tbl[i].h_adc, (uint32_t*) &adc_dma_buf[i],
        ch_count) == HAL_OK)
    {
      if (adc_tbl[i].h_adc->Instance == ADC3)
        adc3BdmaWorkaround();
    }

    i += ch_count;
  }

  cliAdd("adc", cliAdc);
  return true;
}

// ADC3(BDMA) 전용 우회 — HAL_DMA_Init()이 BDMA 채널의 CIRC/MINC/PSIZE/MSIZE와
// DMAMUX2 요청ID(17=ADC3)를 제대로 못 걸어주는 문제를 수동으로 고정한다.
// hdma_adc3/DMAMUX2를 직접 건드리므로 다른 핸들(ADC1/2 등)엔 절대 적용하면 안 됨.
static void adc3BdmaWorkaround(void)
{
  BDMA_Channel_TypeDef *bd = (BDMA_Channel_TypeDef*) hdma_adc3.Instance;

  CLEAR_BIT(bd->CCR, BDMA_CCR_EN);
  while (bd->CCR & BDMA_CCR_EN);

  MODIFY_REG(bd->CCR,
      BDMA_CCR_PSIZE | BDMA_CCR_MSIZE | BDMA_CCR_MINC | BDMA_CCR_PINC | BDMA_CCR_CIRC | BDMA_CCR_DIR,
      BDMA_CCR_CIRC | BDMA_CCR_MINC | (1U << BDMA_CCR_PSIZE_Pos) | (1U << BDMA_CCR_MSIZE_Pos));

  SET_BIT(bd->CCR, BDMA_CCR_EN);

  DMAMUX2_Channel0->CCR = 17;
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
  if (ch >= HW_ADC_CH_MAX)
    return 0;
  return adc_dma_buf[ch];
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
      while (cliKeepLoop())
      {
        for (int i = 0; i < HW_ADC_CH_MAX; i++)
        {
          adc_read[i] = adcReadVoltage(i);
        }
        cliPrintf("V_BAT : %d mV", adc_read[0]);
        cliPrintf("V_TEMP : %d C", adc_read[1]);
        HAL_Delay(100);
        cliLineUp(2);
      }
      cliLineDown(2);
      ret = true;
    }
    else if (cliCheck(argv[1], "raw"))
    {
      uint16_t vbat_raw = 0;
      uint16_t temp_raw = 0;

      while (cliKeepLoop())
      {
        vbat_raw = adcRead(HW_ADC_CH_VBAT);
        temp_raw = adcRead(HW_ADC_CH_TEMP);
        cliPrintf("vbat_raw=%u", vbat_raw);
        cliPrintf("temp_raw=%u", temp_raw);
        HAL_Delay(100);
        cliLineUp(2);
      }
      cliLineDown(2);
      ret = true;
    }
    else if (cliCheck(argv[1], "temp"))
    {
      cliPrintf("TS_CAL1=%u", TS_CAL1);
      cliPrintf("TS_CAL2=%u", TS_CAL2);
      ret = true;
    }
  }


  if (ret == false)
  {
    cliPrintf("adc read");
    cliPrintf("adc raw");
  }
}
