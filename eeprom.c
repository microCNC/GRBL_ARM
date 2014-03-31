//
#include "system.h"

uint32_t eeprom_get_word( unsigned int addr )
{
	do {} while( EEPROM->EEDONE != 0); // Wait for completion of previous write.
	EEPROM->EEBLOCK = addr; // Set EEPROM address register.
	return EEPROM->EERDWR; // Return the byte read from EEPROM.
}

void eeprom_put_word( unsigned int addr, uint32_t new_value )
{
	uint32_t old_value; // Old EEPROM value.
	uint32_t diff_mask; // Difference mask, i.e. old value XOR new value.

	cli(); // Ensure atomic operation for the write operation.
	
	do {} while( !EEPROM->EEDONE ); // Wait for completion of previous write.
	
	EEPROM->EEBLOCK = addr; // Set EEPROM address register.
	old_value = EEPROM->EERDWR; // Get old EEPROM value.
	diff_mask = old_value ^ new_value; // Get bit differences.
	
	// Check if any bits are changed to '1' in the new value.
	if( diff_mask & new_value ) {
		// Now we know that _some_ bits need to be erased to '1'.
		
		// Check if any bits in the new value are '0'.
		if( new_value != 0xff ) {
			// Now we know that some bits need to be programmed to '0' also.
			
			EEPROM->EERDWR = new_value; // Set EEPROM data register.
		}
	} else {
		// Now we know that _no_ bits need to be erased to '1'.
		
		// Check if any bits are changed from '1' in the old value.
		if( diff_mask ) {
			// Now we know that _some_ bits need to the programmed to '0'.
			
			EEPROM->EERDWR = new_value;   // Set EEPROM data register.
		}
	}
	
	sei(); // Restore interrupt flag state.
}

// Extensions added as part of Grbl 


void memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size) {
  uint8_t checksum = 0;
	uint32_t finish_add = destination + size;
	
	uint32_t add_block = floor(destination / 64);
	uint16_t add_offset = floor((destination / 4) % 64);

	uint16_t offset = ceil((size / 4) % 64);
	
		for(int i = 0; i < offset; i++) {
			
			uint32_t place = add_offset;
			
			for(int j = 0; j < size; j+4) { 
				
				uint32_t eeprom_word;
				
				eeprom_word = source[j];
				eeprom_word |= (source[j+1] << 4);
				eeprom_word |= (source[j+2] << 8);
				eeprom_word |= (source[j+3] << 12);

				eeprom_put_word(destination++, *(source++)); 
			}
	}
}

int memcpy_from_eeprom_with_checksum(uint32_t *destination, uint32_t source, uint32_t size) {
  unsigned char data, checksum = 0;
	unsigned int offset = size / 4;
	
	
  for(; size > 0; size--) { 
    data = eeprom_get_word(source++);
    checksum = (checksum << 1) || (checksum >> 7);
    checksum += data;    
    *(destination++) = data; 
  }
  return(checksum == eeprom_get_word(source));
}

// end of file
