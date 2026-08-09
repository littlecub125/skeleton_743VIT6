/*
 * uart.h
 *
 *  Created on: Aug 8, 2026
 *      Author: Yubin Kim
 */

#ifndef INC_HW_DRIVER_CORE_UART_H_
#define INC_HW_DRIVER_CORE_UART_H_

#include "hw_def.h"

bool uartInit(UART_HandleTypeDef *h_uart_list[]);
bool uartOpen(UartPortName_t ch);
bool uartClose(UartPortName_t ch);
bool uartIsOpen(UartPortName_t ch);
bool uartAvailable(UartPortName_t ch);
uint8_t uartRead(UartPortName_t ch);
uint32_t uartReadBytes(UartPortName_t ch, uint8_t *p_buf, uint32_t length);
uint32_t uartWrite(UartPortName_t ch, const uint8_t *p_data, uint32_t length);
uint32_t uartPrintf(UartPortName_t ch, const char *fmt, ...);

#endif /* INC_HW_DRIVER_CORE_UART_H_ */
