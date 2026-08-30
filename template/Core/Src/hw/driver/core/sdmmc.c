/*
 * sdmmc.c
 *
 *  Created on: Aug 30, 2026
 *      Author: Yubin Kim
 */

#include "sdmmc.h"

//-- Definition
//

typedef struct
{
  SD_HandleTypeDef *h_sd;
  bool is_open;
} sdmmc_tbl_t;


//-- Functions
//


//-- Variables 
//

static sdmmc_tbl_t sdmmc_tbl[HW_SDMMC_MAX_CH];

bool sdmmcInit(SD_HandleTypeDef *h_sd_list[])
{
  for (int i = 0; i < HW_SDMMC_MAX_CH; i++)
  {
    sdmmc_tbl[i].h_sd = h_sd_list[i];
    sdmmc_tbl[i].is_open = false;
  }
  return true;
}
bool sdmmcOpen(SdmmcChName_t name)
{
  if (name >= HW_SDMMC_MAX_CH || sdmmc_tbl[name].h_sd == NULL)
  {
    return false;
  }

  sdmmc_tbl[name].is_open = true;

  return true;
}
bool sdmmcIsCardPresent(SdmmcChName_t ch)
{
  return HAL_SD_GetCardState(sdmmc_tbl[ch].h_sd) == HAL_SD_CARD_TRANSFER;
}
bool sdmmcReadBlocks(SdmmcChName_t ch, uint8_t *p_buf, uint32_t block_addr,
    uint32_t block_count)
{
  bool ret = false;
  if (ch >= HW_SDMMC_MAX_CH || sdmmc_tbl[ch].h_sd == NULL)
    return ret;

  if (HAL_SD_ReadBlocks(sdmmc_tbl[ch].h_sd, p_buf, block_addr, block_count,
      1000) == HAL_OK)
  {
    ret = true;
  }

  return ret;
}

bool sdmmcWriteBlocks(SdmmcChName_t ch, const uint8_t *p_buf,
    uint32_t block_addr, uint32_t block_count)
{
  bool ret = false;
  if (ch >= HW_SDMMC_MAX_CH || sdmmc_tbl[ch].h_sd == NULL)
    return ret;

  if (HAL_SD_WriteBlocks(sdmmc_tbl[ch].h_sd, (uint8_t*) p_buf, block_addr,
      block_count, 1000) == HAL_OK)
  {
    ret = true;
  }

  return ret;
}

uint32_t sdmmcGetSectorCount(SdmmcChName_t ch)
{
  HAL_SD_CardInfoTypeDef info;
  HAL_SD_GetCardInfo(sdmmc_tbl[ch].h_sd, &info);
  return info.LogBlockNbr;
}
