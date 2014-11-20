/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2014 Robert Brown
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

#ifdef CPU_MAP_TIVA_LAUNCHPAD

  #include "include/TM4C123/TM4C123.h"

	// Board Specific Defines
	#define F_CPU			80000000UL
	
	//#define FLASH_STORAGE //If commented EEPROM will be used

  // Define serial port pins and interrupt vectors.
  #define SERIAL_PORT 			UART0

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
	#define STEP_PORT         GPIOB
	#define STEP_PORT_IRQN    GPIOB_IRQn
	#define STEP_VALUE				GPIOB->DATA
	#define STEP_DDR          GPIOB->DIR
	#define STEP_ENABLE				GPIOB->DEN
	#define STEP_POWER				GPIOB->DR8R
	#define X_STEP_BIT				5  // Tiva C Launchpad Pin 2
	#define Y_STEP_BIT				0  // Tiva C Launchpad Pin 3
	#define Z_STEP_BIT				1  // Tiva C Launchpad Pin 4
	#define STEP_MASK         ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
	#define DIRECTION_PORT				GPIOA
	#define DIRECTION_PORT_IRQN   GPIOA_IRQn
	#define DIRECTION_VALUE				GPIOA->DATA
	#define DIRECTION_DDR         GPIOA->DIR
	#define DIRECTION_ENABLE			GPIOA->DEN
	#define DIRECTION_POWER				GPIOA->DR8R
	#define X_DIRECTION_BIT				5  // Tiva C Launchpad Pin 8
	#define Y_DIRECTION_BIT				6  // Tiva C Launchpad Pin 9
	#define Z_DIRECTION_BIT				7  // Tiva C Launchpad Pin 10
	#define DIRECTION_MASK				((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

	// Define stepper driver enable/disable output pin.
	#define STEPPERS_DISABLE_PORT       GPIOE
	#define STEPPERS_DISABLE_PORT_IRQN  GPIOE_IRQn
	#define STEPPERS_DISABLE_VALUE			GPIOE->DATA
	#define STEPPERS_DISABLE_DDR        GPIOE->DIR
	#define STEPPERS_DISABLE_ENABLE			GPIOE->DEN
	#define STEPPERS_DISABLE_POWER			GPIOE->DR8R
	#define STEPPERS_DISABLE_BIT        4  // Tiva C Launchpad Pin 5
	#define STEPPERS_DISABLE_MASK       (1<<STEPPERS_DISABLE_BIT)

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (pinout).
	#define LIMIT_PORT        GPIOE
	#define LIMIT_PORT_IRQN		GPIOE_IRQn
	#define LIMIT_VALUE       GPIOE->DATA
	#define LIMIT_DDR         GPIOE->DIR
	#define LIMIT_PULLUP      GPIOE->PUR
	#define LIMIT_ENABLE      GPIOE->DEN
	#define LIMIT_INT_HANDLER	GPIOPortE_IRQHandler
	#define X_LIMIT_BIT       1  // Tiva C Launchpad Pin 17
	#define Y_LIMIT_BIT       2  // Tiva C Launchpad Pin 18
	#define Z_LIMIT_BIT       3  // Tiva C Launchpad Pin 19
	#define LIMIT_MASK        ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
	#define SPINDLE_ENABLE_PORT		GPIOE
	#define SPINDLE_ENABLE_IRQN  	GPIOE_IRQn
	#define SPINDLE_ENABLE_BIT		5  // Tiva C Launchpad Pin 6
	#define SPINDLE_ENABLE_MASK   (1<<SPINDLE_ENABLE_BIT)

	#define SPINDLE_DIRECTION_PORT  GPIOB
	#define SPINDLE_DIRECTION_IRQN  GPIOB_IRQn
  #define SPINDLE_DIRECTION_BIT   4  // Tiva C Launchpad Pin 7
	#define SPINDLE_DIRECTION_MASK  (1<<SPINDLE_DIRECTION_BIT)

  // Define coolant enable output pins.
	#define COOLANT_PORT					GPIOB
	#define COOLANT_IRQN					GPIOB_IRQn
  #define COOLANT_BIT						3  // Tiva C Launchpad Pin 23
	#define COOLANT_MASK					(1<<COOLANT_BIT)

  // Define user-control pinouts (cycle start, reset, feed hold) input pins.
  // NOTE: All pinouts pins must be on the same port and not on a port with other input pins (limits).

	#define PINOUT_PORT         GPIOC
	#define PINOUT_PORT_IRQN    GPIOC_IRQn
	#define PINOUT_VALUE        GPIOC->DATA
	#define PINOUT_DDR          GPIOC->DIR
	#define PINOUT_PULLUP       GPIOC->PUR
	#define PINOUT_ENABLE       GPIOC->DEN
	#define PINOUT_INT_HANDLER  GPIOPortC_IRQHandler
  #define PIN_RESET           4  // Tiva C Launchpad Pin 24
  #define PIN_FEED_HOLD       5  // Tiva C Launchpad Pin 25
  #define PIN_CYCLE_START     6  // Tiva C Launchpad Pin 26
  #define PINOUT_MASK         ((1<<PIN_RESET)|(1<<PIN_FEED_HOLD)|(1<<PIN_CYCLE_START))

	// Define probe switch input pin.
	#define PROBE_PORT					GPIOC
	#define PROBE_PORT_IRQN			GPIOC_IRQn
	#define PROBE_VALUE					GPIOC->DATA
	#define PROBE_DDR						GPIOC->DIR
	#define PROBE_PULLUP		  	GPIOC->PUR
	#define PROBE_ENABLE		  	GPIOC->DEN
	#define PROBE_BIT           7 // Tiva C Launchpad Pin 27
	#define PROBE_MASK          (1<<PROBE_BIT)

  #ifdef FLASH_STORAGE

    #define FLASH_FMC_WRKEY         0xA4420000  // FLASH write key
    #define FLASH_FMC_MERASE        0x00000004  // Mass Erase Flash Memory
    #define FLASH_FMC_ERASE         0x00000002  // Erase a Page of Flash Memory
    #define FLASH_FMC_WRITE         0x00000001  // Write a Word into Flash Memory
    #define FLASH_USECRL_M          0x000000FF  // Microsecond Reload Value mask
    #define ERROR                   1           // Value returned if failure
    #define NOERROR                 0           // Value returned if success
    #define FLASH_FMA_OFFSET_MAX    0x00040000  // Address Offset max
    #define FLASH_FMA_OFFSET	      0x00038600  // Address Offset

  #else

    #define EEPROM_OFFSET_MAX	      0x00040000  // Address Offset max
    #define EEPROM_OFFSET		        0x00000000  // Address Offset

  #endif

  #ifdef VARIABLE_SPINDLE
    // Advanced Configuration Below You should not need to touch these variables
    #define TCCRA_REGISTER    TCCR2A
    #define TCCRB_REGISTER    TCCR2B
    #define OCR_REGISTER      OCR2A

    #define COMB_BIT          COM2A1
    #define WAVE0_REGISTER    WGM20
    #define WAVE1_REGISTER    WGM21
    #define WAVE2_REGISTER    WGM22
    #define WAVE3_REGISTER    WGM23

    // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
    #define SPINDLE_PWM_DDR	  SPINDLE_ENABLE_DDR
    #define SPINDLE_PWM_PORT  SPINDLE_ENABLE_PORT
    #define SPINDLE_PWM_BIT	  SPINDLE_ENABLE_BIT // Shared with SPINDLE_ENABLE.
  #endif // End of VARIABLE_SPINDLE

