ifneq (,$(findstring STM32F4, $(CPU)))  
  ifneq (,$(findstring ST_SPL, $(LIBS)))
    # lib path
    LIBPATH = $(FRAMEWORK_DIR)/libs/st_spl_f4
    $(info using LIB: $(LIBPATH))

    # cpu define
    ifneq (,$(findstring STM32F40, $(CPU)))
      CPUDEF = STM32F40_41xxx
    endif

    # check cpu define
    ifeq (, $(CPUDEF))
      $(error missing CPU definition for $(CPU))
    endif

    # peripheral include
    INCDIRS := $(INCDIRS) $(LIBPATH)/inc/

    ifneq (,$(findstring UART ,$(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) &(LIBPATH)/src/stm32f4xx_usart.c
    endif
  endif
endif