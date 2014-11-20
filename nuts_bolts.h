/*
  nuts_bolts.h - Header file for shared definitions, variables, and functions
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

#ifndef nuts_bolts_h
#define nuts_bolts_h

#include <inttypes.h>

#define false 0
#define true 1

#define N_AXIS 3 // Number of axes
#define X_AXIS 0 // Axis indexing value. Must start with 0 and be continuous.
#define Y_AXIS 1
#define Z_AXIS 2

#define MM_PER_INCH (25.40f)
#define INCH_PER_MM (0.0393701f)

#define TICKS_PER_MICROSECOND (F_CPU/1000000)

//LED macros
#define LEDOFF (GPIOF->DATA & ~0x0E)
#define LEDRED ((GPIOF->DATA & ~0x0E)|0x02)
#define LEDBLUE ((GPIOF->DATA & ~0x0E)|0x04)
#define LEDGREEN ((GPIOF->DATA & ~0x0E)|0x08)
#define LEDPURPLE ((GPIOF->DATA & ~0x0E)|0x06)
#define LEDSKYBLUE ((GPIOF->DATA & ~0x0E)|0x0C)
#define LEDYELLOW ((GPIOF->DATA & ~0x0E)|0x0A)
#define LEDWHITE ((GPIOF->DATA & ~0x0E)|0x0E)

#define PROGMEM
#define PGM_P  const char *
#define PSTR(str) (str)

typedef void prog_void;
typedef char prog_char;
typedef unsigned char prog_uchar;
typedef int8_t prog_int8_t;
typedef uint8_t prog_uint8_t;
typedef int16_t prog_int16_t;
typedef uint16_t prog_uint16_t;
typedef int32_t prog_int32_t;
typedef uint32_t prog_uint32_t;

#define strcpy_P(dest, src) strcpy((dest), (src))
#define strcat_P(dest, src) strcat((dest), (src))
#define strcmp_P(a, b) strcmp((a), (b))

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_float(addr) (*(const float *)(addr))

#define pgm_read_byte_near(addr) pgm_read_byte(addr)
#define pgm_read_word_near(addr) pgm_read_word(addr)
#define pgm_read_dword_near(addr) pgm_read_dword(addr)
#define pgm_read_float_near(addr) pgm_read_float(addr)
#define pgm_read_byte_far(addr) pgm_read_byte(addr)
#define pgm_read_word_far(addr) pgm_read_word(addr)
#define pgm_read_dword_far(addr) pgm_read_dword(addr)
#define pgm_read_float_far(addr) pgm_read_float(addr)

// Useful macros
#define clear_vector(a) memset(a, 0, sizeof(a))
#define clear_vector_float(a) memset(a, 0.0f, sizeof(float)*N_AXIS)
// #define clear_vector_long(a) memset(a, 0.0, sizeof(long)*N_AXIS)
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

// Bit field and masking macros
#define bit(n) (1 << n) 
#define bit_true_atomic(x,mask) {__disable_irq(); (x) |= (mask); __enable_irq(); }
#define bit_false_atomic(x,mask) {__disable_irq(); (x) &= ~(mask); __enable_irq(); }
#define bit_toggle_atomic(x,mask) {__disable_irq(); (x) ^= (mask); __enable_irq(); }
#define bit_true(x,mask) (x) |= (mask)
#define bit_false(x,mask) (x) &= ~(mask)
#define bit_istrue(x,mask) ((x & mask) != 0)
#define bit_isfalse(x,mask) ((x & mask) == 0)

#define atomic_set(x,y) {__disable_irq();(x = y);__enable_irq();}

//Math Macros
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// Read a floating point value from a string. Line points to the input buffer, char_counter 
// is the indexer pointing to the current character of the line, while float_ptr is 
// a pointer to the result variable. Returns true when it succeeds
uint8_t read_float(char *line, uint8_t *char_counter, float *float_ptr);

// Delays variable-defined milliseconds.
void delay_ms(uint32_t ms);

// Delays variable-defined microseconds.
void delay_us(uint32_t us);

uint8_t get_direction_mask(uint8_t i);

float hypot_f(float x, float y);

#endif
