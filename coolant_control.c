/*
  coolant_control.c - coolant control methods
  Part of Grbl

  Copyright (c) 2014 Robert Brown
  Copyright (c) 2012-2014 Sungeun K. Jeon

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
#include "coolant_control.h"
#include "protocol.h"
#include "gcode.h"


void coolant_init(void)
{
		
	COOLANT_PORT->DIR |= COOLANT_MASK;			// make output pin
	COOLANT_PORT->AMSEL &= ~COOLANT_MASK;		// disable analog
  COOLANT_PORT->AFSEL &= ~COOLANT_MASK;		// disable alt funct
  COOLANT_PORT->DEN |= COOLANT_MASK;			// make digital pin
	
  coolant_stop();
}

void coolant_stop(void)
{
  COOLANT_PORT->DATA &= ~(1 << COOLANT_BIT);
}


void coolant_run(uint8_t mode)
{
  if (sys.state == STATE_CHECK_MODE) { return; }

  protocol_auto_cycle_start();   //temp fix for M8 lockup
  protocol_buffer_synchronize(); // Ensure coolant turns on when specified in program.
  if (mode == COOLANT_FLOOD_ENABLE) {
    COOLANT_PORT->DATA |= (1 << COOLANT_BIT);

  } else {
    coolant_stop();
  }
}
