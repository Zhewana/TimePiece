/**
 * @file timepiece.c
 * @brief 时间片轮询函数实现
 * @author ZheWana
 * @date 2021/10/4 004
 */

#include "timepiece.h"

static ptask_t taskList[TaskListNumMAX] = {NULL};

static task_t backTask =
        {0, 0, 0, 0, 0, 0, TaskHandlerDeclare(backTask), 0};

static void TaskRegister_Single(task_t *task) {
    taskList[task->taskID] = task;
}

static uint8_t Task_GetListLength(void) {
    uint8_t taskCounter = 0;
    for (int i = 0; i < TaskListNumMAX; i++) {
        if (taskList[i] != NULL)taskCounter++;
    }
    return taskCounter;
}

static void TaskHandlerDeclare(backTask)(void) {
    for (int i = 0; i < TaskNum(taskList); i++) {
        if (Task_IsHang(i)) {
            if (taskList[i]->taskTimerDelay > ITperiod) {
                taskList[i]->taskTimerDelay -= ITperiod;
            } else {
                Task_DeHang(i);
                taskList[i]->taskTimerDelay = 0;
            }
        }
    }
}

static void Task_Hang(uint8_t taskID) {
    taskList[taskID]->taskHangFlag = 1;
}

static void Task_DeHang(uint8_t taskID) {
    taskList[taskID]->taskHangFlag = 0;
}

//For user

void Task_Init(task_t *regList, uint8_t listLength, uint16_t itPeriod) {
    SetInterruptPeriod(itPeriod);
    TaskRegister_Single(&backTask);
    TaskRegister_List(regList, listLength);
}

void Task_Shield(uint8_t taskID) {
    taskList[taskID]->taskShieldFlag = 1;
}

void Task_DeShield(uint8_t taskID) {
    taskList[taskID]->taskShieldFlag = 0;
}

uint8_t Task_IsShield(uint8_t TaskID) {
    return taskList[TaskID]->taskShieldFlag;
}

uint8_t Task_IsHang(uint8_t TaskID) {
    return taskList[TaskID]->taskHangFlag;
}

void Task_Delay(uint8_t taskID, int ms) {
    if (taskList[taskID]->taskTimerDelay == 0) {
        taskList[taskID]->taskTimerDelay = ms;
        taskList[taskID]->taskTimerLoad = taskList[taskID]->taskTimer;
        Task_Hang(taskID);
    }
}

void Global_Delay(int ms) {
    for (int i = 0; i < TaskNum(taskList); i++) {
        Task_Delay(i, ms);
    }
}

void SetInterruptPeriod(uint16_t period_ms) {
    ITperiod = period_ms;
}

void TaskDicision(void) {
    globalTimer += ITperiod;
    for (int i = 0; i < TaskNum(taskList); i++) {
        if (taskList[i] != NULL) {
            taskList[i]->taskTimer += ITperiod;
            if (taskList[i]->taskTimer >= taskList[i]->taskTimePiece) {
                taskList[i]->taskTimer = 0;
                if (taskList[i]->taskHandler != NULL &&
                    !taskList[i]->taskShieldFlag &&
                    !taskList[i]->taskHangFlag) {
                    taskList[i]->taskHandler();
                }
            }
        }
    }
}

void TaskRegister_List(task_t *listHead, uint8_t listLength) {
    for (int i = 0; i < listLength; i++)
        TaskRegister_Single(listHead + i);
}