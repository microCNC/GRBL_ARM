/*
  spindle_control.c - spindle control methods
  Part of Grbl

  Copyright (c) 2014 Robert Brown
  Copyright (c) 2012-2014 Sungeun K. Jeon
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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
#include "spindle_control.h"
#include "protocol.h"
#include "gcode.h"


void spindle_init()
{  
	
	SYSCTL->RCGCGPIO |= (1 << SPINDLE_ENABLE_IRQN);			// enable clock
	while((SYSCTL->PRGPIO & (1 << SPINDLE_ENABLE_IRQN)) == 0) {};
		
	SPINDLE_ENABLE_PORT->DIR |= SPINDLE_ENABLE_MASK;			// make output pin
	SPINDLE_ENABLE_PORT->AMSEL &= ~SPINDLE_ENABLE_MASK;		// disable analog
  SPINDLE_ENABLE_PORT->AFSEL &= ~SPINDLE_ENABLE_MASK;		// disable alt funct
  SPINDLE_ENABLE_PORT->DEN |= SPINDLE_ENABLE_MASK;			// make digital pin
	
	SYSCTL->RCGCGPIO |= (1 << SPINDLE_DIRECTION_IRQN);			// enable clock
	while((SYSCTL->PRGPIO & (1 << SPINDLE_DIRECTION_IRQN)) == 0) {};
		
	SPINDLE_DIRECTION_PORT->DIR |= SPINDLE_DIRECTION_MASK;			// make output pin
	SPINDLE_DIRECTION_PORT->AMSEL &= ~SPINDLE_DIRECTION_MASK;		// disable analog
  SPINDLE_DIRECTION_PORT->AFSEL &= ~SPINDLE_DIRECTION_MASK;		// disable alt funct
  SPINDLE_DIRECTION_PORT->DEN |= SPINDLE_DIRECTION_MASK;			// make digital pin

  spindle_stop();
}


void spindle_stop()
{
    SPINDLE_ENABLE_PORT->DATA &= ~SPINDLE_ENABLE_MASK;

}


void spindle_run(uint8_t direction, float rpm) 
{
  // Empty planner buffer to ensure spindle is set when programmed.
  protocol_buffer_synchronize(); 

  // Halt or set spindle direction and rpm. 
  if (direction == SPINDLE_DISABLE) {
  
    spindle_stop();
  
  } else {
  
    if (direction == SPINDLE_ENABLE_CW) {
      SPINDLE_DIRECTION_PORT->DATA &= ~SPINDLE_DIRECTION_MASK;
    } else {
      SPINDLE_DIRECTION_PORT->DATA |= SPINDLE_DIRECTION_MASK;
    }

    #ifdef VARIABLE_SPINDLE
      #define SPINDLE_RPM_RANGE (SPINDLE_MAX_RPM-SPINDLE_MIN_RPM)
      TCCRA_REGISTER = (1<<COMB_BIT) | (1<<WAVE1_REGISTER) | (1<<WAVE0_REGISTER);
      TCCRB_REGISTER = (TCCRB_REGISTER & 0b11111000) | 0x02; // set to 1/8 Prescaler
      rpm -= SPINDLE_MIN_RPM;
      if ( rpm > SPINDLE_RPM_RANGE ) { rpm = SPINDLE_RPM_RANGE; } // Prevent uint8 overflow
      uint8_t current_pwm = floor( rpm*(255.0/SPINDLE_RPM_RANGE) + 0.5);
      OCR_REGISTER = current_pwm;
      
      #ifndef CPU_MAP_ATMEGA328P // On the Uno, spindle enable and PWM are shared.
        SPINDLE_ENABLE_PORT->DATA |= SPINDLE_ENABLE_MASK;
      #endif
    #else   
      SPINDLE_ENABLE_PORT->DATA |= SPINDLE_ENABLE_MASK;
    #endif

  }
}
