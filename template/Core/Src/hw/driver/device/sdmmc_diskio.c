/*
 * sdmmc_diskio.c
 *
 *  Created on: Aug 31, 2026
 *      Author: Yubin Kim
 */

#include "sdmmc_diskio.h"
#include "info.h"
#include "ff.h"


//-- Definition
//


//-- Functions
//
static void cliSdmmcDiskio(int argc, char *argv[]);


//-- Variables
//

char SDPath[4];
Diskio_drvTypeDef sdmmc_diskio_driver = { sdmmcDiskInit, sdmmcDiskStatus,
    sdmmcDiskRead, sdmmcDiskWrite, sdmmcDiskIoctl, };
static FATFS sd_fatfs;
/* @formatter:off */

void sdmmcDiskioInit(void)
{
  FATFS_LinkDriver(&sdmmc_diskio_driver, SDPath);
  f_mount(&sd_fatfs, SDPath, 1);
  cliAdd("sdfs", cliSdmmcDiskio);
}


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
//  cliPrintf("disk_read  sector=%lu count=%u", sector, count);   // 디버그용 — sdmmcReadBlocks까지 실제로 내려가는지 확인
  return sdmmcReadBlocks(HW_SDMMC_CH_MICROSD, buf, sector, count) ?
          RES_OK : RES_ERROR;
}

DRESULT sdmmcDiskWrite(BYTE lun, const BYTE *buf, DWORD sector,
    UINT count)
{
//  cliPrintf("disk_write sector=%lu count=%u", sector, count);   // 디버그용 — sdmmcWriteBlocks까지 실제로 내려가는지 확인
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

static void cliSdmmcDiskio(int argc, char *argv[])
{
  bool ret = false;

  if (argc == 2)
  {

    if (cliCheck(argv[1], "write"))
    {
      FIL file;
      UINT bw = 0;
      const char *msg = "sdmmc_diskio write test\r\n";

      char path[16];
      snprintf(path, sizeof(path), "%stest.txt", SDPath);  // "1:/test.txt" 이런 식
      FRESULT fr = f_open(&file, path, FA_CREATE_ALWAYS | FA_WRITE);
      cliPrintf("f_open fr=%d", fr);
      if (fr == FR_OK)
      {
        fr = f_write(&file, msg, strlen(msg), &bw);
        cliPrintf("f_write fr=%d bw=%u", fr, bw);

        fr = f_close(&file);
        cliPrintf("f_close fr=%d", fr);
      }
      ret = true;
    }
    else if (cliCheck(argv[1], "read"))
    {
      FIL file;
      UINT br = 0;
      char buf[64];

      char path[16];
      snprintf(path, sizeof(path), "%stest.txt", SDPath);
      FRESULT fr = f_open(&file, path, FA_READ);
      cliPrintf("f_open fr=%d", fr);
      if (fr == FR_OK)
      {
        fr = f_read(&file, buf, sizeof(buf) - 1, &br);
        buf[br] = 0;
        cliPrintf("f_read fr=%d br=%u", fr, br);
        cliPrintf("%s", buf);

        fr = f_close(&file);
        cliPrintf("f_close fr=%d", fr);
      }
      ret = true;
    }
  }

  if (ret == false)
  {
    cliPrintf("sdfs write");
    cliPrintf("sdfs read");
  }
}
