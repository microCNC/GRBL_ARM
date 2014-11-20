/*
  system_TM4C123.c - System file for TM4C123 Microcontrollers
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

#include "system_TM4C123.h"
#include "TM4C123.h"
#include "../../system.h"

void SystemInit (void)
{
	int i;
	
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_USED)
    SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                   (3UL << 11*2));                 /* set CP11 Full Access */
  #endif

	// SET SYSTEM CLOCK TO 80MHZ
	
	SYSCTL->RCC = 0x78E3D50; // Clear MOSCDIS, set XTAL to 16MHz, set SYSDIV
	
	// With USESYSDIV cleared and BYPASS set the board should be running at 16MHz (internal oscillator)
  for (i = 0; i < 20572; i++)
	{ float f = 999.99f / 333.33f; } // floating point divide should take 14 clock cycles ( 875ns@16MHz )
	
	SYSCTL->RCC &= ~(1<<4);	// Use Main Oscillator
	SYSCTL->RCC2 = 0xC1004800;
	SYSCTL->RCC |= (1<<22);
	while (!(SYSCTL->RIS & 0x00000040)) {} // Wait for PLL to become ready.
	SYSCTL->RCC2 &= ~(1<<11);	
	while (!(SYSCTL->PLLSTAT)) {} // Ensure PLL is ready

//
// SETUP SYSTICK
//
  SysTick->CTRL = 0;				// disable SysTick during setup
  SysTick->LOAD = SysTick_VAL_CURRENT_Msk;	// maximum reload value
  SysTick->VAL = 0;				// any write to current clears it
						// enable SysTick with core clock
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk+SysTick_CTRL_CLKSOURCE_Msk;

#ifdef TM4C123GH6PM
  // SETUP GPIO
  SYSCTL->RCGCGPIO |= 0xFF;			// enable all port clocks
  while(!(SYSCTL->PRGPIO & 0xFF)) {};
  
  // SETUP WTIMERS
	SYSCTL->RCGCWTIMER = SYSCTL->RCGCWTIMER | 0x0F; // Turn WTimer0,1,2,3 clocks on
	while (!(SYSCTL->PRWTIMER & 0x0F)) {};
	
	// SETUP SERIAL PORT

  //Calculate BAUD RATE
  float baud_float = F_CPU / (16 * BAUD_RATE);
  uint16_t baud_int = floor(baud_float);
  uint8_t baud_frac = ((baud_float - baud_int) * 64 + 0.5f);  
  
  SYSCTL->RCGCUART |= 0x01;		// Enable UART0
    while(!(SYSCTL->PRUART & 0x01)) {};
	
	GPIOA->AFSEL |= 0x03;				// Enable Special Functions
	GPIOA->DEN |= 0x03;					// Enable Digital Pins		
	GPIOA->AMSEL &= ~0x03;      // disable analog functionality on PA
	GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00)+0x00000011;
			
	UART0->CTL = 0x0310;  //Disable UART0, Enable RX & TX
	
	// Set Baud Rate to 115200
	UART0->IBRD = baud_int; // IBRD = int(80,000,000 / (16 * 115,200)) = int(43.402777778)
	UART0->FBRD = baud_frac; // FBRD = int(0.402777778 * 64 + 0.5) = 26
	
	UART0->LCRH = 0x70; // Enable, WLEN = 8, FIFO Enabled
	UART0->IFLS = 0x24;
	UART0->IM = 0x70; // Enable Recieve, Trasmit and Recieve Timeout Interrupts
	UART0->CTL |= 0x01; //Enable UART0
	
	NVIC_SetPriority(UART0_IRQn, 6);  // set interrupt priority to 6
	NVIC_EnableIRQ(UART0_IRQn);       // enable IRQ


  // SETUP EEPROM
  //(*((volatile unsigned long *)0x400FE140)) = (80-1)&0x000000FF;
  
  SYSCTL->RCGCEEPROM |= 0x01;		// Enable EEPROM
  while(!(SYSCTL->PREEPROM & 0x01)) {};
  while(EEPROM->EEDONE&0x01) {}; // Wait for WORKING BIT of EEDONE to become clear


  GPIOF->DIR = 0x0E;
  GPIOF->DEN = 0x0E;

  GPIOF->DATA = ((GPIOF->DATA & ~0x0E)|0x06);

#endif

#ifdef TM4C123GH6PZ
  SYSCTL->RCGCGPIO |= 0x3FF;			// enable all port clocks
  while(!(SYSCTL->PRGPIO & 0x3FF)) {};

  // SETUP WTIMERS
	SYSCTL->RCGCWTIMER = SYSCTL->RCGCWTIMER | 0x0F; // Turn WTimer0,1,2,3 clocks on
	while (!(SYSCTL->PRWTIMER & 0x0F)) {};
	
	// SETUP SERIAL PORT

  //Calculate BAUD RATE
  float baud_float = F_CPU / (16 * BAUD_RATE);
  uint16_t baud_int = floor(baud_float);
  uint8_t baud_frac = ((baud_float - baud_int) * 64 + 0.5f);  
  
  SYSCTL->RCGCUART |= 0x20;		// Enable UART0
    while(!(SYSCTL->PRUART & 0x20)) {};
	
	GPIOE->DIR |= 0x20;				// Enable Special Functions
	GPIOE->AFSEL |= 0x20;				// Enable Special Functions
	GPIOE->DR8R |= 0x20;        // Set 8mA pin
	GPIOE->DEN |= 0x20;					// Enable Digital Pins		
	GPIOE->AMSEL &= ~0x20;      // disable analog functionality
	GPIOE->PCTL = (GPIOE->PCTL&0xFFF0FFFF)+0x00010000;
	
	GPIOJ->DIR &= ~0x02;				// Enable Special Functions
	GPIOJ->AFSEL |= 0x02;				// Enable Special Functions
	GPIOJ->DR8R |= 0x02;        // Set 8mA pin
	GPIOJ->DEN |= 0x02;					// Enable Digital Pins		
	GPIOJ->AMSEL &= ~0x02;      // disable analog functionality
	GPIOJ->PCTL = (GPIOJ->PCTL&0xFFFFFF0F)+0x00000010;
			
	UART5->CTL = 0x0310;  //Disable UART0, Enable RX & TX
	
	// Set Baud Rate to 115200
	UART5->IBRD = baud_int; // IBRD = int(80,000,000 / (16 * 115,200)) = int(43.402777778)
	UART5->FBRD = baud_frac; // FBRD = int(0.402777778 * 64 + 0.5) = 26
	
	UART5->LCRH = 0x70; // Enable, WLEN = 8, FIFO Enabled
	UART5->IFLS = 0x10;
	UART5->IM = 0x70; // Enable Recieve, Trasmit and Recieve Timeout Interrupts
	UART5->CTL |= 0x01; //Enable UART0
	
	NVIC_SetPriority(UART5_IRQn, 6);  // set interrupt priority to 6
	NVIC_EnableIRQ(UART5_IRQn);       // enable IRQ
	
	// SETUP EEPROM
  //(*((volatile unsigned long *)0x400FE140)) = (80-1)&0x000000FF;
  
  SYSCTL->RCGCEEPROM |= 0x01;		// Enable EEPROM
  while(!(SYSCTL->PREEPROM & 0x01)) {};
  while(EEPROM->EEDONE&0x01) {}; // Wait for WORKING BIT of EEDONE to become clear
  
  
#endif
}

