/*
 * hw.c
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */
#include "hw.h"

//-- Definition
//


//-- Functions
//


//-- Variables 
//

bool hwInit(hw_config_t *p_cfg)
{
  bool ret = true;
  gpioInit();
#ifdef _USE_HW_UART
  ret &= uartInit(p_cfg->h_uart);
#endif
#ifdef _USE_HW_I2C
  ret &= i2cInit(p_cfg->h_i2c);
#endif
#ifdef _USE_HW_SPI
  ret &= spiInit(p_cfg->h_spi);
#endif
#ifdef _USE_HW_QSPI
  ret &= qspiInit(p_cfg->h_qspi);
#endif
#ifdef _USE_HW_ADC
  adcInit(p_cfg->h_adc);
#endif
#ifdef _USE_HW_PWM
//  pwmInit(p_cfg->h_tim);
#endif
#ifdef _USE_HW_SDMMC
  sdmmcInit(p_cfg->h_sd);
#endif

  w25q64Init();

  rc522Init();
//  ssd1306Init();
  FATFS_LinkDriver(&sdmmc_diskio_driver, SDPath);


  return ret;
}
