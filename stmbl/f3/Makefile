#OPT = -O1

#config size in kB
CONF_SIZE = 4
CPU = STM32F303CC
XTAL_FREQ = 8000000
ADDRESS = 0x08004000

CFLAGS += -DHAL_MAX_PINS=256
CFLAGS += -DHAL_MAX_COMPS=16
CFLAGS += -DHAL_MAX_CTX=1024
LDFLAGS += -lc -specs=nosys.specs

LIBS = ST_HAL
LIBS += CMSIS
LIBS += HAL
LIBS += STMBLTALK
LIBS += MISC
LIBS += CONF
LIBS += CMD

PERIPH = TIM
PERIPH += UART
PERIPH += ADC
PERIPH += DAC
PERIPH += OPAMP
PERIPH += COMP
PERIPH += CRC
PERIPH += RCC
PERIPH += GPIO
PERIPH += PWR
PERIPH += FLASH
PERIPH += DMA

SOURCES += $(wildcard src/*.c)

INCDIRS += ../shared/inc/
SHAREDSOURCES += $(wildcard ../shared/src/*.c)

SRC_COMPS += $(wildcard src/comps/*.c)
SHARED_COMPS += $(wildcard ../shared/src/comps/*.c)
LIB_COMPS += $(wildcard ../../framework/comps/curpid/*.c)
LIB_COMPS += $(wildcard ../../framework/comps/misc/*.c)

#CONFIG_TEMPLATES += $(wildcard conf/template/*.txt)

# Include the base rules
#
include ../../framework/base.mak