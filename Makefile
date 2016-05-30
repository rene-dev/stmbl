# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#
OPT = -O2 -flto
#OPT = -ggdb3 # for debugging

# Object files directory
# Warning: this will be removed by make clean!
#
OBJDIR = obj_app

# Target file name (without extension)
TARGET = $(OBJDIR)/stmbl

# Define all C source files (dependencies are generated automatically)
#INCDIRS += inc
INCDIRS += shared

SOURCES += src/main.c
SOURCES += src/scanf.c
#SOURCES += src/setup.c
SOURCES += src/cdc.c
SOURCES += src/hal.c
SOURCES += src/misc.c
#SOURCES += src/eeprom.c
#SOURCES += src/link.c
SOURCES += src/version.c
SOURCES += src/syscalls.c

SOURCES += shared/crc8.c
SOURCES += shared/common.c
SOURCES += shared/ringbuf.c

LDFLAGS += -lopencm3_stm32f3 -Llibopencm3/lib/ -nostartfiles
CFLAGS += -DSTM32F3
INCDIRS += libopencm3/include

LDSCRIPT = stm32_flash.ld

#============================================================================
OBJECTS += $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES))))
CPPFLAGS += $(addprefix -I,$(INCDIRS))

#---------------- Preprocessor Options ----------------
#  -fsingle...    make better use of the single-precision FPU
#  -g             generate debugging information
#  -save-temps    preserve .s and .i-files
#
CPPFLAGS += -fsingle-precision-constant
# CPPFLAGS += -save-temps=obj

#---------------- C Compiler Options ----------------
#  -O*            optimization level
#  -f...          tuning, see GCC documentation
#  -Wall...       warning level
#
CFLAGS += $(OPT)
CFLAGS += -std=gnu11
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -Wall
CFLAGS += -fno-builtin ## from old
CFLAGS += -nostartfiles
CFLAGS += -Wfatal-errors
#CFLAGS += -Wstrict-prototypes
#CFLAGS += -Wextra
#CFLAGS += -Wpointer-arith
#CFLAGS += -Winline
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wundef

# Use a friendly C dialect
CPPFLAGS += -fno-strict-aliasing
CPPFLAGS += -fwrapv

#---------------- C++ Compiler Options ----------------
#
CXXFLAGS += $(OPT)
CXXFLAGS += -ffunction-sections
CXXFLAGS += -fdata-sections
CXXFLAGS += -Wall

#---------------- Assembler Options ----------------
#  -Wa,...    tell GCC to pass this to the assembler
#

#---------------- Linker Options ----------------
#  -Wl,...      tell GCC to pass this to linker
#  -Map         create map file
#  --cref       add cross reference to  map file
#
LDFLAGS += $(OPT)
LDFLAGS += -lm
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += -Wl,--gc-sections

# LDFLAGS += -specs=nano.specs -u _printf_float -u _scanf_float
LDFLAGS += -T$(LDSCRIPT)

#============================================================================

# Define programs and commands
#
TOOLCHAIN = arm-none-eabi-
CC       = $(TOOLCHAIN)gcc
OBJCOPY  = $(TOOLCHAIN)objcopy
OBJDUMP  = $(TOOLCHAIN)objdump
SIZE     = $(TOOLCHAIN)size
NM       = $(TOOLCHAIN)nm
MKDIR    = mkdir
POSTLD   = tools/add_version_info.py # -q

# Compiler flags to generate dependency files
#
GENDEPFLAGS = -MMD -MP

# Combine all necessary flags and optional flags
# Add target processor to flags.
#
CPU = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

CFLAGS   += $(CPU)
CXXFLAGS += $(CPU)
ASFLAGS  += $(CPU)
LDFLAGS  += $(CPU)

# Default target
#
all: gccversion build showsize

build: elf hex bin lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
bin: $(TARGET).bin
lss: $(TARGET).lss
sym: $(TARGET).sym

boot:
	$(MAKE) -f bootloader/Makefile

boot_clean:
	$(MAKE) -f bootloader/Makefile clean

boot_flash: boot
	$(MAKE) -f bootloader/Makefile flash
	
flash: $(TARGET).bin
	st-flash --reset write $(TARGET).bin 0x08000000

boot_btflash: boot
	$(MAKE) -f bootloader/Makefile btflash


# Display compiler version information
#
gccversion:
	@$(CC) --version

# Show the final program size
#
showsize: build
	@echo
	@$(SIZE) $(TARGET).elf 2>/dev/null

# Flash the device
#
btburn: build showsize $(TARGET).dfu
	#change this to your device
	printf "bootloader\r" > `ls /dev/cu.usbmodem*` || true
	printf "bootloader\r" > `ls /dev/ttyACM*` || true
	sleep 1
	dfu-util -a 0 -s 0x08010000:leave -D $(TARGET).dfu

# Create a DFU file from bin file
%.dfu: %.bin
	@cp $< $@
	dfu-suffix -v 0483 -p df11 -a $@

# Target: clean project
#
clean:
	@echo Cleaning project:
	rm -rf $(OBJDIR)
	@$(MAKE) -f bootloader/Makefile clean

# Include the base rules
#
include base.mak

# Include the dependency files
#
-include $(OBJECTS:.o=.d)

# Listing of phony targets
#
.PHONY: all build flash clean \
        boot boot_clean boot_flash btburn boot_btflash boot_flash\
        elf lss sym \
        showsize gccversion
