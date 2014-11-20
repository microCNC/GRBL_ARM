/*
  coolant_control.c - coolant control methods
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

#ifndef eeprom_h
#define eeprom_h

#include <inttypes.h>
#include "cpu_map.h"

#ifdef FLASH_STORAGE
uint8_t Flash_Clear(uint32_t addr);
uint8_t Flash_Write(uint32_t addr, uint32_t data);
uint32_t Flash_Read(uint32_t addr);
#else
uint32_t EEPROM_Read(uint32_t addr);
void EEPROM_Write(uint32_t addr, uint32_t new_value);
#endif
void write_to_storage(uint32_t destination, char *source, uint32_t size);
int8_t read_from_storage(char *destination, uint32_t source, uint32_t size);

#endif
