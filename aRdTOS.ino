///////////////////////////////////////////////////////////////////
///  @file aRdTOS.ino
///  @copyright 2016 William R Cooke
///
///  @brief implements simple RTOS
///////////////////////////////////////////////////////////////////

//#include "Timers.h"
#include "OS.h"

#define NUM_TASKS  2
#define NUM_ITASKS 8

typedef void (*Task_p)();
typedef void (*ITask_p)();


/*
int isr_count = 0;



void task1();
void task2();
Task_t t1 = {task1, true};
Task_t t2 = {task2, true};

Task_t a_tasks[NUM_TASKS]=
{
  t1,
  t2
};


ITask_t a_itasks[NUM_ITASKS];
int next_task = 0;
int next_itask = 0;
*/

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
 // itask_cnt++;
}


//Task_t TaskList[] = 
//{
TASKLIST
  TASK(task1, 10, 1000)
  TASK(task2, 0, 20)
  
ENDLIST

ITASKLIST
  ITASK(itask1, 0, 2000)
  ITASK(itask2, 0, 1000)
ENDLIST


//////////////////////////////////////////
///////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  Serial.println("setup()");
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  //OS_Init();
  OS_Setup();
}



void loop()
{
  Serial.println("loop()");
  //runTasks();
  OS_Run();
}

/*
ISR(TIMER1_OVF_vect)
{
  isr_count++;
}

/*
ISR(TIMER0_OVF_vect)
{
  isr_count++;
  
}
*/

/*
ISR(TIMER2_COMPB_vect)
{
  isr_count++;
}
*/


