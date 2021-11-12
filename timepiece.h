/**
 * @file timepiece.h
 * @brief 时间片轮询相关函数定义以及句柄
 * @author ZheWana
 * @date 2021/10/4 004
 */

#ifndef TIMEPIECE_H
#define TIMEPIECE_H

#include "Debug.h"
#include "stdio.h"

#define TaskNum(__taskArray) (sizeof(__taskArray)/sizeof(__taskArray[0]))

#define TaskListNumMAX 10

#define TaskHandlerDeclare(__handlerID) Task##__handlerID##Handler

static uint16_t ITperiod = 1;
static uint32_t globalTimer = 0;

//taskState:
typedef struct {
    uint8_t taskID;

    uint8_t taskShieldFlag: 1;
    uint8_t taskHangFlag: 1;

    uint16_t taskTimerLoad;
    int taskTimerDelay;

    uint16_t taskTimePiece;

    void (*taskHandler)(void);

    volatile uint16_t taskTimer;
} task_t, *ptask_t;

static void TaskRegister_Single(task_t *task);

static uint8_t Task_GetListLength(void);

static void TaskHandlerDeclare(backTask)(void);

static void Task_Hang(uint8_t taskID);

static void Task_DeHang(uint8_t taskID);

//For user

void Task_Init(task_t *regList, uint8_t listLength, uint16_t itPeriod);

void Task_Shield(uint8_t taskID);

void Task_DeShield(uint8_t taskID);

uint8_t Task_IsShield(uint8_t TaskID);

uint8_t Task_IsHang(uint8_t TaskID);

void Task_Delay(uint8_t taskID, int ms);

void Global_Delay(int ms);

void SetInterruptPeriod(uint16_t period_ms);

void TaskDicision(void);

void TaskRegister_List(task_t *listHead, uint8_t listLength);

#endif //TIMEPIECE_H