#endif

#ifdef CPU_MAP_ELITE_M4F

  #include "include/TM4C123/TM4C123.h"

	// Board Specific Defines
	#define F_CPU			80000000UL
	
	//#define FLASH_STORAGE //If commented EEPROM will be used

  // Define serial port pins and interrupt vectors.
  #define SERIAL_PORT 			UART5

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
	#define STEP_PORT         GPIOC
	#define STEP_PORT_IRQN    GPIOC_IRQn
	#define STEP_VALUE				STEP_PORT->DATA
	#define STEP_DDR          STEP_PORT->DIR
	#define STEP_ENABLE				STEP_PORT->DEN
	#define STEP_POWER				STEP_PORT->DR8R
	#define X_STEP_BIT				4  // Elite M4F Digital Pin 2
	#define Y_STEP_BIT				5  // Elite M4F Digital Pin 3
	#define Z_STEP_BIT				6  // Elite M4F Digital Pin 4
	#define STEP_MASK         ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
	#define DIRECTION_PORT				GPIOH
	#define DIRECTION_PORT_IRQN   GPIOH_IRQn
	#define DIRECTION_VALUE				DIRECTION_PORT->DATA
	#define DIRECTION_DDR         DIRECTION_PORT->DIR
	#define DIRECTION_ENABLE			DIRECTION_PORT->DEN
	#define DIRECTION_POWER				DIRECTION_PORT->DR8R
	#define X_DIRECTION_BIT				3  // Elite M4F Digital Pin 5
	#define Y_DIRECTION_BIT				0  // Elite M4F Digital Pin 6
	#define Z_DIRECTION_BIT				1  // Elite M4F Digital Pin 7
	#define DIRECTION_MASK				((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

	// Define stepper driver enable/disable output pin.
	#define STEPPERS_DISABLE_PORT       GPIOH
	#define STEPPERS_DISABLE_PORT_IRQN  GPIOH_IRQn
	#define STEPPERS_DISABLE_VALUE			STEPPERS_DISABLE_PORT->DATA
	#define STEPPERS_DISABLE_DDR        STEPPERS_DISABLE_PORT->DIR
	#define STEPPERS_DISABLE_ENABLE			STEPPERS_DISABLE_PORT->DEN
	#define STEPPERS_DISABLE_POWER			STEPPERS_DISABLE_PORT->DR8R
	#define STEPPERS_DISABLE_BIT        4  // Elite M4F Digital Pin 8
	#define STEPPERS_DISABLE_MASK       (1<<STEPPERS_DISABLE_BIT)

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (pinout).
	#define LIMIT_PORT        GPIOH
	#define LIMIT_PORT_IRQN		GPIOH_IRQn
	#define LIMIT_VALUE       LIMIT_PORT->DATA
	#define LIMIT_DDR         LIMIT_PORT->DIR
	#define LIMIT_PULLUP      LIMIT_PORT->PUR
	#define LIMIT_ENABLE      LIMIT_PORT->DEN
	#define LIMIT_INT_HANDLER	GPIOPortH_IRQHandler
	#define X_LIMIT_BIT       5  // Elite M4F Digital Pin 9
	#define Y_LIMIT_BIT       6  // Elite M4F Digital Pin 10
	#define Z_LIMIT_BIT       7  // Elite M4F Digital Pin 11
	#define LIMIT_MASK        ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
	#define SPINDLE_ENABLE_PORT		GPIOH
	#define SPINDLE_ENABLE_IRQN  	GPIOH_IRQn
	#define SPINDLE_ENABLE_BIT		2  // Elite M4F Digital Pin 12
	#define SPINDLE_ENABLE_MASK   (1<<SPINDLE_ENABLE_BIT)

	#define SPINDLE_DIRECTION_PORT  GPIOH
	#define SPINDLE_DIRECTION_IRQN  GPIOH_IRQn
  #define SPINDLE_DIRECTION_BIT   3  // Elite M4F Digital Pin 13
	#define SPINDLE_DIRECTION_MASK  (1<<SPINDLE_DIRECTION_BIT)

  // Define coolant enable output pins.
	#define COOLANT_PORT					GPIOE
	#define COOLANT_IRQN					GPIOE_IRQn
  #define COOLANT_BIT						0  // Elite M4F Digital Pin 12
	#define COOLANT_MASK					(1<<COOLANT_BIT)

  // Define user-control pinouts (cycle start, reset, feed hold) input pins.
  // NOTE: All pinouts pins must be on the same port and not on a port with other input pins (limits).

	#define PINOUT_PORT         GPIOE
	#define PINOUT_PORT_IRQN    GPIOE_IRQn
	#define PINOUT_VALUE        PINOUT_PORT->DATA
	#define PINOUT_DDR          PINOUT_PORT->DIR
	#define PINOUT_PULLUP       PINOUT_PORT->PUR
	#define PINOUT_ENABLE       PINOUT_PORT->DEN
	#define PINOUT_INT_HANDLER  GPIOPortE_IRQHandler
  #define PIN_RESET           3  // Elite M4F Analog Pin 0
  #define PIN_FEED_HOLD       2  // Elite M4F Analog Pin 1
  #define PIN_CYCLE_START     1  // Elite M4F Analog Pin 2
  #define PINOUT_MASK         ((1<<PIN_RESET)|(1<<PIN_FEED_HOLD)|(1<<PIN_CYCLE_START))

	// Define probe switch input pin.
	#define PROBE_PORT					GPIOD
	#define PROBE_PORT_IRQN			GPIOD_IRQn
	#define PROBE_VALUE					PROBE_PORT->DATA
	#define PROBE_DDR						PROBE_PORT->DIR
	#define PROBE_PULLUP		  	PROBE_PORT->PUR
	#define PROBE_ENABLE		  	PROBE_PORT->DEN
	#define PROBE_INT_HANDLER   GPIOPortD_IRQHandler
	#define PROBE_BIT           6 // Uno Analog Pin 5
	#define PROBE_MASK          (1<<PROBE_BIT)

  #ifdef FLASH_STORAGE

    #define FLASH_FMC_WRKEY         0xA4420000  // FLASH write key
    #define FLASH_FMC_MERASE        0x00000004  // Mass Erase Flash Memory
    #define FLASH_FMC_ERASE         0x00000002  // Erase a Page of Flash Memory
    #define FLASH_FMC_WRITE         0x00000001  // Write a Word into Flash Memory
    #define FLASH_USECRL_M          0x000000FF  // Microsecond Reload Value mask
    #define ERROR                   1           // Value returned if failure
    #define NOERROR                 0           // Value returned if success
    #define FLASH_FMA_OFFSET_MAX    0x00040000  // Address Offset max
    #define FLASH_FMA_OFFSET	      0x00038600  // Address Offset

  #else

    #define EEPROM_OFFSET_MAX	      0x00040000  // Address Offset max
    #define EEPROM_OFFSET		        0x00000000  // Address Offset

  #endif
  
  #ifdef VARIABLE_SPINDLE
    // Advanced Configuration Below You should not need to touch these variables
    #define TCCRA_REGISTER    TCCR2A
    #define TCCRB_REGISTER    TCCR2B
    #define OCR_REGISTER      OCR2A

    #define COMB_BIT          COM2A1
    #define WAVE0_REGISTER    WGM20
    #define WAVE1_REGISTER    WGM21
    #define WAVE2_REGISTER    WGM22
    #define WAVE3_REGISTER    WGM23

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
