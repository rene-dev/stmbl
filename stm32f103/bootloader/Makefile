CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
CPPC = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size
TARGET = main
#CFLAGS += -ffunction-sections -fdata-sections -fno-common -Os -g3 -mcpu=cortex-m0 -mthumb

CPU = -mthumb -mcpu=cortex-m3
#OPT = -O3 -flto
OPT = -Os
CFLAGS += -DHSE_VALUE=8000000
CFLAGS += -DSTM32F103R6

LDSCRIPT = stm32f103.ld

CFLAGS += $(CPU)
CFLAGS += $(OPT)
CFLAGS += -fno-common
CFLAGS += -std=gnu11
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -Wall
CFLAGS += -fno-builtin
CFLAGS += -nostartfiles
CFLAGS += -Wfatal-errors
CFLAGS += -fno-strict-aliasing
CFLAGS += -fwrapv
CFLAGS += -fsingle-precision-constant
CFLAGS += -g3

CPPFLAGS += $(CFLAGS)
CPPFLAGS += -fno-exceptions 
CPPFLAGS += -fno-rtti
CPPFLAGS += -std=c++11

LDFLAGS = -nostartfiles
LDFLAGS += $(OPT)
LDFLAGS += $(CPU)
LDFLAGS += -lm
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref
#LDFLAGS += -Wl,--gc-sections


all: $(TARGET).elf

$(TARGET).elf: main.c Makefile $(LDSCRIPT)
	$(CC) -I. -c $(CFLAGS) main.c -o $(TARGET).o
	#$(CPPC) -I. -c $(CPPFLAGS) main.c -o $(TARGET).o
	#$(LD) $(LDFLAGS) -T$(LDSCRIPT) -o $(TARGET).elf $(TARGET).o
	$(CC) $(LDFLAGS) -T$(LDSCRIPT) -o $(TARGET).elf $(TARGET).o
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

size: $(TARGET).elf
	$(SIZE) $<

flash: $(TARGET).elf
	st-flash --reset write $(TARGET).bin 0x8000000

clean:
	rm -f $(TARGET).elf $(TARGET).bin $(TARGET).o $(TARGET).map
