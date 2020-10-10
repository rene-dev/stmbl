ifneq (,$(findstring ST_VCP, $(LIBS)))
  # lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/st_vcp
  $(info using LIB: $(LIBPATH))

  # peripheral include
  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/usbd_desc.c
endif