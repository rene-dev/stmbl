# put your *.o targets here, make should handle the rest!

SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c stlinky.c printf.c scanf.c param.c stm32_ub_dac_dma.c setup.c stm32_ub_encoder_tim3.c
#USB
SRCS +=  ub_lib/stm32_ub_usb_cdc.c ub_lib/usb_cdc_lolevel/usb_core.c ub_lib/usb_cdc_lolevel/usb_dcd_int.c ub_lib/usb_cdc_lolevel/usbd_req.c ub_lib/usb_cdc_lolevel/usbd_cdc_core.c ub_lib/usb_cdc_lolevel/usbd_core.c ub_lib/usb_cdc_lolevel/usb_dcd.c ub_lib/usb_cdc_lolevel/usbd_cdc_vcp.c ub_lib/usb_cdc_lolevel/usbd_desc.c ub_lib/usb_cdc_lolevel/usbd_ioreq.c ub_lib/usb_cdc_lolevel/usb_bsp.c ub_lib/usb_cdc_lolevel/usbd_usr.c
#SRCS = main.c system.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=main

# Put your stlink folder here so make burn will work.

#STLINK=/mnt/share/Programming/embedded/stm32/stlink

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
#CCDIR = /Users/rene/Downloads/gcc-arm-none-eabi-4_7-2013q3/bin

CFLAGS  = -g -Wall -Tstm32_flash.ld -std=gnu99
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -nostartfiles
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostartfiles -fsingle-precision-constant
#USB
CFLAGS += -DUSBTERM
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals -Isrc/ub_lib -Isrc/ub_lib/usb_cdc_lolevel

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -Iinc -Ilib -Ilib/inc
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

# Create tags; assumes ctags exists
ctags:
	ctags -R --exclude=*cm0.h --exclude=*cm3.h .

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf
	
term:	$(PROJ_NAME).elf
	st-term `arm-none-eabi-nm main.elf | grep stlinky_term | awk '{print $$1}'`

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lstm32f4
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f *.o *.i
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
