# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#
OPT = -O1
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
SOURCES += src/system_stm32f4xx.c #TODO: update this, system file from cmsis
SOURCES += src/setup.c
SOURCES += src/usb_cdc.c
SOURCES += src/config.c
# SOURCES += src/hal_conf.c
SOURCES += src/hal_tbl.c

HWVERSION = v4

ifeq ($(HWVERSION),v3)
	COMPS += src/comps/hw/io3.c
	COMPS += src/comps/hvf1.c
	COMPS += shared/comps/pmsm.c
	COMPS += shared/comps/curpid.c
	COMPS += shared/comps/dq.c
	COMPS += shared/comps/idq.c
	SOURCES += shared/common_f1.c
	CFLAGS += -DV3
else
	COMPS += src/comps/hw/io4.c
	COMPS += src/comps/hv.c
	#TODO: need backport to v3
	COMPS += src/comps/enc_cmd.c
	COMPS += src/comps/o_fb.c
	COMPS += src/comps/sserial.c
	COMPS += src/comps/yaskawa.c
	COMPS += src/comps/encs.c
	COMPS += src/comps/encf.c
	COMPS += src/comps/endat.c
	CFLAGS += -DV4
endif

COMPS += src/comps/usart.c
COMPS += src/comps/encm.c
COMPS += src/comps/smartabs.c
COMPS += src/comps/adc.c
COMPS += src/comps/enc_fb.c
COMPS += src/comps/conf.c
COMPS += src/comps/res.c
COMPS += src/comps/hx711.c

COMPS += shared/comps/sim.c
COMPS += shared/comps/term.c
COMPS += shared/comps/svm.c

COMPS += shared/comps/vel.c
COMPS += shared/comps/rev.c
COMPS += shared/comps/hal_test.c
# COMPS += shared/comps/dc.c
COMPS += shared/comps/ypid.c
COMPS += shared/comps/fault.c
COMPS += shared/comps/pid.c
COMPS += shared/comps/spid.c
COMPS += shared/comps/pe.c
COMPS += shared/comps/pmsm_limits.c
COMPS += shared/comps/pmsm_ttc.c
COMPS += shared/comps/dc_limits.c
COMPS += shared/comps/dc_ttc.c
COMPS += shared/comps/acim_ttc.c
COMPS += shared/comps/uvw.c
COMPS += shared/comps/fanuc.c
COMPS += shared/comps/fb_switch.c
COMPS += shared/comps/reslimit.c
COMPS += shared/comps/iit.c
COMPS += shared/comps/vel_int.c
COMPS += shared/comps/linrev.c
COMPS += shared/comps/psi.c
COMPS += shared/comps/stp.c
#COMPS += shared/comps/uf.c
COMPS += shared/comps/uf2.c
COMPS += shared/comps/ramp.c
COMPS += shared/comps/scale.c
COMPS += shared/comps/idx_home.c
COMPS += shared/comps/move.c
# COMPS += shared/comps/ac.c
COMPS += shared/comps/not.c
COMPS += shared/comps/and.c
COMPS += shared/comps/or.c
COMPS += shared/comps/jog.c
COMPS += shared/comps/velbuf.c
COMPS += shared/comps/avg.c
COMPS += shared/comps/mux.c
COMPS += shared/comps/veltopos.c
# COMPS += shared/comps/wobl.c
COMPS += shared/comps/debounce.c
COMPS += shared/comps/pos_filter.c
COMPS += shared/comps/rl.c
COMPS += shared/comps/mad.c
COMPS += shared/comps/sensorless.c
COMPS += shared/comps/field.c
COMPS += shared/comps/gain.c
COMPS += shared/comps/rlpsij.c
COMPS += shared/comps/veltime.c
COMPS += shared/comps/mpid.c
COMPS += shared/comps/fmove.c
COMPS += shared/comps/home.c
COMPS += shared/comps/en.c
COMPS += shared/comps/th.c

SOURCES += $(COMPS)

# SOURCES += src/eeprom.c
# SOURCES += src/link.c
SOURCES += src/version.c
SOURCES += src/syscalls.c

SOURCES += shared/crc8.c
SOURCES += shared/yaskawa_crc16.c
SOURCES += shared/endat.c
SOURCES += shared/angle.c
SOURCES += shared/hal.c
SOURCES += shared/commands.c
SOURCES += shared/config.c
SOURCES += src/conf_templates.c

SOURCES += shared/ringbuf.c

USB_VCP_DIR = lib/STM32_USB_Device_VCP-1.2.0

CPPFLAGS += -DUSBD_PRODUCT_STRING='"STMBL Virtual ComPort"'
CPPFLAGS += -DCDC_IN_FRAME_INTERVAL=1
CPPFLAGS += -DAPP_RX_DATA_SIZE=4096

INCDIRS += $(USB_VCP_DIR)/inc
SOURCES += $(USB_VCP_DIR)/src/usbd_desc.c

USB_DEVICE_DIR = lib/STM32_USB_Device_Library-1.2.0

INCDIRS += $(USB_DEVICE_DIR)/Class/cdc/inc
SOURCES += $(USB_DEVICE_DIR)/Class/cdc/src/usbd_cdc_core.c

