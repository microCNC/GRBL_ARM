/*
  TM4C123.ld - Linker Script for the TI TM4C123 Microcontrollers

  Copyright (c) 2014 Robert Brown
  
  This Linker Script is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This Linker Script is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
/*

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
/* Internal Memory Map*/
MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
  STACK (rwx) : ORIGIN = 0x20007FFF , LENGTH = 0x00000000
}

_eram = 0x20000000 + 0x00008000;
SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text*)
		*(.rodata*)
		__text_end__ = .;
	} >FLASH 
	
	.data :
	{
	  __data_start__ = .;
	  
		*(vtable)
		*(.data*)
		
		. = ALIGN(4);
		/* All data end */
		__data_end__ = .;
	} >RAM AT >FLASH
	
	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
	} >RAM 
	
	.heap (COPY):
	{
		__end__ = .;
		*(.heap*)
		__HeapLimit = .;
	} >RAM
		
/*
* .stack contains nothing, but I use it to set the first vector item (SP R13).
*/
  .stack :
  {
    __stack_top__ = .; /* An index to the end of the stack */
  } >STACK
}
