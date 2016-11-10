//////////////////////////////////////////////////////////////////////////////
///  @file OS.cpp
///  @copyright 2016 William R Cooke
///  @brief  Implementation of aRdTOS simple Arduino RTOS
///  @version 0.0.1
//////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include "OS.h"

static uint8_t num_tasks = 0;    // Will hold number of tasks after init
static uint8_t num_itasks = 0;   // Will hold number of itasks after init
static uint8_t current_task;     // holds number of current task execution
static uint8_t current_itask;    // holds number of current itask execution
static uint8_t running_itasks = 0;   // are we running itasks?

uint32_t timer_ticks = 0; // counts all the timer ticks that happen
int itask_cnt = 0;

extern Task_t TaskList[];
extern ITask_t ITaskList[];

//////////////////////////////////////////////////////////////////////////////
///  @function run_tasks
///  @brief Cycles through all the Tasks in order
//////////////////////////////////////////////////////////////////////////////
static void run_tasks()
{
  uint8_t idx;

  for(idx=0; idx < num_tasks; idx++)
  {
    if(TaskList[idx].state == STATE_READY)
    {
      if(TaskList[idx].wait > 0)
      {
        TaskList[idx].wait--;
      }
      if(TaskList[idx].wait == 0)
      {
        current_task = idx;   // save teh executing task number
        TaskList[idx].wait = TaskList[idx].wait_cycle; // reset timer
        TaskList[idx].fn();  // and run it
      }
    }
    
  }
  
}

//////////////////////////////////////////////////////////////////////////////
///  @function run_itasks
///  @brief Cycles through all the ITasks in order
//////////////////////////////////////////////////////////////////////////////
static void run_itasks()
{
  uint8_t idx;
  running_itasks = 1;   // Tell everyone we are running itasks now
  
  for(idx=0; idx<num_itasks; idx++)
  {
    if(ITaskList[idx].state == STATE_READY)
    {
      if(ITaskList[idx].wait > 0)
      {
        ITaskList[idx].wait--;
      }
      if(ITaskList[idx].wait == 0)
      {
        current_itask = idx;   // save the executing task number
        ITaskList[idx].wait = ITaskList[idx].wait_cycle; // reset timer
        ITaskList[idx].fn();  // and run it
      }
    }
  }
  running_itasks = 0;   // Tell them we aren't
}

//////////////////////////////////////////////////////////////////////////////
///  @function OS_Init
///  @brief Initialized the OS, getting ready to run
///
///  @param[in] uint8_t tasks: number of tasks defined
///  @param[in] uint8_t itasks: number of itasks defined
///  @return a status code, 0 = success, <0 = error
//////////////////////////////////////////////////////////////////////////////
int OS_Init(uint8_t tasks, uint8_t itasks)
{
  int rtn = 0;

  // set task counts
  num_tasks = tasks;
  num_itasks = itasks;
 
  // setup timer
  // Only works on 328p (168?, 88?)
  // TODO:  Must create macros for others (see wiring.c)
  OCR0A = 20;  // just get away from 0 a bit
  TIMSK0 |= 2;  // enable the compare A interrupt
  

  // init task states?

  return rtn;
}

//////////////////////////////////////////////////////////////////////////////
///  @function OS_Run
///  @brief Transfers control to the OS
///
///  @return a status code, 0 = success, <0 = error.  Should never return
//////////////////////////////////////////////////////////////////////////////
int OS_Run()
{
  int rtn = 0;

  while(1)
  {
    run_tasks();
  }

  return rtn;
}


//////////////////////////////////////////////////////////////////////////////
///  @function OS_Sleep
///  @brief Puts current task to sleep
///
///  @param[in] uint16_t ticks: Number of ticks to sleep
//////////////////////////////////////////////////////////////////////////////
void OS_Sleep(uint16_t ticks)
{
  if(running_itasks == 0)   // regular tasks
  {
    TaskList[current_task].wait = ticks;
  }
  else    // interrupt tasks
  {
    ITaskList[current_itask].wait = ticks;
  }
}


//////////////////////////////////////////////////////////////////////////////
///  @function OS_Deactivate
///  @brief Deactivates current task
///
//////////////////////////////////////////////////////////////////////////////
void OS_Deactivate()
{
  if(running_itasks == 0)   // regular tasks
  {
    TaskList[current_task].state = STATE_INACTIVE;
  }
  else    // interrupt tasks
  {
    ITaskList[current_itask].state = STATE_INACTIVE;
  }
}

//////////////////////////////////////////////////////////////////////////////
///  @function OS_DeactivateTask
///  @brief Deactivates a regular task
///  
///  @param[in] uint8_t task: The regular task to deactivate
//////////////////////////////////////////////////////////////////////////////
void OS_DeactivateTask(uint8_t task)
{
  if(task < num_tasks)
  {
    TaskList[task].state = STATE_INACTIVE;
  }
}
//////////////////////////////////////////////////////////////////////////////
///  @function OS_DeactivateITask
///  @brief Deactivates an itask
///  
///  @param[in] uint8_t task: The itask to deactivate
//////////////////////////////////////////////////////////////////////////////
void OS_DeactivateITask(uint8_t task)
{
  if(task < num_itasks)
  {
    ITaskList[task].state = STATE_INACTIVE;
  }
}

//////////////////////////////////////////////////////////////////////////////
///  @function OS_ActivateTask
///  @brief Activates a regular task
///  
///  @param[in] uint8_t task: The regular task to activate
//////////////////////////////////////////////////////////////////////////////
void OS_ActivateTask(uint8_t task)
{
  if(task < num_tasks)
  {
    TaskList[task].state = STATE_READY;
  }
}

//////////////////////////////////////////////////////////////////////////////
///  @function OS_ActivateITask
///  @brief Activates an itask.
///  
///  @param[in] uint8_t task: The itask to deactivate
//////////////////////////////////////////////////////////////////////////////
void OS_ActivateITask(uint8_t task)
{
  if(task < num_itasks)
  {
    ITaskList[task].state = STATE_READY;
  }
}

//////////////////////////////////////////////////////////////////////////////
///  @function ISR
///  @brief Tick timer ISR, executes on timer 0 match, sames as millis counter
//////////////////////////////////////////////////////////////////////////////

// TODO:  get conditional code for different uCs

ISR(TIMER0_COMPA_vect)
{
  timer_ticks++;
  run_itasks();
}


