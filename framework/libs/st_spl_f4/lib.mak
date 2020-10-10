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
    INCDIRS := $(INCDIRS) $(LIBPATH)/inc
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/misc.c

    ifneq (,$(findstring UART, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_usart.c
    endif

    ifneq (,$(findstring ADC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_adc.c
    endif

    ifneq (,$(findstring TIM, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_tim.c
    endif

    ifneq (,$(findstring DAC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_dac.c
    endif

    ifneq (,$(findstring GPIO, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_gpio.c
    endif

    ifneq (,$(findstring CRC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_crc.c
    endif

    ifneq (,$(findstring RCC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_rcc.c
    endif

    ifneq (,$(findstring PWR, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_pwr.c
    endif

    ifneq (,$(findstring FLASH, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_flash.c
    endif

    ifneq (,$(findstring DMA, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_dma.c
    endif

    ifneq (,$(findstring SPI, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f4xx_spi.c
    endif

    $(info libsrc: $(LIBSOURCES))
    $(info inc: $(INCDIRS))

  endif
endif


