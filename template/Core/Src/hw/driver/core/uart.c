/*
 * uart.c
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */
#include "uart.h"

//-- Definition
//

typedef struct
{
  const char *ch_name;
  const char *usage;
  const uint32_t baudrate;
} uart_info_t;

typedef struct
{
  UART_HandleTypeDef *h_uart;
  bool is_open;
} uart_tbl_t;
//-- Functions
//

//-- Variables
//
/* @formatter:off */
const uart_info_t uart_info[HW_UART_MAX_CH] = {
    { "UART_1", "CLI", 1000000 },

};
/* @formatter:on */

