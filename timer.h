/*
  timer.h - Header for system level commands and real-time processes
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

#ifndef timer_h
#define timer_h

#include <inttypes.h>  

void timer_init(void);

// This is a one shot count down BLOCKING timer to create a delay in ms
void one_shot_delay(uint32_t delay);

void set_stepper_timer(uint16_t prescaler, uint32_t hertz);
void start_stepper_timer(void);
void stop_stepper_timer(void);
void set_stepper_reset_timer(uint16_t prescaler, uint32_t hertz);
void stop_stepper_reset_timer(void);
void set_pulse_delay_timer(uint16_t prescaler, uint32_t hertz);
void stop_pulse_delay_timer(void);
	
#endif
