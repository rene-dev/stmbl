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
INCDIRS += inc/comps
INCDIRS += inc/comps/hw
INCDIRS += inc/shared_comps
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
	SRC_COMPS += src/comps/hw/io3.c
	SRC_COMPS += src/comps/hvf1.c
	SHARED_COMPS += shared/comps/pmsm.c
	SHARED_COMPS += shared/comps/curpid.c
	SHARED_COMPS += shared/comps/dq.c
	SHARED_COMPS += shared/comps/idq.c
	SHARED_COMPS += shared/common_f1.c
	CFLAGS += -DV3
else
	SRC_COMPS += src/comps/hw/io4.c
	SRC_COMPS += src/comps/hv.c
	#TODO: need backport to v3
	SRC_COMPS += src/comps/enc_cmd.c
	SRC_COMPS += src/comps/o_fb.c
	SRC_COMPS += src/comps/sserial.c
	SRC_COMPS += src/comps/yaskawa.c
	SRC_COMPS += src/comps/encs.c
	SRC_COMPS += src/comps/encf.c
	SRC_COMPS += src/comps/endat.c
	CFLAGS += -DV4
endif

SRC_COMPS += src/comps/usart.c
SRC_COMPS += src/comps/encm.c
SRC_COMPS += src/comps/dmm.c
SRC_COMPS += src/comps/smartabs.c
SRC_COMPS += src/comps/adc.c
SRC_COMPS += src/comps/enc_fb.c
SRC_COMPS += src/comps/conf.c
SRC_COMPS += src/comps/res.c
SRC_COMPS += src/comps/hx711.c

SHARED_COMPS += shared/comps/sim.c
SHARED_COMPS += shared/comps/term.c
SHARED_COMPS += shared/comps/svm.c

SHARED_COMPS += shared/comps/vel.c
SHARED_COMPS += shared/comps/rev.c
SHARED_COMPS += shared/comps/hal_test.c
# SHARED_COMPS += shared/comps/dc.c
SHARED_COMPS += shared/comps/ypid.c
SHARED_COMPS += shared/comps/fault.c
SHARED_COMPS += shared/comps/pid.c
SHARED_COMPS += shared/comps/spid.c
SHARED_COMPS += shared/comps/pe.c
SHARED_COMPS += shared/comps/pmsm_limits.c
SHARED_COMPS += shared/comps/pmsm_ttc.c
SHARED_COMPS += shared/comps/dc_limits.c
SHARED_COMPS += shared/comps/dc_ttc.c
SHARED_COMPS += shared/comps/acim_ttc.c
SHARED_COMPS += shared/comps/uvw.c
SHARED_COMPS += shared/comps/fanuc.c
SHARED_COMPS += shared/comps/fb_switch.c
SHARED_COMPS += shared/comps/reslimit.c
SHARED_COMPS += shared/comps/iit.c
SHARED_COMPS += shared/comps/vel_int.c
SHARED_COMPS += shared/comps/linrev.c
SHARED_COMPS += shared/comps/psi.c
SHARED_COMPS += shared/comps/stp.c
#SHARED_COMPS += shared/comps/uf.c
SHARED_COMPS += shared/comps/uf2.c
SHARED_COMPS += shared/comps/ramp.c
SHARED_COMPS += shared/comps/scale.c
SHARED_COMPS += shared/comps/idx_home.c
SHARED_COMPS += shared/comps/move.c
# SHARED_COMPS += shared/comps/ac.c
SHARED_COMPS += shared/comps/not.c
SHARED_COMPS += shared/comps/and.c
SHARED_COMPS += shared/comps/or.c
SHARED_COMPS += shared/comps/jog.c
SHARED_COMPS += shared/comps/velbuf.c
SHARED_COMPS += shared/comps/avg.c
SHARED_COMPS += shared/comps/mux.c
SHARED_COMPS += shared/comps/veltopos.c
# SHARED_COMPS += shared/comps/wobl.c
SHARED_COMPS += shared/comps/debounce.c
SHARED_COMPS += shared/comps/pos_filter.c
SHARED_COMPS += shared/comps/rl.c
SHARED_COMPS += shared/comps/mad.c
SHARED_COMPS += shared/comps/sensorless.c
SHARED_COMPS += shared/comps/field.c
SHARED_COMPS += shared/comps/gain.c
SHARED_COMPS += shared/comps/rlpsij.c
SHARED_COMPS += shared/comps/veltime.c
SHARED_COMPS += shared/comps/mpid.c
SHARED_COMPS += shared/comps/fmove.c
SHARED_COMPS += shared/comps/home.c
SHARED_COMPS += shared/comps/en.c
SHARED_COMPS += shared/comps/th.c
#SHARED_COMPS += shared/comps/asm.c
SHARED_COMPS += shared/comps/id.c
SHARED_COMPS += shared/comps/motsim.c

COMPS = $(SRC_COMPS) $(SHARED_COMPS)
SOURCES += $(COMPS)

# SOURCES += src/eeprom.c
# SOURCES += src/link.c
SOURCES += src/version.c
SOURCES += src/syscalls.c

SOURCES += shared/crc8.c
SOURCES += shared/crc.c
SOURCES += shared/yaskawa_crc16.c
SOURCES += shared/endat.c
SOURCES += shared/angle.c
SOURCES += shared/hal.c
SOURCES += shared/commands.c
SOURCES += shared/config.c
SOURCES += src/conf_templates.c

