/*
  probe.c - code pertaining to probing methods
  Part of Grbl

  Copyright (c) 2014 Sungeun K. Jeon

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
#include "probe.h"


// Probe pin initialization routine.
void probe_init() 
{
	SYSCTL->RCGCGPIO |= (1 << PROBE_PORT_IRQN);			// enable clock
	SYSCTL->GPIOHBCTL |= (1 << PROBE_PORT_IRQN);			// enable high performace bus
	
  PROBE_DDR &= ~(PROBE_MASK);				// Configure as input pins
	PROBE_ENABLE |= PROBE_MASK;       // Enable digital pin
  PROBE_PORT->PUR |= PROBE_MASK;		// Enable internal pull-up resistors. Normal high operation.
	
	//set interrupts
  PROBE_PORT->IS  &= ~PROBE_MASK;        // make bit edge sensitive
  PROBE_PORT->IBE &= ~PROBE_MASK;        // trigger is controlled by IEV
  PROBE_PORT->IEV &= ~PROBE_MASK;        // Falling edge trigger
  PROBE_PORT->ICR = PROBE_MASK;          // clear any prior interrupt
  PROBE_PORT->IM  |= PROBE_MASK;         // set interrupt on ports
	
	NVIC_SetPriority(PINOUT_PORT_IRQN, 12);				// set interrupt priority to 6
  NVIC_EnableIRQ(PINOUT_PORT_IRQN);							// enable IRQ
}


// Monitors probe pin state and records the system position when detected. Called by the
// stepper ISR per ISR tick.
// NOTE: This function must be extremely efficient as to not bog down the stepper ISR.
void probe_state_monitor()
{
  if (sys.probe_state == PROBE_ACTIVE) { 
    if (!(PROBE_VALUE & PROBE_MASK)) {
      sys.probe_state = PROBE_OFF;
      memcpy(sys.probe_position, sys.position, sizeof(float)*N_AXIS);
      sys.execute |= EXEC_FEED_HOLD;
    }
  }
}
