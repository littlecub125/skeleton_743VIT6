/*
 * log.c
 *
 *  Created on: 2026. 9. 21.
 *      Author: Yubin Kim
 */

#include "log.h"
#include "info.h"
#include "sdmmc_diskio.h"
//-- Definition
//
#define LOG_LINE_MAX   128
#define LOG_BUF_MAX    1024


//-- Functions
//
static void cliLog(int argc, char *argv[]);
static void logClear(void);
//-- Variables 
//
static bool logShowEnable = false;
static char log_buf[LOG_BUF_MAX];      // 아직 SD에 안 쓴 로그
static uint32_t log_len = 0;
//static FIL log_file;
//static bool log_file_open = false;
void logInit(void)
{
  logShowEnable = false;
  // f_open(&log_file, "1:/log.txt", FA_OPEN_APPEND | FA_WRITE) — 카드 없으면 실패해도 그냥 넘어감
  cliAdd("log", cliLog);
}

void logAdd(const char *fmt, ...)
{
  char line[LOG_LINE_MAX];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(line, sizeof(line), fmt, args);
  va_end(args);

  if (len <= 0)
    return;
  if (len >= sizeof(line))
    len = sizeof(line) - 1;

  if (logShowEnable)
    cliPrintf("%s", line);

  if (log_len + len + 2 < LOG_BUF_MAX)
  {
    memcpy(&log_buf[log_len], line, len);
    log_len += len;
    log_buf[log_len++] = '\r';
    log_buf[log_len++] = '\n';
  }
}

void logUpdate(void)
{

}

static void logFlush(void)
{
  if (log_len == 0)
    return;

  FIL file;
  UINT bw;
  char path[16];
  snprintf(path, sizeof(path), "%slog.txt", SDPath);

  if (f_open(&file, path, FA_OPEN_APPEND | FA_WRITE) == FR_OK)
  {
    f_write(&file, log_buf, log_len, &bw);
    f_close(&file);
    log_len = 0;
  }
}

static void logClear(void)
{
  log_len = 0;                       // 1. 아직 SD에 안 쓴 것부터 버림

  FIL file;
  char path[16];
  snprintf(path, sizeof(path), "%slog.txt", SDPath);

  FRESULT fr = f_open(&file, path, FA_CREATE_ALWAYS | FA_WRITE); // 2. 0바이트로 새로 만듦
  if (fr == FR_OK)
  {
    f_close(&file);
    cliPrintf("log clear ok");
  }
  else
  {
    cliPrintf("log clear fail fr=%d", fr);
  }
}

void logShow(uint32_t tail_bytes)
{
  logFlush();

  FIL file;
  char path[16];
  snprintf(path, sizeof(path), "%slog.txt", SDPath);

  if (f_open(&file, path, FA_READ) != FR_OK)
  {
    cliPrintf("log open fail");
    return;
  }

  if (tail_bytes > 0 && f_size(&file) > tail_bytes)
    f_lseek(&file, f_size(&file) - tail_bytes);   // 끝부분만 보기

  char chunk[64];
  char line[LOG_LINE_MAX];
  uint32_t pos = 0;
  UINT br;

  while (f_read(&file, chunk, sizeof(chunk), &br) == FR_OK && br > 0)
  {
    for (UINT i = 0; i < br; i++)
    {
      if (chunk[i] == '\n')
      {
        line[pos] = 0;
        cliPrintf("%s", line);
        pos = 0;
      }
      else if (chunk[i] != '\r' && pos < sizeof(line) - 1)
      {
        line[pos++] = chunk[i];
      }
    }
  }
  f_close(&file);
}


static void cliLog(int argc, char *argv[])
{
  bool ret = false;

  if (argc == 2)
  {
    if (cliCheck(argv[1], "write"))
    {
      logAdd("test");
      ret = true;
    }
    else if (cliCheck(argv[1], "read"))
    {
      logShow(100);
      ret = true;
    }
    else if (cliCheck(argv[1], "clear"))
    {
      logClear();
      ret = true;
    }
  }
  else if (argc == 3)
  {
    if (cliCheck(argv[1], "show"))
    {
      if (cliCheck(argv[2], "enable"))
      {
        logShowEnable = true;
        cliPrintf("log show enable");
        ret = true;
      }
      else if (cliCheck(argv[2], "disable"))
      {
        logShowEnable = false;
        cliPrintf("log show disable");
        ret = true;
      }
    }
  }


  if (ret == false)
  {
    cliPrintf("log write");
    cliPrintf("log read");
    cliPrintf("log clear");
    cliPrintf("log show [enable|disable]");
  }
}

