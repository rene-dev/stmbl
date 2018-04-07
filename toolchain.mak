# Define programs and commands for the toolchain
PYTHON     = python
DFU-UTIL   = dfu-util
DFU-SUFFIX = dfu-suffix
ST-FLASH   = st-flash
TOOLCHAIN  = arm-none-eabi-
CC         = $(TOOLCHAIN)gcc
OBJCOPY    = $(TOOLCHAIN)objcopy
OBJDUMP    = $(TOOLCHAIN)objdump
SIZE       = $(TOOLCHAIN)size
NM         = $(TOOLCHAIN)nm
MKDIR      = mkdir
