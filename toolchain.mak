# Define programs and commands for the toolchain
TOOLCHAIN = /home/niklas/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-
CC       = $(TOOLCHAIN)gcc
OBJCOPY  = $(TOOLCHAIN)objcopy
OBJDUMP  = $(TOOLCHAIN)objdump
SIZE     = $(TOOLCHAIN)size
NM       = $(TOOLCHAIN)nm
MKDIR    = mkdir
