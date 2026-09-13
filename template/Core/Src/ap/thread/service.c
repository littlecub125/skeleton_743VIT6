/*
 * service.c
 *
 *  Created on: 2026. 9. 13.
 *      Author: Yubin Kim
 */

#include "service.h"
#include "fs90r.h"
#include "rc522.h"
#include "gpio.h"
#include "ssd1306.h"

//-- Definition
//
#define DOOR_ROTATE_TIME_MS  500
#define ITEM_REMOVE_CLOSE_DELAY_MS 500

//-- Functions
//
static void serviceSetDoor(bool status);
static void checkItemTaken(void);

//-- Variables 
//
static bool isDoorOpened = false;
static bool isInit = false;
static uint32_t doorStartTick = 0;
static bool isDoorMoving = false;
static bool itemPresentPrev = false;
static bool itemRemovePending = false;
static uint32_t itemRemovedTick = 0;

void serviceInit(void)
{
  isInit = true;
}

void serviceUpdate(void)
{
  if (isDoorMoving)
  {
    if (HAL_GetTick() - doorStartTick >= DOOR_ROTATE_TIME_MS)
    {
      fs90rCmdSet(FS90R_CMD_STOP);
      isDoorMoving = false;
      ssd1306SetStr(isDoorOpened ? "Door Open" : "Door is\nclosed");
    }
    return;   // 움직이는 동안엔 무조건 여기서 끝
  }

  if (isDoorOpened == false)
  {
    static uint8_t uid[4];
    uint8_t uid_len = 0;
    if (rc522IsCardPresent() && rc522ReadUid(uid, &uid_len))
    {
      ssd1306SetStr("Card Tagged");
      serviceSetDoor(true);
    }
  }
  else
  {
    checkItemTaken();
    if (itemRemovePending
        && (HAL_GetTick() - itemRemovedTick >= ITEM_REMOVE_CLOSE_DELAY_MS))
    {
      serviceSetDoor(false);
      itemRemovePending = false;
    }
  }
}

static void serviceSetDoor(bool status)
{
  fs90rCmdSet(status ? FS90R_CMD_CW_MOVE : FS90R_CMD_CCW_MOVE);
  doorStartTick = HAL_GetTick();
  ssd1306SetStr("Door is moving");
  isDoorMoving = true;
  isDoorOpened = status;
}

static void checkItemTaken(void)
{
  bool itemPresent = gpioGetInputStatus(HW_GPIO_PIN_I_SS5GL_SW);

  if (itemPresentPrev && !itemPresent)      // 있다가 방금 없어진 순간만
  {
    itemRemovePending = true;
    itemRemovedTick = HAL_GetTick();
    ssd1306SetStr("Item is\nremoved");      // 엣지에서 딱 한 번만
  }
  if (itemPresent)                           // 다시 놓이면 타이머 취소
  {
    itemRemovePending = false;
  }

  itemPresentPrev = itemPresent;
}
