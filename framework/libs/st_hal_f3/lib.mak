ifneq (,$(findstring STM32F3, $(CPU)))
  ifneq (,$(findstring ST_HAL, $(LIBS)))
    # lib path
    LIBPATH = $(FRAMEWORK_DIR)/libs/st_hal_f3
    $(info using LIB: $(LIBPATH))

    # check cpu define
    ifeq (, $(CPUDEF))
      $(error missing CPU definition for $(CPU))
    endif

    # peripheral include
    INCDIRS := $(INCDIRS) $(LIBPATH)/inc
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_cortex.c

    ifneq (,$(findstring UART, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_uart.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_uart_ex.c
    endif

    ifneq (,$(findstring OPAMP, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_opamp.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_opamp_ex.c
    endif

    ifneq (,$(findstring COMP, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_comp.c
    endif

    ifneq (,$(findstring ADC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_adc.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_adc_ex.c
    endif

    ifneq (,$(findstring TIM, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_tim.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_tim_ex.c
    endif

    ifneq (,$(findstring DAC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_dac.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_dac_ex.c
    endif

    ifneq (,$(findstring GPIO, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_gpio.c
    endif

    ifneq (,$(findstring CRC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_crc.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_crc_ex.c
    endif

    ifneq (,$(findstring RCC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_rcc.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_rcc_ex.c
    endif

    ifneq (,$(findstring PWR, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_pwr.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_pwr_ex.c
    endif

    ifneq (,$(findstring FLASH, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_flash.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_flash_ex.c
    endif

    ifneq (,$(findstring DMA, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f3xx_hal_dma.c
    endif
  endif
endif
