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
  UART_HandleTypeDef *h_uart;
  bool is_open;
} uart_tbl_t;
//-- Functions
//

//-- Variables
//
static uart_tbl_t uart_tbl[HW_UART_MAX_CH];


bool uartInit(UART_HandleTypeDef *h_uart_list[])
{
  for (int i = 0; i < HW_UART_MAX_CH; i++)
  {
    uart_tbl[i].h_uart = h_uart_list[i];
    uart_tbl[i].is_open = false;
  }
  return true;
}

bool uartOpen(UartPortName_t name)
{
  if (name >= HW_UART_MAX_CH || uart_tbl[name].h_uart == NULL)
  {
    return false;
  }
  uart_tbl[name].is_open = true;
  return true;
}

bool uartIsOpen(UartPortName_t name)
{
  return uart_tbl[name].is_open;
}

bool uartClose(UartPortName_t name)
{
  if (name >= HW_UART_MAX_CH || uart_tbl[name].h_uart == NULL)
  {
    return false;
  }
  uart_tbl[name].is_open = false;
  return true;
}

bool uartAvailable(UartPortName_t ch)
{
  if (__HAL_UART_GET_FLAG(uart_tbl[ch].h_uart, UART_FLAG_ORE))
    __HAL_UART_CLEAR_OREFLAG(uart_tbl[ch].h_uart);   // 오버런 걸렸으면 풀어줌

  return __HAL_UART_GET_FLAG(uart_tbl[ch].h_uart, UART_FLAG_RXNE);
}
uint8_t uartRead(UartPortName_t name)
{
  uint8_t data = 0;
  HAL_UART_Receive(uart_tbl[name].h_uart, &data, 1, 10);
  return data;
}

uint32_t uartReadBytes(UartPortName_t name, uint8_t *p_buf, uint32_t length)
{
  uint32_t i;
  for (i = 0; i < length && uartAvailable(name); i++)
    p_buf[i] = uartRead(name);
  return i;
}

uint32_t uartWrite(UartPortName_t name, const uint8_t *p_data, uint32_t length)
{
  if (HAL_UART_Transmit(uart_tbl[name].h_uart, (uint8_t*) p_data, length, 100)
      != HAL_OK)
    return 0;
  return length;
}

uint32_t uartPrintf(UartPortName_t name, const char *fmt, ...)
{
  char buf[128];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  if (len <= 0)
    return 0;
  if (len >= (int) sizeof(buf))
    len = sizeof(buf) - 1;
  return uartWrite(name, (uint8_t*) buf, (uint32_t) len);
}
