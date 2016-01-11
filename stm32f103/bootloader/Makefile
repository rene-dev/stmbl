# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#
OPT = -O1 -flto
# OPT = -O1         # for debugging

# Object files directory
# Warning: this will be removed by make clean!
#
OBJDIR = obj_hv_boot

# Target file name (without extension)
TARGET = $(OBJDIR)/hv_boot

# Define all C source files (dependencies are generated automatically)
INCDIRS += stm32f103/bootloader/inc
INCDIRS += shared

SOURCES += stm32f103/bootloader/src/main.c
SOURCES += stm32f103/bootloader/src/version.c
SOURCES += shared/common.c

# Standard peripheral library
CPPFLAGS += -DUSE_STDPERIPH_DRIVER
#CPPFLAGS += -DUSE_FULL_ASSERT

STD_PERIPH_DIR = lib/STM32F10x_StdPeriph_Driver-V3.6.1

INCDIRS += $(STD_PERIPH_DIR)/inc
INCDIRS += lib/CMSIS/Include
INCDIRS += lib/CMSIS/Device/ST/STM32F10x/Include

SOURCES += $(STD_PERIPH_DIR)/src/misc.c
SOURCES += $(STD_PERIPH_DIR)/src/stm32f10x_crc.c
SOURCES += $(STD_PERIPH_DIR)/src/stm32f10x_flash.c

#SOURCES += lib/CMSIS/Device/ST/STM32F10x/Source/startup_stm32f10x_ld.s

CPPFLAGS += -DSTM32F10X_LD
LDSCRIPT = stm32f103/bootloader/stm32_flash.ld

#============================================================================
OBJECTS += $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES))))
CPPFLAGS += $(addprefix -I,$(INCDIRS))

#---------------- Preprocessor Options ----------------
#  -fsingle...    make better use of the single-precision FPU
#  -g             generate debugging information
#  -save-temps    preserve .s and .i-files
#
CPPFLAGS += -fsingle-precision-constant
CPPFLAGS += -g
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
#CFLAGS += -Wstrict-prototypes
#CFLAGS += -Wextra
#CFLAGS += -Wpointer-arith
#CFLAGS += -Winline
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wundef

# Use a friendly C dialect
CPPFLAGS += -fno-strict-aliasing
CPPFLAGS += -fno-common
CPPFLAGS += -Wfatal-errors -g -specs=nosys.specs -nostdlib
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
#POSTLD   = tools/add_version_info.py # -q
POSTLD   = cp
# Compiler flags to generate dependency files
#
GENDEPFLAGS = -MMD -MP

# Combine all necessary flags and optional flags
# Add target processor to flags.
#
CPU = -mthumb -mcpu=cortex-m3

CFLAGS   += $(CPU)
CXXFLAGS += $(CPU)
ASFLAGS  += $(CPU)
LDFLAGS  += $(CPU)

# Default target
#
all:  gccversion build showsize

build: elf hex bin lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
bin: $(TARGET).bin
lss: $(TARGET).lss
sym: $(TARGET).sym

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
flash: build showsize
	st-flash --reset write $(TARGET).bin 0x08000000

# Target: clean project
#
clean:
	@echo Cleaning project:
	rm -rf $(OBJDIR)

# Include the base rules
#
include base.mak

# Include the dependency files
#
-include $(OBJECTS:.o=.d)

# Listing of phony targets
#
.PHONY: all build flash clean \
        elf lss sym \
        showsize gccversion
