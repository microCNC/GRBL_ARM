/*
  eeprom.c
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

#include "system.h"
#include "eeprom.h"

static int WriteAddrValid(uint32_t addr){
  // check if address offset works for writing
  // must be 4-byte aligned

#ifdef FLASH_STORAGE
  return (((addr % 4) == 0) && (addr <= FLASH_FMA_OFFSET_MAX));
#else
  return (((addr % 4) == 0) && (addr <= EEPROM_OFFSET_MAX));
#endif
}

#ifdef FLASH_STORAGE

uint8_t Flash_Clear(uint32_t addr){

  uint32_t flash_addr = addr + FLASH_FMA_OFFSET;
  if(WriteAddrValid(flash_addr)){
    // An erase must be preformed first to turn all bits to 0 
    // a write will only turn necessary bits to 1
    __disable_irq(); // may be optional step
    
    // wait for hardware idle
    while(FLASH_CTRL->FMC&(FLASH_FMC_WRITE|FLASH_FMC_ERASE|FLASH_FMC_MERASE)){
                 // to do later: return ERROR if this takes too long
                 // remember to re-enable interrupts
    };
    
    FLASH_CTRL->FMA = flash_addr;
    FLASH_CTRL->FMC = (FLASH_FMC_WRKEY|FLASH_FMC_ERASE);// start erasing
    while(FLASH_CTRL->FMC&FLASH_FMC_ERASE){ }
    
    __enable_irq();
    return NOERROR;
  }
  return ERROR;
}

// Write 32-bit data to flash
// Return NOERROR if successful
uint8_t Flash_Write(uint32_t addr, uint32_t data){
  uint32_t flash_addr = addr + FLASH_FMA_OFFSET;
	
  if(WriteAddrValid(flash_addr)){
    __disable_irq(); // may be optional step
           // wait for hardware idle
    while(FLASH_CTRL->FMC&(FLASH_FMC_WRITE|FLASH_FMC_ERASE|FLASH_FMC_MERASE)){
                 // to do later: return ERROR if this takes too long
                 // remember to re-enable interrupts
    };
    
    FLASH_CTRL->FMD = data;
    FLASH_CTRL->FMA = flash_addr;
    FLASH_CTRL->FMC = (FLASH_FMC_WRKEY|FLASH_FMC_WRITE);// start writing
    while(FLASH_CTRL->FMC&FLASH_FMC_WRITE){
                 // to do later: return ERROR if this takes too long
                 // remember to re-enable interrupts
    };           // wait for completion (~3 to 4 usec)
    __enable_irq();
    return NOERROR;
  }
  return ERROR;
}

uint32_t Flash_Read(uint32_t addr) {
	uint32_t flash_addr = addr + FLASH_FMA_OFFSET;
	return (*((uint32_t *)flash_addr));
}

#else

uint32_t EEPROM_Read(uint32_t addr) {
  uint32_t eeprom_addr = addr + EEPROM_OFFSET;
  uint32_t value;
  uint16_t block = floor(eeprom_addr / 16);
  uint16_t offset = eeprom_addr - (block*16);

	while(EEPROM->EEDONE) {} // Wait for completion of previous write.
	
	if(EEPROM->EEBLOCK != block) {
	EEPROM->EEBLOCK = block; // Set EEPROM block address register.
	}
	EEPROM->EEOFFSET = offset; // Set EEPROM block address register.
	value = EEPROM->EERDWR; // Start EEPROM read operation.
	return value; // Return the byte read from EEPROM.
}

void EEPROM_Write(uint32_t addr, uint32_t new_value) {
  uint32_t eeprom_addr = addr + EEPROM_OFFSET;
  uint16_t block = floor(eeprom_addr / 16);
  uint16_t offset = eeprom_addr - (block*16);
  
  if(WriteAddrValid(eeprom_addr)){
  
	while(EEPROM->EEDONE) {} // Wait for completion of previous write.
	
	if(EEPROM->EEBLOCK != block) {
	EEPROM->EEBLOCK = block; // Set EEPROM block address register.
	}
	EEPROM->EEOFFSET = offset; // Set EEPROM block address register.
	EEPROM->EERDWR = new_value; // Start EEPROM read operation.
	}
}


#endif

void write_to_storage(uint32_t destination, char *source, uint32_t size) {
	
  uint16_t blocks = ceil(size / 4.0f); // convert in 4byte (32bit) blocks 
  uint16_t i;
  uint16_t ch = 0;
		
  for(i = 0; i < blocks; i++) {
    uint32_t word = (uint32_t)source[ch+3] << 24 | (uint32_t)source[ch+2] << 16 | (uint32_t)source[ch+1] << 8 | (uint32_t)source[ch];
#ifdef FLASH_STORAGE
    Flash_Write(destination+ch, word);
#else
    EEPROM_Write(destination+ch, word);
#endif			
    ch = ch+4;
  }
}

int8_t read_from_storage(char *destination, uint32_t source, uint32_t size) {
  uint32_t data;
  uint32_t add_block = floor(source);
  uint16_t blocks = ceil(size / 4.0f); // convert in 4byte (32bit) blocks 
  uint16_t i;
  uint8_t j;
	
  for(i = 0; i < blocks; i++) {
#ifdef FLASH_STORAGE
    data = Flash_Read(add_block+(i*4));
#else
    data = EEPROM_Read(add_block+(i*4));
#endif
			
    for(j = 0; j < 4; j++) {
      uint8_t shift = j*8;
      uint8_t chars = (data >> shift) & 0x000000FF;
      //if(chars == "/0") { break; } // exit is there is no more data
      *(destination++) = chars;
    }
  }
  return(1);
}

// end of file
