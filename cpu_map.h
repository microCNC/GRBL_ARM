/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2013-2014 Sungeun K. Jeon

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

/* The cpu_map.h file serves as a central pin mapping settings file for different processor
   types, i.e. AVR 328p or AVR Mega 2560. Grbl officially supports the Arduino Uno, but the 
   other supplied pin mappings are supplied by users, so your results may vary. */

// NOTE: This is still a work in progress. We are still centralizing the configurations to
// this file, so your success may vary for other CPUs.

#ifndef cpu_map_h
#define cpu_map_h

//----------------------------------------------------------------------------------------

#ifdef CPU_MAP_TM4C123GH6PM

  #include "include/TI/TM4C123/TM4C123GH6PM.h"
	
	// Board Specific Defines
	#define F_CPU			80000000UL	

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_TX			USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  #define STEPPING_DDR    GPIOE->DIR
  #define STEPPING_PORT   GPIOE->DATA
  #define X_STEP_BIT      1  // Uno Digital Pin 2
  #define Y_STEP_BIT      2  // Uno Digital Pin 3
  #define Z_STEP_BIT      3  // Uno Digital Pin 4
  #define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  #define DIRECTION_DDR     GPIOD->DIR
  #define DIRECTION_PORT    GPIOD->DATA
  #define X_DIRECTION_BIT   1  // Uno Digital Pin 5
  #define Y_DIRECTION_BIT   2  // Uno Digital Pin 6
  #define Z_DIRECTION_BIT   3  // Uno Digital Pin 7
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
  #define STEPPERS_DISABLE_DDR    GPIOF->DIR
  #define STEPPERS_DISABLE_PORT   GPIOF->DATA
  #define STEPPERS_DISABLE_BIT    1  // Uno Digital Pin 8
  #define STEPPERS_DISABLE_MASK   (1<<STEPPERS_DISABLE_BIT)

  // Define homing/hard limit switch input pins and limit interrupt vectors. 
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (pinout).
  #define LIMIT_PORT       GPIOA
	#define LIMIT_PORT_IRQN  GPIOA_IRQn
	#define LIMIT_VALUE			 LIMIT_PORT->DATA
	#define LIMIT_DDR				 LIMIT_PORT->DIR
	#define LIMIT_PULLUP		 LIMIT_PORT->PUR
	#define LIMIT_ENABLE		 LIMIT_PORT->DEN
	#define LIMIT_INT_HANDLER   GPIOA_Handler
  #define X_LIMIT_BIT      5  // Uno Digital Pin 9
  #define Y_LIMIT_BIT      6  // Uno Digital Pin 10
  #define Z_LIMIT_BIT			 7  // Uno Digital Pin 11
  #define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
  #define SPINDLE_ENABLE_DDR    GPIOB->DIR
  #define SPINDLE_ENABLE_PORT   GPIOB->DATA
  #define SPINDLE_ENABLE_BIT    0  // Uno Digital Pin 12
  
  #define SPINDLE_DIRECTION_DDR   GPIOB->DIR
  #define SPINDLE_DIRECTION_PORT  GPIOB->DATA
  #define SPINDLE_DIRECTION_BIT   1  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)

  // Define flood and mist coolant enable output pins.
  // NOTE: Uno analog pins 4 and 5 are reserved for an i2c interface, and may be installed at
  // a later date if flash and memory space allows.
  #define COOLANT_FLOOD_DDR   GPIOA->DIR
  #define COOLANT_FLOOD_PORT  GPIOA->DATA
  #define COOLANT_FLOOD_BIT   6  // Uno Analog Pin 3
	
  #ifdef ENABLE_M7 // Mist coolant disabled by default. See config.h to enable/disable.
    #define COOLANT_MIST_DDR   GPIOA->DIR
    #define COOLANT_MIST_PORT  GPIOA->DATA
    #define COOLANT_MIST_BIT   7 // Uno Analog Pin 4
  #endif  

  // Define user-control pinouts (cycle start, reset, feed hold) input pins.
  // NOTE: All pinouts pins must be on the same port and not on a port with other input pins (limits).
	
	#define PINOUT_PORT       GPIOC
	#define PINOUT_PORT_IRQN  GPIOC_IRQn
	#define PINOUT_VALUE			PINOUT_PORT->DATA
	#define PINOUT_DDR				PINOUT_PORT->DIR
	#define PINOUT_PULLUP		  PINOUT_PORT->PUR
	#define PINOUT_ENABLE		  PINOUT_PORT->DEN
	#define PINOUT_INT_HANDLER   GPIOC_Handler
  #define PIN_RESET        0  // Uno Analog Pin 0
  #define PIN_FEED_HOLD    1  // Uno Analog Pin 1
  #define PIN_CYCLE_START  2  // Uno Analog Pin 2
  #define PINOUT_MASK ((1<<PIN_RESET)|(1<<PIN_FEED_HOLD)|(1<<PIN_CYCLE_START))				
  
  #ifdef VARIABLE_SPINDLE
    // Advanced Configuration Below You should not need to touch these variables
    #define TCCRA_REGISTER	 TCCR2A
    #define TCCRB_REGISTER	 TCCR2B
    #define OCR_REGISTER     OCR2A

    #define COMB_BIT	     COM2A1
    #define WAVE0_REGISTER	 WGM20
    #define WAVE1_REGISTER	 WGM21
    #define WAVE2_REGISTER	 WGM22
    #define WAVE3_REGISTER	 WGM23

    // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
    #define SPINDLE_PWM_DDR	  SPINDLE_ENABLE_DDR
    #define SPINDLE_PWM_PORT  SPINDLE_ENABLE_PORT
    #define SPINDLE_PWM_BIT	  SPINDLE_ENABLE_BIT // Shared with SPINDLE_ENABLE.
  #endif // End of VARIABLE_SPINDLE

#endif

/* 
#ifdef CPU_MAP_CUSTOM_PROC
  // For a custom pin map or different processor, copy and paste one of the default cpu map
  // settings above and modify it to your needs. Then, make sure the defined name is also
  // changed in the config.h file.
#endif
*/

#endif
