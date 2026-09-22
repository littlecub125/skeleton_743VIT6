/*
 * sdmmc_diskio.h
 *
 *  Created on: Aug 31, 2026
 *      Author: Yubin Kim
 */

#include "sdmmc.h"
#include "ff_gen_drv.h"

//-- Definition
//


//-- Functions
//


//-- Variables 
//
extern char SDPath[4];
extern Diskio_drvTypeDef sdmmc_diskio_driver;

DSTATUS sdmmcDiskInit(BYTE lun);

DSTATUS sdmmcDiskStatus(BYTE lun);

DRESULT sdmmcDiskRead(BYTE lun, BYTE *buf, DWORD sector, UINT count);

DRESULT sdmmcDiskWrite(BYTE lun, const BYTE *buf, DWORD sector,
    UINT count);

DRESULT sdmmcDiskIoctl(BYTE lun, BYTE cmd, void *buf);

