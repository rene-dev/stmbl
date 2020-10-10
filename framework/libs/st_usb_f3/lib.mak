ifneq (,$(findstring STM32F3, $(CPU)))
  ifneq (,$(findstring ST_USB, $(LIBS)))
    # lib path
    LIBPATH = $(FRAMEWORK_DIR)/libs/st_usb_f3
    $(info using LIB: $(LIBPATH))

    # peripheral include
  endif
endif
