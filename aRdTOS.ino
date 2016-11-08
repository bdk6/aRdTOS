///////////////////////////////////////////////////////////////////
///  @file aRdTOS.ino
///  @copyright 2016 William R Cooke
///
///  @brief implements simple RTOS
///////////////////////////////////////////////////////////////////

#include "OS.h"

#define NUM_TASKS  2
#define NUM_ITASKS 8

//typedef void (*Task_p)();
//typedef void (*ITask_p)();

///////////////////////////////////////
///////////////////////////////////////

void task1()
{
  static int count = 0;
  Serial.print("Task 1: timer_ticks=");
  Serial.println(timer_ticks);
}
void task2()
{
  Serial.print("Task 2 itask_cnt=");
  Serial.println(itask_cnt);
  delay(500);
}

void itask1()
{
  static uint8_t on = 0;
  static uint8_t cycles = 6;

  if(--cycles == 0)
  {
    OS_ActivateITask(1);
    OS_Deactivate();
  }
  if(on == 0)
  {
    on = 1;
    digitalWrite(12, HIGH);
  }
  else
  {
    on = 0;
    digitalWrite(12, LOW);
  }
  itask_cnt++;
}
void itask2()
{
  static uint8_t on = 0;

  if(on == 0)
  {
    on = 1;
    digitalWrite(13, HIGH);
  }
  else
  {
    on = 0;
    digitalWrite(13, LOW);
  }
}


TASKLIST
  TASK(task1, 10, 1000, 0)
  TASK(task2, 0, 20, 1)
  
ENDLIST

ITASKLIST
  ITASK(itask1, 0, 2000, 1)
  ITASK(itask2, 0, 1000, 0)
ENDLIST


//////////////////////////////////////////
///////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  Serial.println("setup()");
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  OS_Setup();
}



void loop()
{
  Serial.println("loop()");
  OS_Run();
}



