# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#
OPT = -O1 -flto
# OPT = -O1         # for debugging

# Object files directory
# Warning: this will be removed by make clean!
#
OBJDIR = obj_app

# Target file name (without extension)
TARGET = $(OBJDIR)/stmbl

# Define all C source files (dependencies are generated automatically)
INCDIRS += inc
INCDIRS += shared

SOURCES += src/main.c
SOURCES += src/stm32f4xx_it.c
SOURCES += src/system_stm32f4xx.c
SOURCES += src/printf.c
SOURCES += src/scanf.c
SOURCES += src/setup.c
SOURCES += src/hal.c
SOURCES += src/misc.c
SOURCES += src/eeprom.c
SOURCES += src/link.c
SOURCES += src/version.c
SOURCES += src/syscalls.c

SOURCES += shared/crc8.c
SOURCES += shared/common.c

#USB CDC
INCDIRS += src/ub_lib
INCDIRS += src/ub_lib/usb_cdc_lolevel

SOURCES += src/ub_lib/stm32_ub_usb_cdc.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usb_core.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usb_dcd_int.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usbd_req.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usbd_cdc_core.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usbd_core.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usb_dcd.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usbd_cdc_vcp.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usbd_desc.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usbd_ioreq.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usb_bsp.c
SOURCES += src/ub_lib/usb_cdc_lolevel/usbd_usr.c

# Standard peripheral library
CPPFLAGS += -DUSE_STDPERIPH_DRIVER
#CPPFLAGS += -DUSE_FULL_ASSERT

PERIPH_DRV_DIR = lib/STM32F4xx_StdPeriph_Driver-V1.6.0

INCDIRS += $(PERIPH_DRV_DIR)/inc
INCDIRS += lib/CMSIS/Include
INCDIRS += lib/CMSIS/Device/ST/STM32F4xx/Include

SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_adc.c
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_crc.c
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_dma.c
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_flash.c
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_gpio.c
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_pwr.c
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_rcc.c
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_tim.c
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_usart.c
SOURCES += $(PERIPH_DRV_DIR)/src/misc.c

SOURCES += lib/CMSIS/Device/ST/STM32F4xx/Source/startup_stm32f40_41xxx.s

CPPFLAGS += -DSTM32F40_41xxx
CPPFLAGS += -DHSE_VALUE=8000000
LDSCRIPT = stm32_flash.ld

#============================================================================
OBJECTS += $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES))))
OBJECTS += hv_firmware.o
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
all: hv gccversion boot hv build showsize

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

hv_flash: boot
	$(MAKE) -f stm32f103/Makefile flash

boot_btflash: boot
	$(MAKE) -f bootloader/Makefile btflash

hv:
	$(MAKE) -f stm32f103/Makefile

hv_firmware.o: hv
	arm-none-eabi-objcopy --rename-section .data=.hv_firmware -I binary obj_hv/hv.bin -B arm -O elf32-littlearm hv_firmware.o

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
btburn: build showsize
	#change this to your device
	printf "bootloader\r" > `ls /dev/cu.usbmodem*` || true
	printf "bootloader\r" > `ls /dev/ttyACM*` || true
	sleep 1
	dfu-util -a 0 -d 0483:df11 -s 0x08010000:leave -D $(TARGET).bin

# Target: clean project
#
clean:
	@echo Cleaning project:
	rm -rf hv_firmware.o
	rm -rf $(OBJDIR)
	@$(MAKE) -f bootloader/Makefile clean
	@$(MAKE) -f stm32f103/Makefile clean

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
