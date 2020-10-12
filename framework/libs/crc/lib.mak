ifneq (,$(findstring CRC, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/crc
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.c)
endif