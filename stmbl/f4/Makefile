#OPT = -O1

#config size in kB
CONF_SIZE = 16
CPU = STM32F405VG
XTAL_FREQ = 8000000

#HWVERSION = v4

CFLAGS += -DHAL_MAX_PINS=1024
CFLAGS += -DHAL_MAX_COMPS=32
CFLAGS += -DHAL_MAX_CTX=16384

LIBS = ST_SPL
LIBS += CMSIS
LIBS += ST_USB
LIBS += ST_VCP
LIBS += ST_OTG
LIBS += HAL
LIBS += STMBLTALK
LIBS += CONF
LIBS += CMD
LIBS += MISC
LIBS += CRC
LIBS += ENDAT

PERIPH += TIM
PERIPH += UART
PERIPH += ADC
PERIPH += CRC
PERIPH += RCC
PERIPH += GPIO
PERIPH += USB
PERIPH += PWR
PERIPH += SPI
PERIPH += FLASH
PERIPH += DMA

SOURCES += $(wildcard src/*.c)

INCDIRS += ../shared/inc/
SHAREDSOURCES += $(wildcard ../shared/src/*.c)

SRC_COMPS += $(wildcard src/comps/*.c)

LIB_COMPS += $(wildcard ../../framework/comps/*.c)
LIB_COMPS += $(wildcard ../../framework/comps/motor_ctr/*.c)
LIB_COMPS += $(wildcard ../../framework/comps/motor_model/*.c)
LIB_COMPS += $(wildcard ../../framework/comps/fb/*.c)
LIB_COMPS += $(wildcard ../../framework/comps/misc/*.c)

# LIB_COMPS += $(wildcard ../../framework/comps/term.c)

INCDIRS += build/gen/inc/src_comps/hw/

ifeq ($(HWVERSION),v3)
	SRC_COMPS += src/comps/hw/io3.c
	SRC_COMPS += src/comps/hw/hvf1.c
	# SHARED_COMPS += shared/comps/pmsm.c
	# SHARED_COMPS += shared/comps/curpid.c
	# SHARED_COMPS += shared/comps/dq.c
	# SHARED_COMPS += shared/comps/idq.c
	# SHARED_COMPS += shared/common_f1.c
	CFLAGS += -DV3
else
	SRC_COMPS += src/comps/hw/io4.c
	SRC_COMPS += src/comps/hw/hv.c
	# #TODO: need backport to v3
	# SRC_COMPS += src/comps/enc_cmd.c
	# SRC_COMPS += src/comps/o_fb.c
	# SRC_COMPS += src/comps/sserial.c
	# SRC_COMPS += src/comps/yaskawa.c
	# SRC_COMPS += src/comps/encs.c
	# SRC_COMPS += src/comps/encf.c
	# SRC_COMPS += src/comps/endat.c
	CFLAGS += -DV4
endif


CONFIG_TEMPLATES += $(wildcard ../../framework/conf/template/fb/*.txt)
CONFIG_TEMPLATES += $(wildcard ../../framework/conf/template/misc/*.txt)
CONFIG_TEMPLATES += $(wildcard ../../framework/conf/template/motor_ctr/*.txt)
CONFIG_TEMPLATES += $(wildcard ../../framework/conf/template/motor_model/*.txt)

OBJECTS += ../f3/build/fw.o

# Include the base rules
#
include ../../framework/base.mak

../f3/build/fw.o: force_look
	$(MAKE) -C ../ f3
	$(OBJCOPY) --rename-section .data=.hv_firmware -I binary ../f3/build/fw.bin -B arm -O elf32-littlearm ../f3/build/fw.o

build/fw_all.bin: f4_boot/build/fw.bin conf/festo.txt build/fw.bin
	cat f4_boot/build/fw.bin /dev/zero | head -c 32768 > build/fw_all.bin
	cat conf/festo.txt /dev/zero | head -c 32768 >> build/fw_all.bin
	cat build/fw.bin >> build/fw_all.bin
