ifneq (,$(findstring ST_OTG, $(LIBS)))
  # lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/st_otg
  $(info using LIB: $(LIBPATH))

  # peripheral include
  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/usb_core.c
  LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/usb_dcd.c
  LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/usb_dcd_int.c
endif
