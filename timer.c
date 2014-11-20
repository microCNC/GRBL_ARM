/*
  timer.c - Setup of Timers
  Part of Grbl
  
  Copyright (c) 2014 Rob Brown

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "system.h"

void timer_init(void)
{	
	
	// configure WTIMER0 A as 32bit timer with a prescaler of 80
	WTIMER0->CTL = 0x0202; // Disable A and B Timers
	WTIMER0->CFG = 0x04; //Set as 32bit timer
	WTIMER0->TAMR = 0x01; //Periodic Timer with interrupt
	WTIMER0->TAPR = 0x50; //Set 16bit prescaler to 80 ( 1us )
	WTIMER0->IMR = 0x00000001; // Set Interrupt Mask

	// configure Stepper Timer
	WTIMER1->CTL = 0x0202; // Disable A and B Timers
	WTIMER1->CFG = 0x04; //Set as 32bit timer
	WTIMER1->TAMR = 0x22; //Periodic Timer with interrupt
	WTIMER1->IMR = 0x00000001; // Set Interrupt Mask

	// configure Stepper Reset Timer 
	WTIMER2->CTL = 0x0202; // Disable A and B Timers
	WTIMER2->CFG = 0x04; //Set as 32bit timer
	WTIMER2->TAMR = 0x22; //Periodic Timer with interrupt	
	WTIMER2->IMR = 0x00000001; // Set Interrupt Mask

	// configure Step Pulse Delay Timer
	//WTIMER3->CTL = 0x0202; // Disable A and B Timers
	//WTIMER3->CFG = 0x04; //Set as 32bit timer
	//WTIMER3->TAMR = 0x22; //Periodic Timer with interrupt
	//WTIMER3->IMR = 0x00000001; // Set Interrupt Mask

  NVIC_SetPriority(WTIMER0A_IRQn, 7); // set interrupt priority to 7
	NVIC_SetPriority(WTIMER1A_IRQn, 3); // set interrupt priority to 3
	NVIC_SetPriority(WTIMER2A_IRQn, 2); // set interrupt priority to 3
	//NVIC_SetPriority(WTIMER3A_IRQn, 3); // set interrupt priority to 3

  NVIC_EnableIRQ(WTIMER0A_IRQn);      // enable IRQ
	NVIC_EnableIRQ(WTIMER1A_IRQn);      // enable IRQ
	NVIC_EnableIRQ(WTIMER2A_IRQn);      // enable IRQ
	//NVIC_EnableIRQ(WTIMER3A_IRQn);      // enable IRQ
}

void serial_set_timer(uint32_t delay) // delay in us
{
  if (WTIMER0->CTL&0x01) // if timer is already running return
  {
    return;
  } else {
    WTIMER0->TAILR = delay;
  }
}

void serial_start_timer(void)
{
  WTIMER0->CTL = WTIMER0->CTL | 0x01; // Enable A Timer
}
  
void serial_stop_timer(void)
{
  WTIMER0->CTL = WTIMER0->CTL & ~0x01; // Disable A Timer
}

// 32bit Stepper Timer
void set_stepper_timer(uint16_t prescaler, uint32_t ticks)
{
	WTIMER1->TAPR = prescaler;
	WTIMER1->TAILR = ticks;
}

void start_stepper_timer()
{
	WTIMER1->CTL = WTIMER1->CTL | 0x01; // Enable A Timer
}

void stop_stepper_timer()
{
	WTIMER1->CTL = WTIMER1->CTL & ~0x01; // Disable A Timer
}

// 32bit Stepper Reset Timer
void set_stepper_reset_timer(uint16_t prescaler, uint32_t ticks)
{
	WTIMER2->TAPR = prescaler;
	WTIMER2->TAILR = ticks;
}

void start_stepper_reset_timer()
{
	WTIMER2->CTL = WTIMER1->CTL | 0x01; // Enable A Timer
}

void stop_stepper_reset_timer()
{
	WTIMER2->CTL = WTIMER2->CTL & ~0x01; // Disable A Timer
}
