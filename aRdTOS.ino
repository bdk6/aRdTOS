///////////////////////////////////////////////////////////////////
///  @file aRdTOS.ino
///  @copyright 2016 William R Cooke
///
///  @brief implements simple RTOS
///////////////////////////////////////////////////////////////////

#include "OS.h"

///////////////////////////////////////////////////////////////////
//  Two regular task definitions
///////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////
//  Two Interrupt Task definitions
///////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////
//  the two task tables are the 
//  main structure of your program
//  The task parameters are:
//  name:  name of the function to use as this task
//  start:  starting time in ticks
//  cycle:  the number of ticks between runs
//  active:  1 if task should run, 0 if paused
/////////////////////////////////////////////////////

TASKLIST
  TASK(task1, 10, 1000, 0)
  TASK(task2, 0, 20, 1)
  
ENDLIST

ITASKLIST
  ITASK(itask1, 0, 2000, 1)
  ITASK(itask2, 0, 1000, 0)
ENDLIST


//////////////////////////////////////////
//  The typical setup() with OS_Setup()
//////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  Serial.println("setup()");
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  OS_Setup();
}

//////////////////////////////////////////
//   loop() will include OS_Run()
//   which then takes over control
//////////////////////////////////////////

void loop()
{
  Serial.println("loop()");
  OS_Run();
}



