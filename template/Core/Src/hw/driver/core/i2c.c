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
  bool is_open;
} i2c_tbl_t;

//-- Functions
//


//-- Variables
//

//static i2c_tbl_t i2c_tbl[HW_I2C_MAX_CH];

//bool i2cInit(I2C_HandleTypeDef *h_i2c_list[]);
//bool i2cOpen(I2cPortName_t ch);
//bool i2cWriteBytes(I2cPortName_t ch, uint8_t dev_addr, uint8_t reg_addr,
//    const uint8_t *p_data, uint32_t length);
//bool i2cReadBytes(I2cPortName_t ch, uint8_t dev_addr, uint8_t reg_addr,
//    uint8_t *p_data, uint32_t length);
