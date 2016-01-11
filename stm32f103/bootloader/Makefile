export

DEVICE_FAMILY = STM32F10x
DEVICE_TYPE = STM32F10X_LD
STARTUP_FILE = stm32f10x_ld
SYSTEM_FILE = stm32f10x

CMSIS = Libraries/CMSIS
CMSIS_DEVSUP = $(CMSIS)/CM3/DeviceSupport/ST/$(DEVICE_FAMILY)/
CMSIS_OPT = -D$(DEVICE_TYPE)
# -DUSE_STDPERIPH_DRIVER
OTHER_OPT = "-D__weak=__attribute__((weak))" "-D__packed=__attribute__((__packed__))"
CPU = -mthumb -mcpu=cortex-m3
SYSTEM = arm-none-eabi

LDSCRIPT = "stm32_flash.ld"

SRCDIR := src/
INCDIR := inc/

LIBDIR := ../Libraries/

LIBINC := -I../inc
LIBINC += -I../Libraries/STM32F10x_StdPeriph_Driver/inc
LIBINC += -I../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
LIBINC += -I../Libraries/CMSIS/CM3/CoreSupport

#LIBS := ./$(LIBDIR)/STM32F10x_StdPeriph_Driver/libstm32fw.a
	   
#LIBS += -lm
CC      = $(SYSTEM)-gcc
CCDEP   = $(SYSTEM)-gcc
LD      = $(SYSTEM)-gcc
AR      = $(SYSTEM)-ar
AS      = $(SYSTEM)-gcc
OBJCOPY = $(SYSTEM)-objcopy
OBJDUMP	= $(SYSTEM)-objdump
GDB		= $(SYSTEM)-gdb
SIZE	= $(SYSTEM)-size

# INCLUDES = -I$(SRCDIR) $(LIBINC)
INCLUDES = $(LIBINC) -I../../shared
CFLAGS  = $(CPU) $(CMSIS_OPT) $(OTHER_OPT) -Wall -fno-common -fno-builtin -fno-strict-aliasing -Os $(INCLUDES) -g -Wfatal-errors -g -std=c99 -fdiagnostics-color=always -ffunction-sections -fdata-sections -nostartfiles -specs=nosys.specs -nostdlib
ASFLAGS = $(CFLAGS) -x assembler-with-cpp
LDFLAGS = -Wl,--gc-sections,-Map=$*.map,-cref -T $(LDSCRIPT) $(CPU)
ARFLAGS = cr
OBJCOPYFLAGS = -Obinary
OBJDUMPFLAGS = -S

#STARTUP_OBJ = ../$(CMSIS_DEVSUP)/startup/TrueSTUDIO/startup_$(STARTUP_FILE).o
#SYSTEM_OBJ = ../$(CMSIS_DEVSUP)/system_$(SYSTEM_FILE).o

BIN = main.bin

OBJS = $(sort \
 $(patsubst %.c,%.o,$(wildcard src/*.c)) \
 $(patsubst %.s,%.o,$(wildcard src/*.s)))
# $(STARTUP_OBJ) \
# $(SYSTEM_OBJ))

all: $(BIN)

burn: $(BIN)
	st-flash --reset write $(BIN) 0x8000000
	
$(BIN): main.out
	$(OBJCOPY) $(OBJCOPYFLAGS) main.out $(BIN)
	$(OBJDUMP) $(OBJDUMPFLAGS) main.out > main.list
	$(SIZE) main.out
	@echo Make finished

main.out: $(LIBS) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(LIBS): libs

libs:
	@$(MAKE) -C $(LIBDIR)

libclean: clean
	@$(MAKE) -C $(LIBDIR) clean

clean:
	-rm -f $(OBJS)
	-rm -f main.list main.out main.hex main.map main.bin .depend

depend dep: .depend

include .depend

.depend: src/*.c
	$(CCDEP) $(CFLAGS) -MM $^ | sed -e 's@.*.o:@src/&@' > .depend 

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.s.o:
	$(AS) $(ASFLAGS) -c -o $@ $<
