/*
 * infocli.c
 *
 *  Created on: Aug 16, 2026
 *      Author: Yubin Kim
 */

#include "info.h"
#include <string.h>

//-- Definition
//
#define CLI_MAX_CMD   16
typedef void (*cli_func_t)(int argc, char *argv[]);

typedef struct
{
  const char *name;
  cli_func_t func;
} cli_cmd_t;

//-- Functions
//
static void cliExecute(char *line);

//-- Variables 
//
static char cli_buf[1024];
static uint8_t cli_idx = 0;

static cli_cmd_t cli_tbl[CLI_MAX_CMD];
static uint8_t cli_cmd_cnt = 0;


void infoCliInit(void)
{
  cli_idx = 0;
}

bool cliAdd(const char *name, cli_func_t func)
{
  if (cli_cmd_cnt >= CLI_MAX_CMD)
    return false;

  cli_tbl[cli_cmd_cnt].name = name;
  cli_tbl[cli_cmd_cnt].func = func;
  cli_cmd_cnt++;

  return true;
}

void infoCliUpdate(void)
{
  if (!uartAvailable(HW_UART_CH_CLI))
    return;

  uint8_t data = uartRead(HW_UART_CH_CLI);

  if (data == '\r')
  {
    cli_buf[cli_idx] = '\0';
    uartPrintf(HW_UART_CH_CLI, "\r\n");

    cliExecute(cli_buf);

    cli_idx = 0;
    uartPrintf(HW_UART_CH_CLI, "cli# ");
  }
  else if (data == '\b' || data == 0x7F)
  {
    if (cli_idx > 0)
    {
      cli_idx--;
      uartPrintf(HW_UART_CH_CLI, "\b \b");   // 커서 뒤로, 공백으로 지우기, 다시 뒤로
    }
  }
  else
  {
    if (cli_idx < sizeof(cli_buf) - 1)
      cli_buf[cli_idx++] = data;
    uartPrintf(HW_UART_CH_CLI, "%c", data);
  }
}
static void cliExecute(char *line)
{
  char *argv[8];
  int argc = 0;

  char *tok = strtok(line, " ");
  while (tok != NULL && argc < 8)
  {
    argv[argc++] = tok;
    tok = strtok(NULL, " ");
  }
  if (argc == 0)
    return;

  for (int i = 0; i < cli_cmd_cnt; i++)
  {
    if (strcmp(argv[0], cli_tbl[i].name) == 0)
    {
      cli_tbl[i].func(argc, argv);
      return;
    }
  }
  cliPrintf("unknown command: %s\r\n", argv[0]);
}

uint32_t cliPrintf(const char *fmt, ...)
{
  char buf[128];
  va_list args;

  va_start(args, fmt);
  int len = vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  if (len <= 0)
    return 0;
  if (len >= ((int) sizeof(buf)) - 2)
    len = sizeof(buf) - 2;
  buf[len++] = '\r';
  buf[len++] = '\n';
  return uartWrite(HW_UART_CH_CLI, (uint8_t*) buf, (uint32_t) len);
}

bool cliCheck(const char *buf, const char *cmd)
{
  return strcmp(buf, cmd) == 0;
}
