/*
 * qspi.c
 *
 *  Created on: 2026. 8. 10.
 *      Author: Yubin Kim
 */
#include "qspi.h"


//-- Definition
//
typedef struct
{
  const char *ch_name;
  const char *usage;
  const uint32_t baudrate;
} qspi_info_t;

typedef struct
{
  QSPI_HandleTypeDef *h_qspi;
  bool is_open;
} qspi_tbl_t;

//-- Functions
//


//-- Variables
//
static qspi_tbl_t qspi_tbl[HW_QSPI_MAX_CH];


bool qspiInit(QSPI_HandleTypeDef *h_qspi_list[])
{
  for (int i = 0; i < HW_QSPI_MAX_CH; i++)
  {
    qspi_tbl[i].h_qspi = h_qspi_list[i];
    qspi_tbl[i].is_open = false;
  }
  return true;
}

bool qspiOpen(QspiChName_t name)
{
  if (name >= HW_QSPI_MAX_CH || qspi_tbl[name].h_qspi == NULL)
  {
    return false;
  }
  qspi_tbl[name].is_open = true;
  return true;
}

bool qspiIsOpen(QspiChName_t name)
{
  return qspi_tbl[name].is_open;
}

bool qspiCommand(QspiChName_t ch, QSPI_CommandTypeDef *p_cmd,
    const uint8_t *tx, uint8_t *rx, uint32_t timeout)
{
  if (ch >= HW_QSPI_MAX_CH || qspi_tbl[ch].h_qspi == NULL)
    return false;

  if (HAL_QSPI_Command(qspi_tbl[ch].h_qspi, p_cmd, timeout) != HAL_OK)
    return false;
  if (tx != NULL)
    return HAL_QSPI_Transmit(qspi_tbl[ch].h_qspi, (uint8_t*) tx, timeout)
        == HAL_OK;
  if (rx != NULL)
    return HAL_QSPI_Receive(qspi_tbl[ch].h_qspi, rx, timeout) == HAL_OK;
  return true;
}

