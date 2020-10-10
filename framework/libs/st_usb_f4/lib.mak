ifneq (,$(findstring STM32F4, $(CPU)))
  ifneq (,$(findstring ST_USB, $(LIBS)))
    # lib path
    LIBPATH = $(FRAMEWORK_DIR)/libs/st_usb_f4
    $(info using LIB: $(LIBPATH))

    # peripheral include
    INCDIRS := $(INCDIRS) $(LIBPATH)/Class/cdc/inc
    INCDIRS := $(INCDIRS) $(LIBPATH)/Core/inc
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Class/cdc/src/usbd_cdc_core.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/src/usbd_core.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/src/usbd_ioreq.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/src/usbd_req.c
  endif
endif
