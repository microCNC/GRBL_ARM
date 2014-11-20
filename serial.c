/*
  serial.c - Low level functions for sending and recieving bytes via the serial port
  Part of Grbl

  Copyright (c) 2014 Robert Brown
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
#include "fifo.h"

void serial_init()
{ 
  FIFO_Init(RX_Buf,RX_BUFFER_SIZE);
  FIFO_Init(TX_Buf,RX_BUFFER_SIZE);
}

void serial_write(uint8_t data) {
  
  if ((UART0->FR&0x80) && (FIFO_IsEmpty(TX_Buf))) {
    UART0->DR = data;
    return;
  } else if(!FIFO_IsFull(TX_Buf) && !(UART0->FR&0x20)) {
    FIFO_Push(TX_Buf, data);
  } else {
    while(FIFO_IsFull(TX_Buf)) {}; // Loop until FIFO has Space then write data
    FIFO_Push(TX_Buf, data);
  }

}

void UART0_IRQHandler()
{  
	if (UART0->RIS&0x10) {
	UART0->ICR = 0x10; // Clear Interrupt
		
		while (!(UART0->FR&0x10)) {
		uint8_t data = UART0->DR&0xFF;
		
		// Pick off runtime command characters directly from the serial stream. These characters are
		// not passed into the buffer, but these set system state flag bits for runtime execution.
		switch (data) {
			case CMD_STATUS_REPORT: bit_true_atomic(sys.execute, EXEC_STATUS_REPORT); break; // Set as true
			case CMD_CYCLE_START:   bit_true_atomic(sys.execute, EXEC_CYCLE_START); break; // Set as true
			case CMD_FEED_HOLD:     bit_true_atomic(sys.execute, EXEC_FEED_HOLD); break; // Set as true
			case CMD_RESET:         mc_reset(); break; // Call motion control reset routine.
			default: // Write character to buffer  
			
			  FIFO_Push(RX_Buf, data);

			}
		}
		
	}

if (UART0->RIS&0x40) {
		
		UART0->ICR = 0x40; // Clear Interrupt
		
		while (!(UART0->FR&0x10)) {
		uint8_t data = UART0->DR&0xFF;
		
		// Pick off runtime command characters directly from the serial stream. These characters are
		// not passed into the buffer, but these set system state flag bits for runtime execution.
		switch (data) {
			case CMD_STATUS_REPORT: bit_true_atomic(sys.execute, EXEC_STATUS_REPORT); break; // Set as true
			case CMD_CYCLE_START:   bit_true_atomic(sys.execute, EXEC_CYCLE_START); break; // Set as true
			case CMD_FEED_HOLD:     bit_true_atomic(sys.execute, EXEC_FEED_HOLD); break; // Set as true
			case CMD_RESET:         mc_reset(); break; // Call motion control reset routine.
			default: // Write character to buffer    
				
				FIFO_Push(RX_Buf, data);
				
			}
		}
		
	}	
	
if (UART0->RIS&0x20) { // This Interrupt is called when all data has left the FIFO
  UART0->ICR = 0x20; // Clear Interrupt
  while (!(FIFO_IsEmpty(TX_Buf)) && !(UART0->FR&0x20))
  {
    UART0->DR = FIFO_Pop(TX_Buf);
  }
}
  
}

uint8_t serial_read()
{
  
  if (FIFO_IsEmpty(RX_Buf)) {
    return SERIAL_NO_DATA;
  } else {
    return FIFO_Pop(RX_Buf);
  }
}


void serial_reset_read_buffer() 
{
  FIFO_Init(RX_Buf,RX_BUFFER_SIZE);
}

uint8_t serial_get_rx_buffer_count()
{
  return FIFO_Len(RX_Buf);
}
