/*
 * fs90r.c
 *
 *  Created on: 2026. 8. 30.
 *      Author: Yubin Kim
 */

#include "fs90r.h"
#include "pwm.h"
#include "info.h"
//-- Definition
//
#define FS90R_FREQ_STOP       1500
#define FS90R_FREQ_CW_MOVE    900
#define FS90R_FREQ_CCW_MOVE   2100

//-- Functions
//
static void cliFs90r(int argc, char *argv[]);

//-- Variables 
//
static uint32_t freq_set = 0;

bool fs90rInit(void)
{
  bool ret = true;
  freq_set = FS90R_FREQ_STOP;

  pwmOpen(HW_PWM_CH_MOTOR_1, 0);

  cliAdd("fs90r", cliFs90r);
  return ret;
}

bool fs90rCmdSet(FS90R_Cmd_t cmd)
{
  bool ret = true;

  switch (cmd)
  {
    case FS90R_CMD_STOP:
      freq_set = FS90R_FREQ_STOP;
      break;
    case FS90R_CMD_CW_MOVE:
      freq_set = FS90R_FREQ_CW_MOVE;
      break;
    case FS90R_CMD_CCW_MOVE:
      freq_set = FS90R_FREQ_CCW_MOVE;
      break;
    default:
      break;
  }


  return ret;
}


static void cliFs90r(int argc, char *argv[])
{
  bool ret = false;

  if (argc == 2)
  {
    if (cliCheck(argv[1], "read"))
    {
      uint32_t read_frea = 0;
      for (int i = 0; i < HW_PWM_CH_MOTOR_1; i++)
      {
        read_frea = pwmRead(i);
        cliPrintf("%d\n", read_frea);
      }

      ret = true;
    }
  }
  else if (argc == 4)
  {
    if (cliCheck(argv[1], "write"))
    {
      uint8_t idx = strtoul(argv[2], NULL, 0);
      uint32_t freq = strtoul(argv[3], NULL, 0);
      pwmWrite(idx, freq);
      ret = true;
    }
  }

  if (ret == false)
  {
    cliPrintf("fs90r read");
    cliPrintf("fs90r write [idx] [freq]");
  }
}


