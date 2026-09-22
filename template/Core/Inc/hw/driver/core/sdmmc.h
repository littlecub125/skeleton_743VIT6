/*
 * sdmmc.h
 *
 *  Created on: Aug 30, 2026
 *      Author: Yubin Kim
 */

#include "hw_def.h"


bool sdmmcInit(SD_HandleTypeDef *h_sd_list[]);
bool sdmmcOpen(SdmmcChName_t ch);
bool sdmmcIsCardPresent(SdmmcChName_t ch); // GPIO CD핀 또는 HAL_SD_GetCardState()로 판단
bool sdmmcReadBlocks(SdmmcChName_t ch, uint8_t *p_buf, uint32_t block_addr,
    uint32_t block_count);
bool sdmmcWriteBlocks(SdmmcChName_t ch, const uint8_t *p_buf,
    uint32_t block_addr, uint32_t block_count);

uint32_t sdmmcGetSectorCount(SdmmcChName_t ch);

