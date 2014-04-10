/*
  timer.c - An embedded CNC Controller with rs274/ngc (g-code) support
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
	// SETUP WTIMER0
	SYSCTL->RCGCWTIMER = SYSCTL->RCGCWTIMER | 0x3F; // Turn WTimer0 clocks on
	while (SYSCTL->PRWTIMER != 0x3F);
	
	// configure WTIMER0 A as 32bit timer for one_shot_delay() function
	WTIMER0->CTL = 0x00; // Disable A Timer
	WTIMER0->CFG = 0x04; //Set as 32bit timer
	WTIMER0->TAMR = 0x01; //Count Down, One Shot
	WTIMER0->TAPR = 0x50; //Set 16bit prescaler to 80
}

void one_shot_delay(uint32_t delay)
{	
	WTIMER0->TAILR = delay;
	WTIMER0->CTL = WTIMER0->CTL | 0x01; // Enable A Timer
		while (WTIMER0->CTL != 0x00);
}

// 32bit Stepper Timer
void set_stepper_timer(uint16_t prescaler, uint32_t time)
{
	
}

// 16bit Stepper Reset Timer
void set_stepper_reset_timer()
{
	
}

// 16bit Pulse Delay Timer
void set_pulse_delay_timer()
{
	
}
