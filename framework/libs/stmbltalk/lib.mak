ifneq (,$(findstring STMBLTALK, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/stmbltalk
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.c)
endif