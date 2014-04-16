# Tiva Makefile
# #####################################
#
# Part of the uCtools project
# uctools.github.com
#
#######################################
# user configuration:
#######################################
# TARGET: name of the output file
TARGET = GRBL

# MCU: part number to build for
MCU = TM4C123GH6PM

# set compiler flags for the MCU
ifeq ($(MCU),TM4C123GH6PM)
SERIES = TM4C123
# define compiler flags
CFLAGS = -g -mthumb -march=armv7e-m -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard 
CFLAGS +=-Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall 
CFLAGS +=-D$(MCU) -DARM_MATH_CM4 
endif

# SOURCES: list of input source sources
SOURCES = main.c motion_control.c gcode.c spindle_control.c coolant_control.c serial.c \
             protocol.c timer.c stepper.c eeprom.c settings.c planner.c nuts_bolts.c limits.c \
             print.c probe.c report.c system.c
             
SYSTEM_FILE = include/$(SERIES)/system_$(SERIES).c
STARTUP_FILE = include/$(SERIES)/startup_$(SERIES).c

LIB_GCC_PATH=${shell ${CC} ${CFLAGS} -print-libgcc-file-name}
LIBC_PATH=${shell ${CC} ${CFLAGS} -print-file-name=libc.a}
LIBM_PATH=${shell ${CC} ${CFLAGS} -print-file-name=libm.a}

# INCLUDES: list of includes, by default, use Includes directory
INCLUDES = -Iinclude
# OUTDIR: directory to use for output
OUTDIR = build

# LD_SCRIPT: linker script
LD_SCRIPT = include/$(SERIES)/$(SERIES).ld

# define linker flags
LDFLAGS = -T $(LD_SCRIPT) $(LIBM_PATH) $(LIBC_PATH) $(LIB_GCC_PATH) --entry rst_handler $(CFLAGS) -nostartfiles

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
RM      = rm -f
MKDIR	= mkdir -p
#######################################

# list of object files, placed in the build directory regardless of source path
OBJECTS = $(addprefix $(OUTDIR)/,$(notdir $(SOURCES:.c=.o)))
OBJECTS += $(SYSTEM_FILE:.c=.o)
OBJECTS += $(STARTUP_FILE:.c=.o)

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

# create the output directory
$(OUTDIR):
	$(MKDIR) $(OUTDIR)
	
clean:
	-$(RM) $(OUTDIR)/*
	-$(RM) $(SYSTEM_FILE:.c=.o)
	-$(RM) $(STARTUP_FILE:.c=.o)

.PHONY: all clean
