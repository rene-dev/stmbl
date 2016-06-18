CPPFLAGS += -DSTM32F303xx
SOURCES += lib/CMSIS/Device/ST/STM32F30x/Source/Templates/gcc/startup_stm32f303xx.s
SOURCES += lib/CMSIS/Device/ST/STM32F30x/Source/Templates/system_stm32f30x.c
SOURCES += $(wildcard plattform/$(CPU)/src/*.c)
HAL_DRV_DIR = lib/STM32F3xx_HAL_Driver
INCDIRS += lib/CMSIS/Device/ST/STM32F30x/Include
INCDIRS += plattform/$(CPU)/inc
LDSCRIPT = plattform/$(CPU)/stm32f303xB_flash.ld
CPU_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
