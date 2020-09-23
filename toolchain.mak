# Define programs and commands for the toolchain
# Do not modiy this file. Instead create a file toolchain-user.mak which can overwrite anything in here.
PYTHON     = python3
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
