/*
 * w25q64.h
 *
 *  Created on: Aug 10, 2026
 *      Author: Yubin Kim
 */

#include "hw_def.h"



#ifndef INC_HW_DRIVER_W25Q64_H_
#define INC_HW_DRIVER_W25Q64_H_

/* @formatter:off */
#ifdef _USE_HW_W25Q64
typedef enum
{
  W25Q64_CH_SPI,
  W25Q64_CH_QSPI,
  W25Q64_MAX_CH,
} W25q64Ch_t;

#define W25Q_CMD_WRITE_ENABLE   0x06
#define W25Q_CMD_READ_SR1       0x05
#define W25Q_CMD_READ_DATA      0x03
#define W25Q_CMD_PAGE_PROGRAM   0x02
#define W25Q_CMD_SECTOR_ERASE   0x20
#define W25Q_CMD_JEDEC_ID       0x9F

#define W25Q_SR1_BUSY_BIT       0x01
#define W25Q_PAGE_SIZE          256
#define W25Q_SECTOR_SIZE        4096
#endif

bool     w25q64Init(void);
bool     w25q64ReadId(W25q64Ch_t ch, uint8_t *p_mfr, uint16_t *p_dev_id);
bool     w25q64IsBusy(W25q64Ch_t ch);
bool     w25q64WaitBusy(W25q64Ch_t ch, uint32_t timeout_ms);
bool     w25q64WriteEnable(W25q64Ch_t ch);
bool     w25q64Read(W25q64Ch_t ch, uint32_t addr, uint8_t *p_buf, uint32_t length);
bool     w25q64PageProgram(W25q64Ch_t ch, uint32_t addr, const uint8_t *p_data, uint32_t length);
bool     w25q64SectorErase(W25q64Ch_t ch, uint32_t addr);
/* @formatter:on */

#endif /* INC_HW_DRIVER_W25Q64_H_ */
