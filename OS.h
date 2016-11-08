//////////////////////////////////////////////////////////////////////////////
///  @file OS.h
///  @copyright 2016 William R Cooke
///  @brief  Interface for aRdTOS simple Arduino RTOS
//////////////////////////////////////////////////////////////////////////////

#ifndef _OS_H_
#define _OS_H_

#include <Arduino.h>

typedef enum _OS_State_t
{
  // The first two states MUST remain in those positions
  // So the initialization macros work right
  STATE_INACTIVE = 0,   // Task is deactivated
  STATE_READY    = 1,   // Task is ready to run
  STATE_RUN,            // Task is currently running (not used)
  STATE_WAIT_TIME,      // Waiting for elapsed time

  STATE_MAX,        // highest number of state
  STATE_INVALID     // something's wrong
} OS_State_t;

typedef struct _Semaphore_t
{
  uint8_t count;
}Semaphore_t;



typedef void (*Task_p)();
typedef void (*ITask_p)();

typedef struct _Task_t
{
  Task_p fn;
  uint16_t wait;
  uint16_t wait_cycle;
  OS_State_t state;
  Semaphore_t *sem;
} Task_t;

typedef struct _ITask_t
{
  Task_p fn;
  uint16_t wait;
  uint16_t wait_cycle;
  OS_State_t state;
  Semaphore_t *sem;
} ITask_t;

#define OS_Setup() OS_Init(sizeof(TaskList)/sizeof(Task_t), sizeof(ITaskList)/sizeof(ITask_t))
#define TASKLIST Task_t TaskList[] = {
#define ENDLIST };
#define ITASKLIST ITask_t ITaskList[] = {
#define TASK(f,w,c,s)   { .fn=(f), .wait=(w), .wait_cycle = (c), .state = (s) },
#define ITASK(f,w,c,s)   { .fn=(f), .wait=(w), .wait_cycle = (c), .state = (s) },


int OS_Init(uint8_t tasks, uint8_t itasks);
int OS_Run(void);
void OS_Sleep(uint16_t ticks);
void OS_Deactivate();
void OS_DeactivateTask(uint8_t task);
void OS_ActivateTask(uint8_t task);
void OS_DeactivateITask(uint8_t task);
void OS_ActivateITask(uint8_t task);

extern uint32_t timer_ticks;
extern int itask_cnt;

#endif

