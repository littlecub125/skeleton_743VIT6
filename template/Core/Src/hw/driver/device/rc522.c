/*
 * rc522.c
 *
 *  Created on: 2026. 9. 10.
 *      Author: Yubin Kim
 */

#include "rc522.h"
#include "info.h"
#include "gpio.h"
#include "spi.h"
//-- Definition
//


//-- Functions
//
static uint8_t rc522ReadReg(uint8_t reg);
static void rc522WriteReg(uint8_t reg, uint8_t value);
static void cliRc522(int argc, char *argv[]);
static bool rc522PcdTransceive(const uint8_t *tx_data, uint8_t tx_len,
    uint8_t *rx_buf, uint8_t *p_rx_len, uint8_t tx_last_bits);
static bool rc522Rst(bool status);
static bool rc522CsSelect(bool status);
//-- Variables 
//
static uint8_t time_out = 100;

bool rc522Init(void)
{
  bool ret = true;

  rc522Rst(true);
  HAL_Delay(100);
  rc522Rst(false);
  HAL_Delay(1);
  rc522Rst(true);
  HAL_Delay(50);

  rc522WriteReg(0x01, 0x0F);             // CommandReg = SoftReset (한 번 더, 확실하게)
  HAL_Delay(50);

  rc522WriteReg(0x2A, 0x8D);                     // TModeReg — TAuto=1
  rc522WriteReg(0x2B, 0x3E);                     // TPrescalerReg
  rc522WriteReg(0x2D, 30);                       // TReloadReg Lo
  rc522WriteReg(0x2C, 0);                        // TReloadReg Hi
  rc522WriteReg(0x15, 0x40);                     // TxAutoReg: Force100ASK=1
  rc522WriteReg(0x11, 0x3D);                     // ModeReg: CRC preset 등 표준값
  rc522WriteReg(0x13, 0x08);
  rc522WriteReg(0x26, 0x70);
  uint8_t tx_ctrl = rc522ReadReg(0x14);
  cliPrintf("1tx_ctrl=0x%02X", rc522ReadReg(0x14)); // 하위 2비트(0x03)가 서있어야 안테나 On
  rc522WriteReg(0x14, 0x83);
  cliPrintf("2tx_ctrl=0x%02X", rc522ReadReg(0x14));


  if ((tx_ctrl & 0x03) != 0x03)
    rc522WriteReg(0x14, tx_ctrl | 0x03); // TxControlReg: Tx1RFEn|Tx2RFEn=1, 안테나 On

  uint8_t ver = rc522ReadReg(0x37);
  ret = (ver != 0x00 && ver != 0xFF);     // 0x90/0x91(정품) 또는 0x92(클론) 나오면 통신 정상

  cliAdd("rc522", cliRc522);

  return ret;
}
static bool rc522PcdTransceive(const uint8_t *tx_data, uint8_t tx_len,
    uint8_t *rx_buf, uint8_t *p_rx_len, uint8_t tx_last_bits)
{
  rc522WriteReg(0x01, 0x00);
  rc522WriteReg(0x04, 0x7F);
  rc522WriteReg(0x0A, 0x80);

  for (uint8_t i = 0; i < tx_len; i++)
    rc522WriteReg(0x09, tx_data[i]);

  rc522WriteReg(0x0D, tx_last_bits);
  rc522WriteReg(0x01, 0x0C);
  rc522WriteReg(0x0D, tx_last_bits | 0x80);

  uint32_t timeout = 2000;
  uint8_t irq;
  do
  {
    irq = rc522ReadReg(0x04);
    if (irq & 0x20)
      break;
  } while (--timeout);

  rc522WriteReg(0x0D, tx_last_bits);

  uint8_t err = rc522ReadReg(0x06);
  uint8_t fifo_len = rc522ReadReg(0x0A);
  cliPrintf("pcd: timeout=%lu irq=0x%02X err=0x%02X fifo=%d", timeout, irq, err,
      fifo_len);   // 임시 디버그

  if (timeout == 0)
    return false;

  if (err & 0x1F)   // BufferOvfl|CollErr|CRCErr|ParityErr|ProtocolErr — 전부 체크
    return false;

  if (fifo_len == 0)      // ← 여기 추가
    return false;

  if (fifo_len > *p_rx_len)
    fifo_len = *p_rx_len;

  for (uint8_t i = 0; i < fifo_len; i++)
    rx_buf[i] = rc522ReadReg(0x09);

  *p_rx_len = fifo_len;
  return true;
}
bool rc522IsCardPresent(void)
{
  uint8_t rx_buf[2];
  uint8_t rx_len = sizeof(rx_buf);
  uint8_t req_cmd = 0x26;

  rc522WriteReg(0x0D, 0x07);

  bool ok = rc522PcdTransceive(&req_cmd, 1, rx_buf, &rx_len, 0x07);
  cliPrintf("req ok=%d err=0x%02X irq=0x%02X", ok, rc522ReadReg(0x06),
      rc522ReadReg(0x04));
  return ok;
}

