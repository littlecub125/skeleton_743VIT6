/*
 * i2c.c
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#include "i2c.h"

//-- Definition
//
typedef struct
{
  I2C_HandleTypeDef *h_i2c;
  uint8_t dev_addr;
  bool is_open;
} i2c_tbl_t;

//-- Functions
//


//-- Variables
//
static i2c_tbl_t i2c_tbl[HW_I2C_MAX_CH];

bool i2cInit(I2C_HandleTypeDef *h_i2c_list[])
{
  for (int i = 0; i < HW_I2C_MAX_CH; i++)
  {
    i2c_tbl[i].h_i2c = h_i2c_list[i];
    i2c_tbl[i].is_open = false;
  }
  return true;
}
bool i2cOpen(I2CChName_t name, uint8_t addr)
{
  if (name >= HW_I2C_MAX_CH || i2c_tbl[name].h_i2c == NULL)
  {
    return false;
  }
  i2c_tbl[name].dev_addr = addr;
  i2c_tbl[name].is_open = true;

  return true;
}
bool i2cWriteBytes(I2CChName_t name, uint8_t *p_data, uint32_t length)
{
  HAL_I2C_Master_Transmit(i2c_tbl[name].h_i2c, i2c_tbl[name].dev_addr, p_data,
      length, 100);

  return true;
}
bool i2cReadBytes(I2CChName_t name, uint8_t dev_addr, uint8_t reg_addr,
    uint8_t *p_data, uint32_t length)
{
  if (HAL_I2C_Mem_Read(i2c_tbl[name].h_i2c, i2c_tbl[name].dev_addr, reg_addr,
  I2C_MEMADD_SIZE_8BIT, p_data, length, 100) != HAL_OK)
  {
    return false;
  }
  return true;
}

bool i2cDeviceReady(I2CChName_t name)
{
  bool ret = false;

  if (HAL_I2C_IsDeviceReady(i2c_tbl[name].h_i2c, i2c_tbl[name].dev_addr, 1,
      20000) == HAL_OK)
  {
    ret = true;
  }

  return ret;
}
