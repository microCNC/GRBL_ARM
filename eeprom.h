#ifndef eeprom_h
#define eeprom_h

#include <inttypes.h>

char eeprom_get_word(uint32_t addr);
void eeprom_put_word(uint32_t addr, uint8_t new_value);
void memcpy_to_eeprom_with_checksum(uint32_t destination, char *source, uint32_t size);
int8_t memcpy_from_eeprom_with_checksum(char *destination, uint32_t source, uint32_t size);

#endif
