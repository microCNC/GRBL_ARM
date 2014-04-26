//
#include "system.h"

uint32_t eeprom_get_word( uint32_t addr )
{
   uint32_t block;
   uint32_t offset;

	do {} while( EEPROM->EEDONE != 0); // Wait for completion of previous write.

   // convert address into block and offset
   block = floor(addr / 16);
   offset = (addr % 16);

   EEPROM->EEBLOCK = block; // Set EEPROM block address register.
   EEPROM->EEOFFSET = offset;

   return EEPROM->EERDWR; // Return the byte read from EEPROM.
}

void eeprom_put_word( uint32_t addr, uint32_t new_value )
{
   uint32_t block;
   uint32_t offset;

	do {} while( EEPROM->EEDONE != 0 ); // Wait for completion of previous write.
	
   // convert address into block and offset
   block = floor(addr / 16);
   offset = (addr % 16);

// need an if to check if the current block = the new block

   EEPROM->EEBLOCK = block; // Set EEPROM block address register.
   EEPROM->EEOFFSET = offset; // check register name
   EEPROM->EERDWR = new_value;   // Set EEPROM data register.
}

void memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size) {
	
	uint32_t add_block = floor(destination);
	uint16_t blocks = ceil(size / 4.0f); // convert in 4byte (32bit) blocks 
	uint16_t i;
	
		for(i = 0; i < blocks; i++) {
			uint8_t byte1 = *(source++);
			uint8_t byte2 = *(source++);
			uint8_t byte3 = *(source++);
			uint8_t byte4 = *(source++);
			
			uint32_t word = (byte1 << 24) & (byte2 << 16) & (byte3 << 8) & byte4;
			
			eeprom_put_word(add_block+i, word); 
		}
	}

int memcpy_from_eeprom_with_checksum(uint8_t *destination, uint32_t source, uint32_t size) {
  unsigned char data, checksum = 0;	
	
	uint32_t add_block = floor(source);
	uint16_t blocks = ceil(size / 4.0f); // convert in 4byte (32bit) blocks 
	uint16_t i;
	uint8_t j;
	
		for(i = 0; i < blocks; i++) {
			data = eeprom_get_word(add_block+i);
			
			for(j = 0; j < 4; j++) {
				uint8_t shift = i*8;
				uint8_t chars = (data >> shift) & 0x000000FF;
				checksum = (checksum << 1) || (checksum >> 7);
				checksum += chars;    
				*(destination++) = chars;
		}
	}
  return(checksum == eeprom_get_word(source));
}

// end of file
