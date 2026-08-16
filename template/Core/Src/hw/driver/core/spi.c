/*
 * spi.c
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */
#include "spi.h"

//-- Definition
//
typedef struct
{
  const char *ch_name;
  const char *usage;
  const uint32_t baudrate;
} spi_info_t;

typedef struct
{
  SPI_HandleTypeDef *h_spi;
  bool is_open;
} spi_tbl_t;

//-- Functions
//


//-- Variables
//
static spi_tbl_t spi_tbl[HW_SPI_MAX_CH];

bool spiInit(SPI_HandleTypeDef *h_spi_list[])
{
  for (int i = 0; i < HW_SPI_MAX_CH; i++)
  {
    spi_tbl[i].h_spi = h_spi_list[i];
    spi_tbl[i].is_open = false;
  }
  return true;
}

bool spiOpen(SpiChName_t name)
{
  if (name >= HW_SPI_MAX_CH || spi_tbl[name].h_spi == NULL)
  {
    return false;
  }
  spi_tbl[name].is_open = true;
  return true;
}

bool spiIsOpen(SpiChName_t name)
{
  return spi_tbl[name].is_open;
}

bool spiTransfer(SpiChName_t ch, const uint8_t *p_tx, uint8_t *p_rx,
    uint32_t length, uint32_t timeout)
{
  if (ch >= HW_SPI_MAX_CH || spi_tbl[ch].h_spi == NULL)
    return false;

  if (p_tx != NULL && p_rx != NULL)
    return HAL_SPI_TransmitReceive(spi_tbl[ch].h_spi, (uint8_t*) p_tx, p_rx,
        length, timeout) == HAL_OK;
  else if (p_tx != NULL)
    return HAL_SPI_Transmit(spi_tbl[ch].h_spi, (uint8_t*) p_tx, length, timeout)
        == HAL_OK;
  else if (p_rx != NULL)
    return HAL_SPI_Receive(spi_tbl[ch].h_spi, p_rx, length, timeout) == HAL_OK;
  return false;
}