INCDIRS += $(USB_DEVICE_DIR)/Core/inc
SOURCES += $(USB_DEVICE_DIR)/Core/src/usbd_core.c
SOURCES += $(USB_DEVICE_DIR)/Core/src/usbd_ioreq.c
SOURCES += $(USB_DEVICE_DIR)/Core/src/usbd_req.c

USB_DRIVER_DIR = lib/STM32_USB_OTG_Driver-2.2.0

INCDIRS += $(USB_DRIVER_DIR)/inc
SOURCES += $(USB_DRIVER_DIR)/src/usb_core.c
SOURCES += $(USB_DRIVER_DIR)/src/usb_dcd.c
SOURCES += $(USB_DRIVER_DIR)/src/usb_dcd_int.c

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
SOURCES += $(PERIPH_DRV_DIR)/src/stm32f4xx_spi.c
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
CFLAGS += -Wmaybe-uninitialized
CFLAGS += -Wuninitialized
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

ADDRESS = 0x08010000

# Default target
#
all:  gccversion tbl boot build showsize

build: tbl elf hex bin lss sym f3_boot

elf: $(TARGET).elf
hex: $(TARGET).hex
bin: tbl $(TARGET).bin
lss: $(TARGET).lss
sym: $(TARGET).sym

inc/commandslist.h: tbl
inc/hal_tbl.h: tbl
src/hal_tbl.c: tbl
src/conf_templates.c: tbl

#generate hal and command tables
tbl:
	@echo Generating tables
	@$(PYTHON) tools/create_hal_tbl.py . $(COMPS)
	@$(PYTHON) tools/create_config.py conf/template/* > src/conf_templates.c
	@$(PYTHON) tools/create_cmd.py $(SOURCES) > inc/commandslist.h

#build f4 bootloader
boot:
	$(MAKE) -f bootloader/Makefile

#flash f4 bootloader using stlink
boot_flash: boot
	$(MAKE) -f bootloader/Makefile flash

#flash f4 bootloader using df-util
boot_btburn: boot
	$(MAKE) -f bootloader/Makefile btburn


#build f3 bootloader
f3_boot:
	$(MAKE) -f f3_boot/Makefile

#flash f3 bootloader using stlink
f3_boot_flash:
	$(MAKE) -f f3_boot/Makefile flash

#flash f3 bootloader using df-util
f3_boot_btburn:
	$(MAKE) -f f3_boot/Makefile btburn


#build f3 firmware
f3:
	$(MAKE) -f stm32f303/Makefile

#flash f3 firmware using stlink
f3_flash:
	$(MAKE) -f stm32f303/Makefile flash

#flash f3 firmware using df-util
f3_btburn:
	$(MAKE) -f stm32f303/Makefile btburn


#generate f3 firmware object from f3 bin
hv_firmware.o:
	$(MAKE) -f stm32f303/Makefile all

#build f103 firmware for V3 hardware
f1:
	$(MAKE) -f stm32f103/Makefile

#flash f103 firmware for V3 hardware using stlink
f1_flash: boot
	$(MAKE) -f stm32f103/Makefile flash

f3_all_btburn:
	@$(DFU-UTIL) -d 0483:df11 -a 0 -s 0x08000000:leave -D f3.bin
all_btburn:
	@$(PYTHON) tools/bootloader.py
	@sleep 1
	@$(DFU-UTIL) -d 0483:df11 -a 0 -s 0x08000000:leave -D f4.bin
all_flash:
	@$(ST-FLASH) --reset write f4.bin 0x08000000
f3_all_flash:
	@$(ST-FLASH) --reset write f3.bin 0x08000000

deploy: f3_boot f3 boot build binall

binall:
	cat obj_boot/blboot.bin /dev/zero | head -c 32768 > f4.bin
	cat conf/festo.txt /dev/zero | head -c 32768 >> f4.bin
	cat obj_app/stmbl.bin >> f4.bin
	cat obj_f3_boot/f3_boot.bin /dev/zero | head -c 16384 > f3.bin
	cat obj_hvf3/hvf3.bin >> f3.bin
	$(PYTHON) tools/dfu-convert.py -b 0x08000000:f4.bin f4.dfu
	$(PYTHON) tools/dfu-convert.py -b 0x08000000:f3.bin f3.dfu
	$(PYTHON) tools/dfu-convert.py -b 0x08010000:obj_app/stmbl.bin stmbl.dfu

format:
	find src/ f3_boot/ bootloader/ stm32f103/ stm32f303/ shared/ inc/ tools/ -iname '*.h' -o -iname '*.c' | xargs clang-format -i

# Target: clean project
#
clean:
	@echo Cleaning project:
	rm -rf hv_firmware.o
	rm -rf f3.bin f4.bin f3.dfu f4.dfu stmbl.dfu 
	rm -rf $(OBJDIR)
	rm -rf inc/commandslist.h
	rm -rf src/conf_templates.c
	@$(MAKE) -f bootloader/Makefile clean
	@$(MAKE) -f f3_boot/Makefile clean
	@$(MAKE) -f stm32f103/Makefile clean
	@$(MAKE) -f stm32f303/Makefile clean

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
        showsize gccversion tbl f3_boot binall deploy format f3_all_btburn all_btburn f3_all_flash all_flash
