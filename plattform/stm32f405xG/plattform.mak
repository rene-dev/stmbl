CPPFLAGS += -DSTM32F405xx
SOURCES += $(wildcard plattform/$(CPU)/src/*.c)
SOURCES += lib/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f405xx.s
SOURCES += lib/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
HAL_DRV_DIR = lib/STM32F4xx_HAL_Driver
INCDIRS += lib/CMSIS/Device/ST/STM32F4xx/Include
INCDIRS += plattform/$(CPU)/inc
LDSCRIPT = plattform/$(CPU)/stm32f405xG_flash.ld
CPU_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
