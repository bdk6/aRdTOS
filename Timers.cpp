


#include <Arduino.h>
#include "Timers.h"

volatile uint16_t f_count = 0;
static volatile uint32_t f;

void Timers_Init(void)
{

  TCCR1A = 0;   // [ com0a1 com0a0 com0b1 com0b0 res res wgm1 wgm0]
  TCCR1B = 0;   // [ icnc1 ices1 r wgm13 wgm12 cs02 cs01 cs00 ] 
  TCCR1C = 0;   // [ foc
  TCNT1H = 0;   // clear the counter
  TCNT1L = 0;
  TIMSK1 = 0;   // Timer int mask [r r r r r OCIE0B OCIE0A TOIE0] overflow
  //OCR0B = 0;   // output compare instead of overflow
  OCR1BH = 0;
  OCR1BL = 0;
  OCR1AH = 0;
  OCR1AL = 0;

  OCR0A = 20;  // just get away from 0 a bit
  TIMSK0 |= 2;  // enable the compare A interrupt
  
}

uint32_t Timers_readCount()
{
  uint32_t rtn = 0;
  TCCR1B = 0;    // no clock
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1H = 0;    // clear the count
  TCNT1L = 0;
  ICR1H = 0;
  ICR1L = 0;
  f_count = 0;
  f=0;
  interrupts();
  TIMSK1 = 1;
  TCCR1B = 7;  // external clock
  delay(195);  // wait 200 mS
  //delayMicroseconds(100000);
  TCCR1B = 0;  // turn off clock
  noInterrupts();
  //return f;
  //rtn = f_count;// << 16;
  rtn = f << 16;
  rtn += TCNT1L;
  rtn += (uint32_t)TCNT1H << 8;
  interrupts();
  return rtn*5;
}


