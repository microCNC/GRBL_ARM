#  Part of Grbl_ARM
#
#  Copyright (c) 2014 Rob Brown
#
#  Grbl is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Grbl is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.


# TARGET: name of the output file
TARGET = GRBL

# MCU: part number to build for
# Options are:
# TM4C123GH6PM = Tiva C Launchpad
# TM4C123GH6PZ = Elite M4F
# ?? Coming Soon = Arduino Due
MCU = TM4C123GH6PM

# set compiler flags for the MCU
# Tiva C Launchpad configuration
ifeq ($(MCU),TM4C123GH6PM)
SERIES = TM4C123
MANUFACTURER = TI
# define compiler flags
CFLAGS = -g -mthumb -march=armv7e-m -mcpu=cortex-m4 -mtune=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard 
CFLAGS +=-Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall
CFLAGS +=-D$(MCU) -D$(MANUFACTURER) -DARM_MATH_CM4 -D__FPU_USED -D__FPU_PRESENT
endif
# Elite M4F configuration
ifeq ($(MCU),TM4C123GH6PZ)
SERIES = TM4C123
MANUFACTURER = TI
# define compiler flags
CFLAGS = -g -mthumb -march=armv7e-m -mcpu=cortex-m4 -mtune=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard 
CFLAGS +=-Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall
CFLAGS +=-D$(MCU) -D$(MANUFACTURER) -DARM_MATH_CM4 -D__FPU_USED -D__FPU_PRESENT
endif

# SOURCES: list of input source sources
SOURCES = main.c motion_control.c gcode.c spindle_control.c coolant_control.c serial.c \
             protocol.c timer.c stepper.c eeprom.c settings.c planner.c nuts_bolts.c limits.c \
             print.c probe.c report.c system.c
             
SYSTEM_FILE = include/$(SERIES)/system_$(SERIES).c
STARTUP_FILE = include/$(SERIES)/startup_$(SERIES).c

# INCLUDES: list of includes, by default, use Includes directory
INCLUDES = -Iinclude
# OUTDIR: directory to use for output
OUTDIR = build

# LD_SCRIPT: linker script
LD_SCRIPT = include/$(SERIES)/$(SERIES).ld

# define linker flags
LDFLAGS = -T$(LD_SCRIPT) --entry Reset_Handler $(CFLAGS) -nostartfiles -gc-sections -lm -lgcc -lc -Wl,-Map,$(SERIES).map

#######################################
# end of user configuration
#######################################
#
#######################################
# binaries
#######################################
AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
NM = arm-none-eabi-nm
RM = rm -f
MKDIR	= mkdir -p
FLASH = lm4flash
#######################################

# list of object files, placed in the build directory regardless of source path
OBJECTS = $(STARTUP_FILE:.c=.o)
OBJECTS += $(SYSTEM_FILE:.c=.o)
OBJECTS += $(addprefix $(OUTDIR)/,$(notdir $(SOURCES:.c=.o)))


# default: build bin
all: $(TARGET).bin

$(OUTDIR)/%.o: %.c | $(OUTDIR)
	$(CC) -o $@ $^ $(CFLAGS) -c
	$(CC) -o $(SYSTEM_FILE:.c=.o) $(SYSTEM_FILE) $(CFLAGS) -c
	$(CC) -o $(STARTUP_FILE:.c=.o) $(STARTUP_FILE) $(CFLAGS) -c

$(OUTDIR)/$(TARGET).elf: $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(TARGET).bin: $(OUTDIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $^ $@
	$(SIZE) -B -d -t --common $^
	$(NM) -s --size-sort -s --demangle=arm $^

# create the output directory
$(OUTDIR):
	$(MKDIR) $(OUTDIR)
	
clean:
	-$(RM) $(TARGET).bin
	-$(RM) $(OUTDIR)/*
	-$(RM) $(SYSTEM_FILE:.c=.o)
	-$(RM) $(SYSTEM_FILE:.c=.d)
	-$(RM) $(STARTUP_FILE:.c=.o)
	-$(RM) $(STARTUP_FILE:.c=.d)

#
# The rule to flash the program to the chip.
#
flash:
	$(FLASH) -E $(TARGET).bin

debug: clean
debug: CFLAGS+=-g -DDEBUG
debug: ${COMPILER}
debug: ${COMPILER} $(TARGET).bin
debug: 
	./debug_nemiver.sh

.PHONY: all clean
