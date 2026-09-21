/*
 * sdmmc.c
 *
 *  Created on: Aug 30, 2026
 *      Author: Yubin Kim
 */

#include "sdmmc.h"
#include "info.h"

//-- Definition
//

typedef struct
{
  SD_HandleTypeDef *h_sd;
  bool is_open;
} sdmmc_tbl_t;


//-- Functions
//
static void cliSdmmc(int argc, char *argv[]);

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
  cliAdd("sdmmc", cliSdmmc);

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
  if (ch >= HW_SDMMC_MAX_CH || sdmmc_tbl[ch].h_sd == NULL)
    return false;

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

static void cliSdmmc(int argc, char *argv[])
{
  bool ret = false;

  SdmmcChName_t ch = HW_SDMMC_CH_MICROSD;   // 채널 1개뿐이라 고정

  if (argc == 2)
  {
    if (cliCheck(argv[1], "test"))
    {
      // 검증 순서 1~2단계: 카드 인식 + 섹터 0(MBR) 읽기
      bool present = sdmmcIsCardPresent(ch);
      cliPrintf("card present=%d", present);

      uint8_t buf[512] = { 0, };
      bool ok = sdmmcReadBlocks(ch, buf, 0, 1);
      cliPrintf("read sector0 ok=%d", ok);
      if (!ok)
      {
        cliPrintf("hal state=%d error=0x%08lX", sdmmc_tbl[ch].h_sd->State,
            HAL_SD_GetError(sdmmc_tbl[ch].h_sd));
      }

      if (ok)
      {
        char line[64];
        int pos = 0;
        for (int i = 0; i < 16; i++)   // 앞 16바이트만 hex로 찍어서 랜덤값 아닌지 확인
          pos += snprintf(line + pos, sizeof(line) - pos, "%02X ", buf[i]);
        cliPrintf("%s", line);
      }
      ret = true;
    }
    else if (cliCheck(argv[1], "detect"))
    {
      // 검증 순서 5단계: 카드 뽑았다 다시 꽂았을 때 재인식되는지 반복 확인용
      cliPrintf("card present=%d", sdmmcIsCardPresent(ch));
      ret = true;
    }
  }

  if (ret == false)
  {
    cliPrintf("sdmmc test");
    cliPrintf("sdmmc detect");
  }
}
