/*
 * w25q64.c
 *
 *  Created on: Aug 10, 2026
 *      Author: Yubin Kim
 */

#include "w25q64.h"
#include "gpio.h"
#include "info.h"

/* @formatter:off */
//-- Definition
//
typedef struct
{
  bool (*sendCmd)(uint8_t cmd, uint32_t addr, const uint8_t *tx, uint8_t *rx, uint32_t data_len);
} w25q64_flash_t;

//-- Functions
//
static bool w25q64CsSelect(bool set);
static bool w25q64QspiSendCmd(uint8_t cmd, uint32_t addr, const uint8_t *tx, uint8_t *rx, uint32_t data_len);
static bool w25q64SpiSendCmd(uint8_t cmd, uint32_t addr, const uint8_t *tx, uint8_t *rx, uint32_t data_len);
static void cliW25q64(int argc, char *argv[]);

//-- Variables 
//
static const w25q64_flash_t flash_tbl[W25Q64_MAX_CH] =
{
  { w25q64QspiSendCmd },
  { w25q64SpiSendCmd },
};

static uint32_t time_out = 100;

/* @formatter:on */

bool w25q64Init(void)
{
  bool ret = false;

  cliAdd("w25q", cliW25q64);
  return ret;
}

static bool w25q64CsSelect(bool status)
{
  return gpioSetPin(HW_GPIO_PIN_O_W25Q64_CS, status);
}

static bool w25q64SpiSendCmd(uint8_t cmd, uint32_t addr,
    const uint8_t *tx, uint8_t *rx, uint32_t data_len)
{
  bool ret = false;
  uint8_t hdr[4];
  uint32_t hdr_len = 1;

  hdr[0] = cmd;
  hdr[1] = (addr >> 16) & 0xFF;
  hdr[2] = (addr >> 8) & 0xFF;
  hdr[3] = addr & 0xFF;
  hdr_len = 4;

  w25q64CsSelect(false);
  ret &= spiTransfer(HW_SPI_CH_FLASH, hdr, NULL, hdr_len, time_out);
  if (tx != NULL)
    ret &= spiTransfer(HW_SPI_CH_FLASH, tx, NULL, data_len, time_out);
  if (rx != NULL)
    ret &= spiTransfer(HW_SPI_CH_FLASH, NULL, rx, data_len, time_out);
  ret &= w25q64CsSelect(true);

  return ret;
}

static bool w25q64QspiSendCmd(uint8_t cmd, uint32_t addr,
    const uint8_t *tx, uint8_t *rx, uint32_t data_len)
{
  QSPI_CommandTypeDef qcmd = { 0 };

  qcmd.Instruction = cmd;
  qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
  qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
  qcmd.Address = addr;
  qcmd.DataMode = (tx || rx) ? QSPI_DATA_1_LINE : QSPI_DATA_NONE;
  qcmd.DummyCycles = 0;
  qcmd.NbData = data_len;

  return qspiCommand(HW_QSPI_CH_FLASH, &qcmd, tx, rx, HAL_MAX_DELAY);
}

bool w25q64ReadId(W25q64Ch_t ch, uint8_t *p_mfr, uint16_t *p_dev_id)
{
  uint8_t rx[3] = { 0 };
  flash_tbl[ch].sendCmd(W25Q_CMD_JEDEC_ID, 0, NULL, rx, 3);

  *p_mfr = rx[0];                              // EFh (Winbond)
  *p_dev_id = ((uint16_t) rx[1] << 8) | rx[2];      // 7017h (W25Q64JV)

  return (*p_mfr == 0xEF);
}

bool w25q64IsBusy(W25q64Ch_t ch)
{
  uint8_t sr1 = 0;
  flash_tbl[ch].sendCmd(W25Q_CMD_READ_SR1, 0, NULL, &sr1, 1);
  return (sr1 & W25Q_SR1_BUSY_BIT) != 0;
}

bool w25q64WaitBusy(W25q64Ch_t ch, uint32_t timeout_ms)
{
  uint32_t start = HAL_GetTick();
  while (w25q64IsBusy(ch))
  {
    if (HAL_GetTick() - start > timeout_ms)
      return false;
  }
  return true;
}

bool w25q64WriteEnable(W25q64Ch_t ch)
{
  return flash_tbl[ch].sendCmd(W25Q_CMD_WRITE_ENABLE, 0, NULL, NULL, 0);
}

bool w25q64Read(W25q64Ch_t ch, uint32_t addr, uint8_t *p_buf, uint32_t length)
{
  return flash_tbl[ch].sendCmd(W25Q_CMD_READ_DATA, addr, NULL, p_buf,
      length);
}

bool w25q64PageProgram(W25q64Ch_t ch, uint32_t addr, const uint8_t *p_data,
    uint32_t length)
{
  if (length > W25Q_PAGE_SIZE)
    return false;   // 페이지 경계 넘으면 wrap-around로 데이터 깨짐 (8.2.16절)

  w25q64WriteEnable(ch);
  flash_tbl[ch].sendCmd(W25Q_CMD_PAGE_PROGRAM, addr, p_data, NULL,
      length);

  return w25q64WaitBusy(ch, 10);   // tPP typ 0.4ms, max 3ms
}

bool w25q64SectorErase(W25q64Ch_t ch, uint32_t addr)
{
  w25q64WriteEnable(ch);
  flash_tbl[ch].sendCmd(W25Q_CMD_SECTOR_ERASE, addr, NULL, NULL, 0);

  return w25q64WaitBusy(ch, 500);   // tSE typ 45ms, max 400ms
}

static void cliW25q64(int argc, char *argv[])
{

}
