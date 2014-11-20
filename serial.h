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

#ifndef serial_h
#define serial_h

#include <stdint.h>

#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 255
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 255
#endif

#define SERIAL_NO_DATA 0xff

volatile struct
{
  uint8_t     m_getIdx;
  uint8_t     m_putIdx;
  uint8_t     m_size;
  uint8_t     m_entry[RX_BUFFER_SIZE];
} RX_Buf;

volatile struct
{
  uint8_t     m_getIdx;
  uint8_t     m_putIdx;
  uint8_t     m_size;
  uint8_t     m_entry[TX_BUFFER_SIZE];
} TX_Buf;

void serial_init(void);

void serial_write(uint8_t data);

uint8_t serial_read(void);

// Reset and empty data in read buffer. Used by e-stop and reset.
void serial_reset_read_buffer(void);

uint8_t serial_get_rx_buffer_count(void);

#endif
