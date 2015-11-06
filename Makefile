# put your *.o targets here, make should handle the rest!
#TODO
#dependency files
#ld gcollection
#komische flags

SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c printf.c scanf.c setup.c hal.c misc.c eeprom.c link.c stm32f103/src/common.c
#USB
SRCS +=  ub_lib/stm32_ub_usb_cdc.c ub_lib/usb_cdc_lolevel/usb_core.c ub_lib/usb_cdc_lolevel/usb_dcd_int.c ub_lib/usb_cdc_lolevel/usbd_req.c ub_lib/usb_cdc_lolevel/usbd_cdc_core.c ub_lib/usb_cdc_lolevel/usbd_core.c ub_lib/usb_cdc_lolevel/usb_dcd.c ub_lib/usb_cdc_lolevel/usbd_cdc_vcp.c ub_lib/usb_cdc_lolevel/usbd_desc.c ub_lib/usb_cdc_lolevel/usbd_ioreq.c ub_lib/usb_cdc_lolevel/usb_bsp.c ub_lib/usb_cdc_lolevel/usbd_usr.c
#SRCS = main.c system.c
COMPS = df.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=main

# Put your stlink folder here so make burn will work.

#STLINK=/mnt/share/Programming/embedded/stm32/stlink

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
#CCDIR = /Users/rene/Downloads/gcc-arm-none-eabi-4_7-2013q3/bin

CFLAGS  = -g -Wall -Tstm32_flash.ld -std=gnu99 -fno-builtin -CC -fdiagnostics-color=always
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -nostartfiles
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostartfiles -fsingle-precision-constant
CFLAGS += -ffunction-sections -fdata-sections

CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals -Isrc/ub_lib -Isrc/ub_lib/usb_cdc_lolevel

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -Iinc -Ilib -Ilib/inc -Istm32f103/inc
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals

SRCS += lib/startup_stm32f4xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj

again: clean all

# Flash the STM32F4
burn: main.elf
	st-flash --reset write $(PROJ_NAME).bin 0x8000000

btburn: main.elf
	printf "bootloader\r" > /dev/cu.usbmodem1411 || true
	printf "bootloader\r" > /dev/cu.usbmodem1421 || true
	sleep 1
	dfu-util -a 0 -d 0483:df11 -s 0x08000000:leave -D main.bin

# Create tags; assumes ctags exists
ctags:
	ctags -R --exclude=*cm0.h --exclude=*cm3.h .

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf


$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lstm32f4 -Wl,--gc-sections -Wl,-Map -Wl,$(PROJ_NAME).map
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	arm-none-eabi-size main.elf

clean:
	rm -f *.o *.i
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
