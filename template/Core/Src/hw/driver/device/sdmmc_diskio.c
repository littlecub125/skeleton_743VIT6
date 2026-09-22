/*
 * sdmmc_diskio.c
 *
 *  Created on: Aug 31, 2026
 *      Author: Yubin Kim
 */

#include "sdmmc_diskio.h"


//-- Definition
//


//-- Functions
//


//-- Variables 
//

char SDPath[4];
Diskio_drvTypeDef sdmmc_diskio_driver = { sdmmcDiskInit, sdmmcDiskStatus,
    sdmmcDiskRead, sdmmcDiskWrite, sdmmcDiskIoctl, };
/* @formatter:off */



DSTATUS sdmmcDiskInit(BYTE lun)
{
  return sdmmcOpen(HW_SDMMC_CH_MICROSD) ? RES_OK : STA_NOINIT;
}

DSTATUS sdmmcDiskStatus(BYTE lun)
{
  return sdmmcIsCardPresent(HW_SDMMC_CH_MICROSD) ? RES_OK : STA_NODISK;
}

DRESULT sdmmcDiskRead(BYTE lun, BYTE *buf, DWORD sector, UINT count)
{
  return sdmmcReadBlocks(HW_SDMMC_CH_MICROSD, buf, sector, count) ?
          RES_OK : RES_ERROR;
}

DRESULT sdmmcDiskWrite(BYTE lun, const BYTE *buf, DWORD sector,
    UINT count)
{
  return sdmmcWriteBlocks(HW_SDMMC_CH_MICROSD, buf, sector, count) ?
          RES_OK : RES_ERROR;
}

DRESULT sdmmcDiskIoctl(BYTE lun, BYTE cmd, void *buf)
{
  switch (cmd)
  {
    case CTRL_SYNC:
      return RES_OK;
    case GET_SECTOR_COUNT:
      *(DWORD *)buf = sdmmcGetSectorCount(HW_SDMMC_CH_MICROSD);
      return RES_OK;
    default:
      return RES_PARERR;
  }
}
/* @formatter:on */
