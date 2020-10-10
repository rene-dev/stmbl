# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#
OPT = -O3
# -flto
# OPT = -O1         # for debugging

# Object files directory
# Warning: this will be removed by make clean!
#
OBJDIR = obj_f3_boot

# Target file name (without extension)
TARGET = $(OBJDIR)/f3_boot

# Define all C source files (dependencies are generated automatically)
INCDIRS += f3_boot/inc
SOURCES += f3_boot/src/main.c
SOURCES += f3_boot/src/stm32f3xx_hal_msp.c
SOURCES += f3_boot/src/tim.c
SOURCES += f3_boot/src/stm32f3xx_it.c
SOURCES += f3_boot/src/version.c

CFLAGS += -DHAL_MAX_CTX=1024

INCDIRS += shared

#CMSIS
CPPFLAGS += -DSTM32F303xC
INCDIRS += lib/CMSIS/Include/
INCDIRS += lib/CMSIS/Device/ST/STM32F3xx/Include/
SOURCES += lib/CMSIS/Device/ST/STM32F3xx/Source/Templates/gcc/startup_stm32f303xc.s
SOURCES += lib/CMSIS/Device/ST/STM32F3xx/Source/Templates/system_stm32f3xx.c

#stm32f3 HAL Driver
HAL_DRV_DIR = lib/STM32F3xx_HAL_Driver/

INCDIRS += $(HAL_DRV_DIR)/Inc
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_cortex.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_crc.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_crc_ex.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_rtc.c

# SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_dma.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_flash.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_flash_ex.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_gpio.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_rcc.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_rcc_ex.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_tim.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_tim_ex.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_uart.c
SOURCES += $(HAL_DRV_DIR)/Src/stm32f3xx_hal_uart_ex.c

LDSCRIPT = f3_boot/STM32F303CBTx_FLASH.ld

#============================================================================
OBJECTS += $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES))))
CPPFLAGS += $(addprefix -I,$(INCDIRS))

#---------------- Preprocessor Options ----------------
#  -fsingle...    make better use of the single-precision FPU
#  -g             generate debugging information
#  -save-temps    preserve .s and .i-files
#
#CPPFLAGS += 
# CPPFLAGS += -g
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
CFLAGS += -Wno-pointer-sign #for usb lib...
#CFLAGS += -Wdouble-promotion
CFLAGS += -Wfloat-conversion
CFLAGS += -fsingle-precision-constant
# CFLAGS += -ffast-math
CFLAGS += -ffinite-math-only
CFLAGS += -fno-trapping-math
CFLAGS += -fno-signaling-nans
CFLAGS += -fno-rounding-math
CFLAGS += -fno-signed-zeros
CFLAGS += -fno-math-errno
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
LDFLAGS += -lc -specs=nosys.specs #fixes sbrk missing? present in eclipse?
LDFLAGS += -T$(LDSCRIPT)

#============================================================================

POSTLD   = $(PYTHON) tools/add_version_info.py # -q

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

ADDRESS = 0x8000000
# Default target
#
all: gccversion build showsize

build: elf hex bin lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
bin: $(TARGET).bin
lss: $(TARGET).lss
sym: $(TARGET).sym

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
.PHONY: all build clean \
        elf hex bin lss sym