bool rc522ReadUid(uint8_t *p_uid, uint8_t *p_uid_len)
{
  uint8_t cmd[2] = { 0x93, 0x20 };
  uint8_t rx_buf[5];
  uint8_t rx_len = sizeof(rx_buf);

  if (!rc522PcdTransceive(cmd, 2, rx_buf, &rx_len, 0x00))
  {
    cliPrintf("transceive fail, err=0x%02X", rc522ReadReg(0x06)); // ErrorReg 값 찍기
    return false;
  }

  cliPrintf("rx_len=%d", rx_len);   // 몇 바이트 받았는지 확인

  if (rx_len != 5)
    return false;

  uint8_t bcc = rx_buf[0] ^ rx_buf[1] ^ rx_buf[2] ^ rx_buf[3];
  if (bcc != rx_buf[4])
  {
    cliPrintf("bcc mismatch: calc=0x%02X recv=0x%02X", bcc, rx_buf[4]);
    return false;
  }

  memcpy(p_uid, rx_buf, 4);
  *p_uid_len = 4;
  return true;
}
static uint8_t rc522ReadReg(uint8_t reg)
{
  uint8_t tx[2] = { (uint8_t) ((reg << 1) | 0x80), 0x00 };
  uint8_t rx[2] = { 0 };

  rc522CsSelect(false);
  spiTransfer(HW_SPI_CH_FLASH_AND_RC522, tx, rx, 2, time_out);
  rc522CsSelect(true);

  return rx[1];
}
static bool rc522CsSelect(bool status)
{
  return gpioSetPin(HW_GPIO_PIN_O_RC522_CS, status);
}

static bool rc522Rst(bool status)
{
  return gpioSetPin(HW_GPIO_PIN_O_RC522_RST, status);
}

static void rc522WriteReg(uint8_t reg, uint8_t value)
{
  uint8_t tx[2] = { (uint8_t) ((reg << 1) & 0x7E), value };

  rc522CsSelect(false);
  spiTransfer(HW_SPI_CH_FLASH_AND_RC522, tx, NULL, 2, time_out);
  rc522CsSelect(true);
}


static void cliRc522(int argc, char *argv[])
{
  bool ret = false;

  if (argc == 2)
  {
    if (cliCheck(argv[1], "version"))
    {
      uint8_t ver = rc522ReadReg(0x37);
      cliPrintf("version=0x%02X", ver);
      ret = true;
    }
    else if (cliCheck(argv[1], "uid"))
    {
      uint8_t uid[4];
      uint8_t uid_len = 0;

      if (!rc522IsCardPresent())
      {
        cliPrintf("no card");
      }
      else if (rc522ReadUid(uid, &uid_len))
      {
        cliPrintf("uid=%02X %02X %02X %02X", uid[0], uid[1], uid[2], uid[3]);
      }
      else
      {
        cliPrintf("uid read fail");
      }
      ret = true;
    }
  }

  if (ret == false)
  {
    cliPrintf("rc522 version");
    cliPrintf("rc522 uid");
  }
}
