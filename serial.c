/*
  serial.c - Low level functions for sending and recieving bytes via the serial port
  Part of Grbl

  Copyright (c) 2011-2014 Sungeun K. Jeon
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

/* This code was initially inspired by the wiring_serial module by David A. Mellis which
   used to be a part of the Arduino project. */ 

#include "system.h"
#include "serial.h"
#include "motion_control.h"
#include "protocol.h"


uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_buffer_head = 0;
volatile uint8_t rx_buffer_tail = 0;

uint8_t tx_buffer[TX_BUFFER_SIZE];
uint8_t tx_buffer_head = 0;
volatile uint8_t tx_buffer_tail = 0;

void serial_init()
{
  SYSCTL->RCGCUART |= 0x01;		// Enable UART0
	SYSCTL->RCGCGPIO |= 0x01;		// Enable clock PORTA
	SYSCTL->GPIOHBCTL |= 0x01;	// Enable HB for PORTA
	
	GPIOA->AFSEL |= 0x03;				// Enable Special Functions
	GPIOA->DEN |= 0x03;					// Enable Digital Pins
	GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00)+0x00000011;
	
	UART0->CTL = 0x300; //Disable UART0, Enable RX & TX
	
	// Set Baud Rate to 115200
	UART0->IBRD = 0x2B; // IBRD = int(80,000,000 / (16 * 115,200)) = int(43.402777778)
	UART0->FBRD = 0x1A; // FBRD = int(0.402777778 * 64 + 0.5) = 26
	
	UART0->LCRH = 0x60; // Enable, WLEN = 8, FIFO Disabled
	UART0->IFLS = 0x03;
	UART0->DMACTL = 0x03;
	UART0->CTL |= 0x01; //Enable UART0
	
	NVIC_SetPriority(PINOUT_PORT_IRQN, 12);				// set interrupt priority to 12
  NVIC_EnableIRQ(UART0_IRQn);										// enable IRQ
	
	UART0->IM |= 0x30;
}


void serial_write(uint8_t data) {
  // Calculate next head
  uint8_t next_head = tx_buffer_head + 1;
  if (next_head == TX_BUFFER_SIZE) { next_head = 0; }

  // Wait until there is space in the buffer
  while (next_head == tx_buffer_tail) { 
    if (sys.execute & EXEC_RESET) { return; } // Only check for abort to avoid an endless loop.
  }

  // Store data and advance head
  tx_buffer[tx_buffer_head] = data;
  tx_buffer_head = next_head;
  						
}


// Data Register Empty Interrupt handler
void UART0_Handler(void)
{
	if (UART0->RIS == 0x10) {
			
		uint8_t data = UART0->DR;
		uint8_t next_head;
		
		UART0->ICR = 0x10; // Clear Interrupt
		
		// Pick off runtime command characters directly from the serial stream. These characters are
		// not passed into the buffer, but these set system state flag bits for runtime execution.
		switch (data) {
			case CMD_STATUS_REPORT: sys.execute |= EXEC_STATUS_REPORT; break; // Set as true
			case CMD_CYCLE_START:   sys.execute |= EXEC_CYCLE_START; break; // Set as true
			case CMD_FEED_HOLD:     sys.execute |= EXEC_FEED_HOLD; break; // Set as true
			case CMD_RESET:         mc_reset(); break; // Call motion control reset routine.
			default: // Write character to buffer    
				next_head = rx_buffer_head + 1;
				if (next_head == RX_BUFFER_SIZE) { next_head = 0; }
			
				// Write data to buffer unless it is full.
				if (next_head != rx_buffer_tail) {
					rx_buffer[rx_buffer_head] = data;
					rx_buffer_head = next_head;    
					
				}
		}
	} 
	if (UART0->RIS == 0x20)
	{
		volatile uint8_t tail = tx_buffer_tail; // Temporary tx_buffer_tail (to optimize for volatile)
		
		{ 
			// Send a byte from the buffer	
			UART0->DR = tx_buffer[tail];
		
			// Update tail position
			tail++;
			if (tail == TX_BUFFER_SIZE) { tail = 0; }
		
			tx_buffer_tail = tail;
		}
		
		// Turn off Data Register Empty Interrupt to stop tx-streaming if this concludes the transfer
		if (tail == tx_buffer_head) {
				UART0->ICR = 0x20; // Clear Interrupt
		}
	}
}

uint8_t serial_read()
{
  uint8_t tail = rx_buffer_tail; // Temporary rx_buffer_tail (to optimize for volatile)
  if (rx_buffer_head == tail) {
    return SERIAL_NO_DATA;
  } else {
    uint8_t data = rx_buffer[tail];
    
    tail++;
    if (tail == RX_BUFFER_SIZE) { tail = 0; }
    rx_buffer_tail = tail;
    
    return data;
  }
}


void serial_reset_read_buffer() 
{
  rx_buffer_tail = rx_buffer_head;
}
