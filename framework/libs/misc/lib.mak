ifneq (,$(findstring MISC, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/misc
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.c)
endif