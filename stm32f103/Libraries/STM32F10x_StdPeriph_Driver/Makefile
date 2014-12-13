# Compile the STM32F10x Standard Peripheral Library
export

LIB_OUT = libstm32fw.a

SRCS := $(shell find . -name *.[cs])

LIB_OBJS = $(sort $(patsubst %.c,%.o,$(SRCS))) 

.PHONY: all
all: $(LIB_OUT)

$(LIB_OUT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJS)

.PHONY: clean
clean:
	-rm -f $(LIB_OBJS) $(LIB_OUT)
