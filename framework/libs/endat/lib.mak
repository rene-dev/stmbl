ifneq (,$(findstring ENDAT, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/endat
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.c)
endif