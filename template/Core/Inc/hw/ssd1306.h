/*
 * ssd1306.h
 *
 *  Created on: 2026. 8. 23.
 *      Author: Yubin Kim
 */


#ifndef INC_HW_SSD1306_H_
#define INC_HW_SSD1306_H_


#include "hw_def.h"
#include "fonts.h"

#define SSD1306_I2C_ADDR         0x78

typedef enum
{
  SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
  SSD1306_COLOR_WHITE = 0x01 /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

uint8_t SSD1306_Init(void);

#endif /* INC_HW_SSD1306_H_ */
