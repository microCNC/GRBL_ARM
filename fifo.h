/*
  fifo.h - Header file for serial fifo macros
  Part of Grbl

  Copyright (c) 2014 Robert Brown

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

#if !defined(FIFO_H)
#define FIFO_H

/** 
*   Initializes the circular buffer for use.
*/ 

#define FIFO_Init(buffer,size)       {buffer.m_getIdx = buffer.m_putIdx = 0;buffer.m_size = size;}

/**
*   Returns the number of elements which are currently
*   contained in the circular buffer.
*/

#define FIFO_Len(buffer)        ((uint8_t)((buffer.m_putIdx) - (buffer.m_getIdx)))

/**
*   Appends an element to the end of the circular buffer. The
*   element is expected to be of the same type as the @c m_entry
*   member.
*/

#define FIFO_Push(buffer, elem) {(buffer.m_entry)[(buffer.m_putIdx++) % buffer.m_size] = (elem);}

/**
*   Retrieves an element from the beginning of the circular buffer
*/

#define FIFO_Pop(buffer)        (buffer.m_entry)[(buffer.m_getIdx++) % buffer.m_size]

/**
*   Determines if the circular buffer is empty.
*/

#define FIFO_IsEmpty(buffer)    (FIFO_Len(buffer) == 0)

/**
*   Determines if the circular buffer is full.
*/

#define FIFO_IsFull(buffer)     (FIFO_Len(buffer) == (buffer.m_size))

/**
*   Determines if the circular buffer is currenly overflowed or underflowed.
*/

#define FIFO_Error(buffer)      (FIFO_Len(buffer) > buffer.m_size)

#endif // FIFO_H