SOURCES += shared/ringbuf.c

CONFIG_TEMPLATES = $(wildcard conf/template/*.txt)

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
SRC_COMP_OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SRC_COMPS))))
SHARED_COMP_OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SHARED_COMPS))))
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

$(OBJDIR)/shared/commands.o: inc/commandslist.h

inc/commandslist.h: tools/create_cmd.py $(SOURCES)
	@echo Generating commands list
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) tools/create_cmd.py $@ $(SOURCES)

src/hal_tbl.c: tools/create_hal_tbl.py $(COMPS)
	@echo Generating HAL table
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) tools/create_hal_tbl.py $@ $(COMPS)

$(SRC_COMP_OBJECTS): $(OBJDIR)/src/comps/%.o: inc/comps/%_comp.h

$(SHARED_COMP_OBJECTS): $(OBJDIR)/shared/comps/%.o: inc/shared_comps/%_comp.h

inc/comps/%_comp.h: src/comps/%.c
	@echo Generating H: $<
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) tools/create_comp_h.py $@ $<

inc/shared_comps/%_comp.h: shared/comps/%.c
	@echo Generating H: $<
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) tools/create_comp_h.py $@ $<

src/conf_templates.c: tools/create_config.py $(CONFIG_TEMPLATES)
	@echo Generating config
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) tools/create_config.py src/conf_templates.c $(CONFIG_TEMPLATES)

tbl: inc/commandslist.h src/hal_tbl.c src/conf_templates.c

obj_boot/blboot.bin: force_look
	$(MAKE) -f bootloader/Makefile

#build f4 bootloader
boot: obj_boot/blboot.bin

#flash f4 bootloader using stlink
boot_flash:
	$(MAKE) -f bootloader/Makefile flash

#flash f4 bootloader using df-util
boot_btburn:
	$(MAKE) -f bootloader/Makefile btburn


#build f3 bootloader
f3_boot: force_look
	$(MAKE) -f f3_boot/Makefile

#flash f3 bootloader using stlink
f3_boot_flash:
	$(MAKE) -f f3_boot/Makefile flash

#flash f3 bootloader using df-util
f3_boot_btburn:
	$(MAKE) -f f3_boot/Makefile btburn


#build f3 firmware
f3: force_look
	$(MAKE) -f stm32f303/Makefile

#flash f3 firmware using stlink
f3_flash:
	$(MAKE) -f stm32f303/Makefile flash

#flash f3 firmware using df-util
f3_btburn:
	$(MAKE) -f stm32f303/Makefile btburn


#generate f3 firmware object from f3 bin
hv_firmware.o: force_look
	$(MAKE) -f stm32f303/Makefile all

#build f103 firmware for V3 hardware
f1: force_look
	$(MAKE) -f stm32f103/Makefile

#flash f103 firmware for V3 hardware using stlink
f1_flash: boot
	$(MAKE) -f stm32f103/Makefile flash

f3_all_btburn: f3.bin
	@$(DFU-UTIL) -d 0483:df11 -a 0 -s 0x08000000:leave -D f3.bin
all_btburn: tools/bootloader.py f4.bin
	@$(PYTHON) tools/bootloader.py
	@sleep 1
	@$(DFU-UTIL) -d 0483:df11 -a 0 -s 0x08000000:leave -D f4.bin
all_flash: f4.bin
	@$(ST-FLASH) --reset write f4.bin 0x08000000
f3_all_flash: f3.bin
	@$(ST-FLASH) --reset write f3.bin 0x08000000

deploy: f3_boot f3 boot build binall

f4.bin: obj_boot/blboot.bin conf/festo.txt $(TARGET).bin
	cat obj_boot/blboot.bin /dev/zero | head -c 32768 > f4.bin
	cat conf/festo.txt /dev/zero | head -c 32768 >> f4.bin
	cat $(TARGET).bin >> f4.bin

f3.bin: obj_f3_boot/f3_boot.bin obj_hvf3/hvf3.bin
	cat obj_f3_boot/f3_boot.bin /dev/zero | head -c 16384 > f3.bin
	cat obj_hvf3/hvf3.bin >> f3.bin

# TODO: consolidate these two rules into a wildcard version?
f4.dfu: tools/dfu-convert.py f4.bin
	$(PYTHON) tools/dfu-convert.py -b 0x08000000:f4.bin f4.dfu

f3.dfu: tools/dfu-convert.py f3.bin
	$(PYTHON) tools/dfu-convert.py -b 0x08000000:f3.bin f3.dfu

stmbl.dfu: tools/dfu-convert.py $(TARGET).bin
	$(PYTHON) tools/dfu-convert.py -b 0x08010000:$(TARGET).bin stmbl.dfu

binall: f4.dfu f3.dfu stmbl.dfu

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
	rm -rf inc/comps/*
	rm -rf inc/shared_comps/*
	rm -rf src/conf_templates.c
	rm -rf src/hal_tbl.c
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

force_look:
	@true

# Listing of phony targets
#
.PHONY: all build clean \
        elf hex bin lss sym tbl \
        boot boot_flash boot_brburn \
        f3_boot f3_boot_flash f3_boot_btburn \
        f3 f3_flash f3_btburn \
        f1 f1_flash \
        f3_all_btburn all_btburn all_flash f3_all_flash \
        deploy \
        format \
        force_look
