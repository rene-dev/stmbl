#include <stdint.h>

// STM32F103xx register definitions 
// file version: 1.3

// TIM2 register struct @ 0x40000000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40000000 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t DIR : 1; // Direction  reset: 0
      uint32_t CMS : 2; // Center-aligned mode selection  reset: 0
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_0 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40000004 read-write reset: 0x00000000
      uint32_t RESERVED_1 : 3;
      uint32_t CCDS : 1; // Capture/compare DMA selection  reset: 0
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t TI1S : 1; // TI1 selection  reset: 0
      uint32_t RESERVED_2 : 24;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // slave mode control register @ 0x40000008 read-write reset: 0x00000000
      uint32_t SMS : 3; // Slave mode selection  reset: 0
      uint32_t RESERVED_3 : 1;
      uint32_t TS : 3; // Trigger selection  reset: 0
      uint32_t MSM : 1; // Master/Slave mode  reset: 0
      uint32_t ETF : 4; // External trigger filter  reset: 0
      uint32_t ETPS : 2; // External trigger prescaler  reset: 0
      uint32_t ECE : 1; // External clock enable  reset: 0
      uint32_t ETP : 1; // External trigger polarity  reset: 0
      uint32_t RESERVED_4 : 16;
    };
  } SMCR;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4000000C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable  reset: 0
      uint32_t CC3IE : 1; // Capture/Compare 3 interrupt enable  reset: 0
      uint32_t CC4IE : 1; // Capture/Compare 4 interrupt enable  reset: 0
      uint32_t RESERVED_5 : 1;
      uint32_t TIE : 1; // Trigger interrupt enable  reset: 0
      uint32_t RESERVED_6 : 1;
      uint32_t UDE : 1; // Update DMA request enable  reset: 0
      uint32_t CC1DE : 1; // Capture/Compare 1 DMA request enable  reset: 0
      uint32_t CC2DE : 1; // Capture/Compare 2 DMA request enable  reset: 0
      uint32_t CC3DE : 1; // Capture/Compare 3 DMA request enable  reset: 0
      uint32_t CC4DE : 1; // Capture/Compare 4 DMA request enable  reset: 0
      uint32_t RESERVED_7 : 1;
      uint32_t TDE : 1; // Trigger DMA request enable  reset: 0
      uint32_t RESERVED_8 : 17;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40000010 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t CC2IF : 1; // Capture/Compare 2 interrupt flag  reset: 0
      uint32_t CC3IF : 1; // Capture/Compare 3 interrupt flag  reset: 0
      uint32_t CC4IF : 1; // Capture/Compare 4 interrupt flag  reset: 0
      uint32_t RESERVED_9 : 1;
      uint32_t TIF : 1; // Trigger interrupt flag  reset: 0
      uint32_t RESERVED_10 : 2;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t CC2OF : 1; // Capture/compare 2 overcapture flag  reset: 0
      uint32_t CC3OF : 1; // Capture/Compare 3 overcapture flag  reset: 0
      uint32_t CC4OF : 1; // Capture/Compare 4 overcapture flag  reset: 0
      uint32_t RESERVED_11 : 19;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40000014 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t CC2G : 1; // Capture/compare 2 generation  reset: 0
      uint32_t CC3G : 1; // Capture/compare 3 generation  reset: 0
      uint32_t CC4G : 1; // Capture/compare 4 generation  reset: 0
      uint32_t RESERVED_12 : 1;
      uint32_t TG : 1; // Trigger generation  reset: 0
      uint32_t RESERVED_13 : 25;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (output mode) @ 0x40000018 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t OC1FE : 1; // Output compare 1 fast enable  reset: 0
      uint32_t OC1PE : 1; // Output compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output compare 1 mode  reset: 0
      uint32_t OC1CE : 1; // Output compare 1 clear enable  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t OC2FE : 1; // Output compare 2 fast enable  reset: 0
      uint32_t OC2PE : 1; // Output compare 2 preload enable  reset: 0
      uint32_t OC2M : 3; // Output compare 2 mode  reset: 0
      uint32_t OC2CE : 1; // Output compare 2 clear enable  reset: 0
      uint32_t RESERVED_14 : 16;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (input mode) @ 0x40000018 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t CC2S : 2; // Capture/compare 2 selection  reset: 0
      uint32_t IC2PSC : 2; // Input capture 2 prescaler  reset: 0
      uint32_t IC2F : 4; // Input capture 2 filter  reset: 0
      uint32_t RESERVED_15 : 16;
    };
  } CCMR1_Input;
  };
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (output mode) @ 0x4000001C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t OC3FE : 1; // Output compare 3 fast enable  reset: 0
      uint32_t OC3PE : 1; // Output compare 3 preload enable  reset: 0
      uint32_t OC3M : 3; // Output compare 3 mode  reset: 0
      uint32_t OC3CE : 1; // Output compare 3 clear enable  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t OC4FE : 1; // Output compare 4 fast enable  reset: 0
      uint32_t OC4PE : 1; // Output compare 4 preload enable  reset: 0
      uint32_t OC4M : 3; // Output compare 4 mode  reset: 0
      uint32_t O24CE : 1; // Output compare 4 clear enable  reset: 0
      uint32_t RESERVED_16 : 16;
    };
  } CCMR2_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (input mode) @ 0x4000001C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t IC3PSC : 2; // Input capture 3 prescaler  reset: 0
      uint32_t IC3F : 4; // Input capture 3 filter  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t IC4PSC : 2; // Input capture 4 prescaler  reset: 0
      uint32_t IC4F : 4; // Input capture 4 filter  reset: 0
      uint32_t RESERVED_17 : 16;
    };
  } CCMR2_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40000020 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_18 : 2;
      uint32_t CC2E : 1; // Capture/Compare 2 output enable  reset: 0
      uint32_t CC2P : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t RESERVED_19 : 2;
      uint32_t CC3E : 1; // Capture/Compare 3 output enable  reset: 0
      uint32_t CC3P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_20 : 2;
      uint32_t CC4E : 1; // Capture/Compare 4 output enable  reset: 0
      uint32_t CC4P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_21 : 18;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40000024 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_22 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40000028 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_23 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4000002C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_24 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40000034 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_25 : 16;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 2 @ 0x40000038 read-write reset: 0x00000000
      uint32_t CCR2 : 16; // Capture/Compare 2 value  reset: 0
      uint32_t RESERVED_26 : 16;
    };
  } CCR2;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 3 @ 0x4000003C read-write reset: 0x00000000
      uint32_t CCR3 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_27 : 16;
    };
  } CCR3;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 4 @ 0x40000040 read-write reset: 0x00000000
      uint32_t CCR4 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_28 : 16;
    };
  } CCR4;
  union{
    volatile uint32_t w;
    struct { // DMA control register @ 0x40000048 read-write reset: 0x00000000
      uint32_t DBA : 5; // DMA base address  reset: 0
      uint32_t RESERVED_29 : 3;
      uint32_t DBL : 5; // DMA burst length  reset: 0
      uint32_t RESERVED_30 : 19;
    };
  } DCR;
  union{
    volatile uint32_t w;
    struct { // DMA address for full transfer @ 0x4000004C read-write reset: 0x00000000
      uint32_t DMAB : 16; // DMA register for burst accesses  reset: 0
      uint32_t RESERVED_31 : 16;
    };
  } DMAR;
} TIM2_t;

// TIM3 register struct @ 0x40000400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40000400 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t DIR : 1; // Direction  reset: 0
      uint32_t CMS : 2; // Center-aligned mode selection  reset: 0
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_32 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40000404 read-write reset: 0x00000000
      uint32_t RESERVED_33 : 3;
      uint32_t CCDS : 1; // Capture/compare DMA selection  reset: 0
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t TI1S : 1; // TI1 selection  reset: 0
      uint32_t RESERVED_34 : 24;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // slave mode control register @ 0x40000408 read-write reset: 0x00000000
      uint32_t SMS : 3; // Slave mode selection  reset: 0
      uint32_t RESERVED_35 : 1;
      uint32_t TS : 3; // Trigger selection  reset: 0
      uint32_t MSM : 1; // Master/Slave mode  reset: 0
      uint32_t ETF : 4; // External trigger filter  reset: 0
      uint32_t ETPS : 2; // External trigger prescaler  reset: 0
      uint32_t ECE : 1; // External clock enable  reset: 0
      uint32_t ETP : 1; // External trigger polarity  reset: 0
      uint32_t RESERVED_36 : 16;
    };
  } SMCR;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4000040C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable  reset: 0
      uint32_t CC3IE : 1; // Capture/Compare 3 interrupt enable  reset: 0
      uint32_t CC4IE : 1; // Capture/Compare 4 interrupt enable  reset: 0
      uint32_t RESERVED_37 : 1;
      uint32_t TIE : 1; // Trigger interrupt enable  reset: 0
      uint32_t RESERVED_38 : 1;
      uint32_t UDE : 1; // Update DMA request enable  reset: 0
      uint32_t CC1DE : 1; // Capture/Compare 1 DMA request enable  reset: 0
      uint32_t CC2DE : 1; // Capture/Compare 2 DMA request enable  reset: 0
      uint32_t CC3DE : 1; // Capture/Compare 3 DMA request enable  reset: 0
      uint32_t CC4DE : 1; // Capture/Compare 4 DMA request enable  reset: 0
      uint32_t RESERVED_39 : 1;
      uint32_t TDE : 1; // Trigger DMA request enable  reset: 0
      uint32_t RESERVED_40 : 17;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40000410 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t CC2IF : 1; // Capture/Compare 2 interrupt flag  reset: 0
      uint32_t CC3IF : 1; // Capture/Compare 3 interrupt flag  reset: 0
      uint32_t CC4IF : 1; // Capture/Compare 4 interrupt flag  reset: 0
      uint32_t RESERVED_41 : 1;
      uint32_t TIF : 1; // Trigger interrupt flag  reset: 0
      uint32_t RESERVED_42 : 2;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t CC2OF : 1; // Capture/compare 2 overcapture flag  reset: 0
      uint32_t CC3OF : 1; // Capture/Compare 3 overcapture flag  reset: 0
      uint32_t CC4OF : 1; // Capture/Compare 4 overcapture flag  reset: 0
      uint32_t RESERVED_43 : 19;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40000414 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t CC2G : 1; // Capture/compare 2 generation  reset: 0
      uint32_t CC3G : 1; // Capture/compare 3 generation  reset: 0
      uint32_t CC4G : 1; // Capture/compare 4 generation  reset: 0
      uint32_t RESERVED_44 : 1;
      uint32_t TG : 1; // Trigger generation  reset: 0
      uint32_t RESERVED_45 : 25;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (output mode) @ 0x40000418 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t OC1FE : 1; // Output compare 1 fast enable  reset: 0
      uint32_t OC1PE : 1; // Output compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output compare 1 mode  reset: 0
      uint32_t OC1CE : 1; // Output compare 1 clear enable  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t OC2FE : 1; // Output compare 2 fast enable  reset: 0
      uint32_t OC2PE : 1; // Output compare 2 preload enable  reset: 0
      uint32_t OC2M : 3; // Output compare 2 mode  reset: 0
      uint32_t OC2CE : 1; // Output compare 2 clear enable  reset: 0
      uint32_t RESERVED_46 : 16;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (input mode) @ 0x40000418 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t CC2S : 2; // Capture/compare 2 selection  reset: 0
      uint32_t IC2PSC : 2; // Input capture 2 prescaler  reset: 0
      uint32_t IC2F : 4; // Input capture 2 filter  reset: 0
      uint32_t RESERVED_47 : 16;
    };
  } CCMR1_Input;
  };
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (output mode) @ 0x4000041C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t OC3FE : 1; // Output compare 3 fast enable  reset: 0
      uint32_t OC3PE : 1; // Output compare 3 preload enable  reset: 0
      uint32_t OC3M : 3; // Output compare 3 mode  reset: 0
      uint32_t OC3CE : 1; // Output compare 3 clear enable  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t OC4FE : 1; // Output compare 4 fast enable  reset: 0
      uint32_t OC4PE : 1; // Output compare 4 preload enable  reset: 0
      uint32_t OC4M : 3; // Output compare 4 mode  reset: 0
      uint32_t O24CE : 1; // Output compare 4 clear enable  reset: 0
      uint32_t RESERVED_48 : 16;
    };
  } CCMR2_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (input mode) @ 0x4000041C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t IC3PSC : 2; // Input capture 3 prescaler  reset: 0
      uint32_t IC3F : 4; // Input capture 3 filter  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t IC4PSC : 2; // Input capture 4 prescaler  reset: 0
      uint32_t IC4F : 4; // Input capture 4 filter  reset: 0
      uint32_t RESERVED_49 : 16;
    };
  } CCMR2_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40000420 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_50 : 2;
      uint32_t CC2E : 1; // Capture/Compare 2 output enable  reset: 0
      uint32_t CC2P : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t RESERVED_51 : 2;
      uint32_t CC3E : 1; // Capture/Compare 3 output enable  reset: 0
      uint32_t CC3P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_52 : 2;
      uint32_t CC4E : 1; // Capture/Compare 4 output enable  reset: 0
      uint32_t CC4P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_53 : 18;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40000424 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_54 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40000428 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_55 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4000042C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_56 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40000434 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_57 : 16;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 2 @ 0x40000438 read-write reset: 0x00000000
      uint32_t CCR2 : 16; // Capture/Compare 2 value  reset: 0
      uint32_t RESERVED_58 : 16;
    };
  } CCR2;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 3 @ 0x4000043C read-write reset: 0x00000000
      uint32_t CCR3 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_59 : 16;
    };
  } CCR3;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 4 @ 0x40000440 read-write reset: 0x00000000
      uint32_t CCR4 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_60 : 16;
    };
  } CCR4;
  union{
    volatile uint32_t w;
    struct { // DMA control register @ 0x40000448 read-write reset: 0x00000000
      uint32_t DBA : 5; // DMA base address  reset: 0
      uint32_t RESERVED_61 : 3;
      uint32_t DBL : 5; // DMA burst length  reset: 0
      uint32_t RESERVED_62 : 19;
    };
  } DCR;
  union{
    volatile uint32_t w;
    struct { // DMA address for full transfer @ 0x4000044C read-write reset: 0x00000000
      uint32_t DMAB : 16; // DMA register for burst accesses  reset: 0
      uint32_t RESERVED_63 : 16;
    };
  } DMAR;
} TIM3_t;

// TIM4 register struct @ 0x40000800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40000800 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t DIR : 1; // Direction  reset: 0
      uint32_t CMS : 2; // Center-aligned mode selection  reset: 0
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_64 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40000804 read-write reset: 0x00000000
      uint32_t RESERVED_65 : 3;
      uint32_t CCDS : 1; // Capture/compare DMA selection  reset: 0
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t TI1S : 1; // TI1 selection  reset: 0
      uint32_t RESERVED_66 : 24;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // slave mode control register @ 0x40000808 read-write reset: 0x00000000
      uint32_t SMS : 3; // Slave mode selection  reset: 0
      uint32_t RESERVED_67 : 1;
      uint32_t TS : 3; // Trigger selection  reset: 0
      uint32_t MSM : 1; // Master/Slave mode  reset: 0
      uint32_t ETF : 4; // External trigger filter  reset: 0
      uint32_t ETPS : 2; // External trigger prescaler  reset: 0
      uint32_t ECE : 1; // External clock enable  reset: 0
      uint32_t ETP : 1; // External trigger polarity  reset: 0
      uint32_t RESERVED_68 : 16;
    };
  } SMCR;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4000080C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable  reset: 0
      uint32_t CC3IE : 1; // Capture/Compare 3 interrupt enable  reset: 0
      uint32_t CC4IE : 1; // Capture/Compare 4 interrupt enable  reset: 0
      uint32_t RESERVED_69 : 1;
      uint32_t TIE : 1; // Trigger interrupt enable  reset: 0
      uint32_t RESERVED_70 : 1;
      uint32_t UDE : 1; // Update DMA request enable  reset: 0
      uint32_t CC1DE : 1; // Capture/Compare 1 DMA request enable  reset: 0
      uint32_t CC2DE : 1; // Capture/Compare 2 DMA request enable  reset: 0
      uint32_t CC3DE : 1; // Capture/Compare 3 DMA request enable  reset: 0
      uint32_t CC4DE : 1; // Capture/Compare 4 DMA request enable  reset: 0
      uint32_t RESERVED_71 : 1;
      uint32_t TDE : 1; // Trigger DMA request enable  reset: 0
      uint32_t RESERVED_72 : 17;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40000810 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t CC2IF : 1; // Capture/Compare 2 interrupt flag  reset: 0
      uint32_t CC3IF : 1; // Capture/Compare 3 interrupt flag  reset: 0
      uint32_t CC4IF : 1; // Capture/Compare 4 interrupt flag  reset: 0
      uint32_t RESERVED_73 : 1;
      uint32_t TIF : 1; // Trigger interrupt flag  reset: 0
      uint32_t RESERVED_74 : 2;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t CC2OF : 1; // Capture/compare 2 overcapture flag  reset: 0
      uint32_t CC3OF : 1; // Capture/Compare 3 overcapture flag  reset: 0
      uint32_t CC4OF : 1; // Capture/Compare 4 overcapture flag  reset: 0
      uint32_t RESERVED_75 : 19;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40000814 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t CC2G : 1; // Capture/compare 2 generation  reset: 0
      uint32_t CC3G : 1; // Capture/compare 3 generation  reset: 0
      uint32_t CC4G : 1; // Capture/compare 4 generation  reset: 0
      uint32_t RESERVED_76 : 1;
      uint32_t TG : 1; // Trigger generation  reset: 0
      uint32_t RESERVED_77 : 25;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (output mode) @ 0x40000818 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t OC1FE : 1; // Output compare 1 fast enable  reset: 0
      uint32_t OC1PE : 1; // Output compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output compare 1 mode  reset: 0
      uint32_t OC1CE : 1; // Output compare 1 clear enable  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t OC2FE : 1; // Output compare 2 fast enable  reset: 0
      uint32_t OC2PE : 1; // Output compare 2 preload enable  reset: 0
      uint32_t OC2M : 3; // Output compare 2 mode  reset: 0
      uint32_t OC2CE : 1; // Output compare 2 clear enable  reset: 0
      uint32_t RESERVED_78 : 16;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (input mode) @ 0x40000818 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t CC2S : 2; // Capture/compare 2 selection  reset: 0
      uint32_t IC2PSC : 2; // Input capture 2 prescaler  reset: 0
      uint32_t IC2F : 4; // Input capture 2 filter  reset: 0
      uint32_t RESERVED_79 : 16;
    };
  } CCMR1_Input;
  };
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (output mode) @ 0x4000081C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t OC3FE : 1; // Output compare 3 fast enable  reset: 0
      uint32_t OC3PE : 1; // Output compare 3 preload enable  reset: 0
      uint32_t OC3M : 3; // Output compare 3 mode  reset: 0
      uint32_t OC3CE : 1; // Output compare 3 clear enable  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t OC4FE : 1; // Output compare 4 fast enable  reset: 0
      uint32_t OC4PE : 1; // Output compare 4 preload enable  reset: 0
      uint32_t OC4M : 3; // Output compare 4 mode  reset: 0
      uint32_t O24CE : 1; // Output compare 4 clear enable  reset: 0
      uint32_t RESERVED_80 : 16;
    };
  } CCMR2_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (input mode) @ 0x4000081C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t IC3PSC : 2; // Input capture 3 prescaler  reset: 0
      uint32_t IC3F : 4; // Input capture 3 filter  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t IC4PSC : 2; // Input capture 4 prescaler  reset: 0
      uint32_t IC4F : 4; // Input capture 4 filter  reset: 0
      uint32_t RESERVED_81 : 16;
    };
  } CCMR2_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40000820 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_82 : 2;
      uint32_t CC2E : 1; // Capture/Compare 2 output enable  reset: 0
      uint32_t CC2P : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t RESERVED_83 : 2;
      uint32_t CC3E : 1; // Capture/Compare 3 output enable  reset: 0
      uint32_t CC3P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_84 : 2;
      uint32_t CC4E : 1; // Capture/Compare 4 output enable  reset: 0
      uint32_t CC4P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_85 : 18;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40000824 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_86 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40000828 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_87 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4000082C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_88 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40000834 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_89 : 16;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 2 @ 0x40000838 read-write reset: 0x00000000
      uint32_t CCR2 : 16; // Capture/Compare 2 value  reset: 0
      uint32_t RESERVED_90 : 16;
    };
  } CCR2;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 3 @ 0x4000083C read-write reset: 0x00000000
      uint32_t CCR3 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_91 : 16;
    };
  } CCR3;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 4 @ 0x40000840 read-write reset: 0x00000000
      uint32_t CCR4 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_92 : 16;
    };
  } CCR4;
  union{
    volatile uint32_t w;
    struct { // DMA control register @ 0x40000848 read-write reset: 0x00000000
      uint32_t DBA : 5; // DMA base address  reset: 0
      uint32_t RESERVED_93 : 3;
      uint32_t DBL : 5; // DMA burst length  reset: 0
      uint32_t RESERVED_94 : 19;
    };
  } DCR;
  union{
    volatile uint32_t w;
    struct { // DMA address for full transfer @ 0x4000084C read-write reset: 0x00000000
      uint32_t DMAB : 16; // DMA register for burst accesses  reset: 0
      uint32_t RESERVED_95 : 16;
    };
  } DMAR;
} TIM4_t;

// TIM5 register struct @ 0x40000C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40000C00 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t DIR : 1; // Direction  reset: 0
      uint32_t CMS : 2; // Center-aligned mode selection  reset: 0
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_96 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40000C04 read-write reset: 0x00000000
      uint32_t RESERVED_97 : 3;
      uint32_t CCDS : 1; // Capture/compare DMA selection  reset: 0
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t TI1S : 1; // TI1 selection  reset: 0
      uint32_t RESERVED_98 : 24;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // slave mode control register @ 0x40000C08 read-write reset: 0x00000000
      uint32_t SMS : 3; // Slave mode selection  reset: 0
      uint32_t RESERVED_99 : 1;
      uint32_t TS : 3; // Trigger selection  reset: 0
      uint32_t MSM : 1; // Master/Slave mode  reset: 0
      uint32_t ETF : 4; // External trigger filter  reset: 0
      uint32_t ETPS : 2; // External trigger prescaler  reset: 0
      uint32_t ECE : 1; // External clock enable  reset: 0
      uint32_t ETP : 1; // External trigger polarity  reset: 0
      uint32_t RESERVED_100 : 16;
    };
  } SMCR;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x40000C0C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable  reset: 0
      uint32_t CC3IE : 1; // Capture/Compare 3 interrupt enable  reset: 0
      uint32_t CC4IE : 1; // Capture/Compare 4 interrupt enable  reset: 0
      uint32_t RESERVED_101 : 1;
      uint32_t TIE : 1; // Trigger interrupt enable  reset: 0
      uint32_t RESERVED_102 : 1;
      uint32_t UDE : 1; // Update DMA request enable  reset: 0
      uint32_t CC1DE : 1; // Capture/Compare 1 DMA request enable  reset: 0
      uint32_t CC2DE : 1; // Capture/Compare 2 DMA request enable  reset: 0
      uint32_t CC3DE : 1; // Capture/Compare 3 DMA request enable  reset: 0
      uint32_t CC4DE : 1; // Capture/Compare 4 DMA request enable  reset: 0
      uint32_t RESERVED_103 : 1;
      uint32_t TDE : 1; // Trigger DMA request enable  reset: 0
      uint32_t RESERVED_104 : 17;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40000C10 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t CC2IF : 1; // Capture/Compare 2 interrupt flag  reset: 0
      uint32_t CC3IF : 1; // Capture/Compare 3 interrupt flag  reset: 0
      uint32_t CC4IF : 1; // Capture/Compare 4 interrupt flag  reset: 0
      uint32_t RESERVED_105 : 1;
      uint32_t TIF : 1; // Trigger interrupt flag  reset: 0
      uint32_t RESERVED_106 : 2;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t CC2OF : 1; // Capture/compare 2 overcapture flag  reset: 0
      uint32_t CC3OF : 1; // Capture/Compare 3 overcapture flag  reset: 0
      uint32_t CC4OF : 1; // Capture/Compare 4 overcapture flag  reset: 0
      uint32_t RESERVED_107 : 19;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40000C14 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t CC2G : 1; // Capture/compare 2 generation  reset: 0
      uint32_t CC3G : 1; // Capture/compare 3 generation  reset: 0
      uint32_t CC4G : 1; // Capture/compare 4 generation  reset: 0
      uint32_t RESERVED_108 : 1;
      uint32_t TG : 1; // Trigger generation  reset: 0
      uint32_t RESERVED_109 : 25;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (output mode) @ 0x40000C18 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t OC1FE : 1; // Output compare 1 fast enable  reset: 0
      uint32_t OC1PE : 1; // Output compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output compare 1 mode  reset: 0
      uint32_t OC1CE : 1; // Output compare 1 clear enable  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t OC2FE : 1; // Output compare 2 fast enable  reset: 0
      uint32_t OC2PE : 1; // Output compare 2 preload enable  reset: 0
      uint32_t OC2M : 3; // Output compare 2 mode  reset: 0
      uint32_t OC2CE : 1; // Output compare 2 clear enable  reset: 0
      uint32_t RESERVED_110 : 16;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (input mode) @ 0x40000C18 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t CC2S : 2; // Capture/compare 2 selection  reset: 0
      uint32_t IC2PSC : 2; // Input capture 2 prescaler  reset: 0
      uint32_t IC2F : 4; // Input capture 2 filter  reset: 0
      uint32_t RESERVED_111 : 16;
    };
  } CCMR1_Input;
  };
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (output mode) @ 0x40000C1C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t OC3FE : 1; // Output compare 3 fast enable  reset: 0
      uint32_t OC3PE : 1; // Output compare 3 preload enable  reset: 0
      uint32_t OC3M : 3; // Output compare 3 mode  reset: 0
      uint32_t OC3CE : 1; // Output compare 3 clear enable  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t OC4FE : 1; // Output compare 4 fast enable  reset: 0
      uint32_t OC4PE : 1; // Output compare 4 preload enable  reset: 0
      uint32_t OC4M : 3; // Output compare 4 mode  reset: 0
      uint32_t O24CE : 1; // Output compare 4 clear enable  reset: 0
      uint32_t RESERVED_112 : 16;
    };
  } CCMR2_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (input mode) @ 0x40000C1C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t IC3PSC : 2; // Input capture 3 prescaler  reset: 0
      uint32_t IC3F : 4; // Input capture 3 filter  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t IC4PSC : 2; // Input capture 4 prescaler  reset: 0
      uint32_t IC4F : 4; // Input capture 4 filter  reset: 0
      uint32_t RESERVED_113 : 16;
    };
  } CCMR2_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40000C20 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_114 : 2;
      uint32_t CC2E : 1; // Capture/Compare 2 output enable  reset: 0
      uint32_t CC2P : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t RESERVED_115 : 2;
      uint32_t CC3E : 1; // Capture/Compare 3 output enable  reset: 0
      uint32_t CC3P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_116 : 2;
      uint32_t CC4E : 1; // Capture/Compare 4 output enable  reset: 0
      uint32_t CC4P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_117 : 18;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40000C24 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_118 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40000C28 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_119 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x40000C2C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_120 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40000C34 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_121 : 16;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 2 @ 0x40000C38 read-write reset: 0x00000000
      uint32_t CCR2 : 16; // Capture/Compare 2 value  reset: 0
      uint32_t RESERVED_122 : 16;
    };
  } CCR2;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 3 @ 0x40000C3C read-write reset: 0x00000000
      uint32_t CCR3 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_123 : 16;
    };
  } CCR3;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 4 @ 0x40000C40 read-write reset: 0x00000000
      uint32_t CCR4 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_124 : 16;
    };
  } CCR4;
  union{
    volatile uint32_t w;
    struct { // DMA control register @ 0x40000C48 read-write reset: 0x00000000
      uint32_t DBA : 5; // DMA base address  reset: 0
      uint32_t RESERVED_125 : 3;
      uint32_t DBL : 5; // DMA burst length  reset: 0
      uint32_t RESERVED_126 : 19;
    };
  } DCR;
  union{
    volatile uint32_t w;
    struct { // DMA address for full transfer @ 0x40000C4C read-write reset: 0x00000000
      uint32_t DMAB : 16; // DMA register for burst accesses  reset: 0
      uint32_t RESERVED_127 : 16;
    };
  } DMAR;
} TIM5_t;

// TIM6 register struct @ 0x40001000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40001000 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t RESERVED_128 : 3;
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t RESERVED_129 : 24;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40001004 read-write reset: 0x00000000
      uint32_t RESERVED_130 : 4;
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t RESERVED_131 : 25;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4000100C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t RESERVED_132 : 7;
      uint32_t UDE : 1; // Update DMA request enable  reset: 0
      uint32_t RESERVED_133 : 23;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40001010 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t RESERVED_134 : 31;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40001014 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t RESERVED_135 : 31;
    };
  } EGR;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40001024 read-write reset: 0x00000000
      uint32_t CNT : 16; // Low counter value  reset: 0
      uint32_t RESERVED_136 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40001028 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_137 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4000102C read-write reset: 0x00000000
      uint32_t ARR : 16; // Low Auto-reload value  reset: 0
      uint32_t RESERVED_138 : 16;
    };
  } ARR;
} TIM6_t;

// TIM7 register struct @ 0x40001400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40001400 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t RESERVED_139 : 3;
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t RESERVED_140 : 24;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40001404 read-write reset: 0x00000000
      uint32_t RESERVED_141 : 4;
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t RESERVED_142 : 25;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4000140C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t RESERVED_143 : 7;
      uint32_t UDE : 1; // Update DMA request enable  reset: 0
      uint32_t RESERVED_144 : 23;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40001410 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t RESERVED_145 : 31;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40001414 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t RESERVED_146 : 31;
    };
  } EGR;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40001424 read-write reset: 0x00000000
      uint32_t CNT : 16; // Low counter value  reset: 0
      uint32_t RESERVED_147 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40001428 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_148 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4000142C read-write reset: 0x00000000
      uint32_t ARR : 16; // Low Auto-reload value  reset: 0
      uint32_t RESERVED_149 : 16;
    };
  } ARR;
} TIM7_t;

// TIM12 register struct @ 0x40001800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40001800 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t RESERVED_150 : 3;
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_151 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40001804 read-write reset: 0x00000000
      uint32_t RESERVED_152 : 4;
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t RESERVED_153 : 25;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // slave mode control register @ 0x40001808 read-write reset: 0x00000000
      uint32_t SMS : 3; // Slave mode selection  reset: 0
      uint32_t RESERVED_154 : 1;
      uint32_t TS : 3; // Trigger selection  reset: 0
      uint32_t MSM : 1; // Master/Slave mode  reset: 0
      uint32_t RESERVED_155 : 24;
    };
  } SMCR;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4000180C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable  reset: 0
      uint32_t RESERVED_156 : 3;
      uint32_t TIE : 1; // Trigger interrupt enable  reset: 0
      uint32_t RESERVED_157 : 25;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40001810 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t CC2IF : 1; // Capture/Compare 2 interrupt flag  reset: 0
      uint32_t RESERVED_158 : 3;
      uint32_t TIF : 1; // Trigger interrupt flag  reset: 0
      uint32_t RESERVED_159 : 2;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t CC2OF : 1; // Capture/compare 2 overcapture flag  reset: 0
      uint32_t RESERVED_160 : 21;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40001814 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t CC2G : 1; // Capture/compare 2 generation  reset: 0
      uint32_t RESERVED_161 : 3;
      uint32_t TG : 1; // Trigger generation  reset: 0
      uint32_t RESERVED_162 : 25;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (output mode) @ 0x40001818 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t OC1FE : 1; // Output Compare 1 fast enable  reset: 0
      uint32_t OC1PE : 1; // Output Compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output Compare 1 mode  reset: 0
      uint32_t RESERVED_163 : 1;
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t OC2FE : 1; // Output Compare 2 fast enable  reset: 0
      uint32_t OC2PE : 1; // Output Compare 2 preload enable  reset: 0
      uint32_t OC2M : 3; // Output Compare 2 mode  reset: 0
      uint32_t RESERVED_164 : 17;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (input mode) @ 0x40001818 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t IC2PSC : 2; // Input capture 2 prescaler  reset: 0
      uint32_t IC2F : 4; // Input capture 2 filter  reset: 0
      uint32_t RESERVED_165 : 16;
    };
  } CCMR1_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40001820 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_166 : 1;
      uint32_t CC1NP : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t CC2E : 1; // Capture/Compare 2 output enable  reset: 0
      uint32_t CC2P : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t RESERVED_167 : 1;
      uint32_t CC2NP : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t RESERVED_168 : 24;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40001824 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_169 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40001828 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_170 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4000182C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_171 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40001834 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_172 : 16;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 2 @ 0x40001838 read-write reset: 0x00000000
      uint32_t CCR2 : 16; // Capture/Compare 2 value  reset: 0
      uint32_t RESERVED_173 : 16;
    };
  } CCR2;
} TIM12_t;

// TIM13 register struct @ 0x40001C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40001C00 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t RESERVED_174 : 4;
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_175 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40001C04 read-write reset: 0x00000000
      uint32_t RESERVED_176 : 4;
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t RESERVED_177 : 25;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x40001C0C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t RESERVED_178 : 30;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40001C10 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t RESERVED_179 : 7;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t RESERVED_180 : 22;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40001C14 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t RESERVED_181 : 30;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (output mode) @ 0x40001C18 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t RESERVED_182 : 1;
      uint32_t OC1PE : 1; // Output Compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output Compare 1 mode  reset: 0
      uint32_t RESERVED_183 : 25;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (input mode) @ 0x40001C18 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t RESERVED_184 : 24;
    };
  } CCMR1_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40001C20 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_185 : 1;
      uint32_t CC1NP : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_186 : 28;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40001C24 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_187 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40001C28 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_188 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x40001C2C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_189 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40001C34 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_190 : 16;
    };
  } CCR1;
} TIM13_t;

// TIM14 register struct @ 0x40002000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40002000 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t RESERVED_191 : 4;
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_192 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40002004 read-write reset: 0x00000000
      uint32_t RESERVED_193 : 4;
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t RESERVED_194 : 25;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4000200C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t RESERVED_195 : 30;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40002010 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t RESERVED_196 : 7;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t RESERVED_197 : 22;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40002014 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t RESERVED_198 : 30;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (output mode) @ 0x40002018 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t RESERVED_199 : 1;
      uint32_t OC1PE : 1; // Output Compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output Compare 1 mode  reset: 0
      uint32_t RESERVED_200 : 25;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (input mode) @ 0x40002018 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t RESERVED_201 : 24;
    };
  } CCMR1_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40002020 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_202 : 1;
      uint32_t CC1NP : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_203 : 28;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40002024 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_204 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40002028 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_205 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4000202C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_206 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40002034 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_207 : 16;
    };
  } CCR1;
} TIM14_t;

// RTC register struct @ 0x40002800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // RTC Control Register High @ 0x40002800 read-write reset: 0x00000000
      uint32_t SECIE : 1; // Second interrupt Enable  reset: 0
      uint32_t ALRIE : 1; // Alarm interrupt Enable  reset: 0
      uint32_t OWIE : 1; // Overflow interrupt Enable  reset: 0
      uint32_t RESERVED_208 : 29;
    };
  } CRH;
  union{
    volatile uint32_t w;
    struct { // RTC Control Register Low @ 0x40002804  reset: 0x00000020
      uint32_t SECF : 1; // Second Flag read-write reset: 0
      uint32_t ALRF : 1; // Alarm Flag read-write reset: 0
      uint32_t OWF : 1; // Overflow Flag read-write reset: 0
      uint32_t RSF : 1; // Registers Synchronized Flag read-write reset: 0
      uint32_t CNF : 1; // Configuration Flag read-write reset: 0
      uint32_t RTOFF : 1; // RTC operation OFF read-only reset: 1
      uint32_t RESERVED_209 : 26;
    };
  } CRL;
  union{
    volatile uint32_t w;
    struct { // RTC Prescaler Load Register High @ 0x40002808 write-only reset: 0x00000000
      uint32_t PRLH : 4; // RTC Prescaler Load Register High  reset: 0
      uint32_t RESERVED_210 : 28;
    };
  } PRLH;
  union{
    volatile uint32_t w;
    struct { // RTC Prescaler Load Register Low @ 0x4000280C write-only reset: 0x00008000
      uint32_t PRLL : 16; // RTC Prescaler Divider Register Low  reset: 32768
      uint32_t RESERVED_211 : 16;
    };
  } PRLL;
  union{
    volatile uint32_t w;
    struct { // RTC Prescaler Divider Register High @ 0x40002810 read-only reset: 0x00000000
      uint32_t DIVH : 4; // RTC prescaler divider register high  reset: 0
      uint32_t RESERVED_212 : 28;
    };
  } DIVH;
  union{
    volatile uint32_t w;
    struct { // RTC Prescaler Divider Register Low @ 0x40002814 read-only reset: 0x00008000
      uint32_t DIVL : 16; // RTC prescaler divider register Low  reset: 32768
      uint32_t RESERVED_213 : 16;
    };
  } DIVL;
  union{
    volatile uint32_t w;
    struct { // RTC Counter Register High @ 0x40002818 read-write reset: 0x00000000
      uint32_t CNTH : 16; // RTC counter register high  reset: 0
      uint32_t RESERVED_214 : 16;
    };
  } CNTH;
  union{
    volatile uint32_t w;
    struct { // RTC Counter Register Low @ 0x4000281C read-write reset: 0x00000000
      uint32_t CNTL : 16; // RTC counter register Low  reset: 0
      uint32_t RESERVED_215 : 16;
    };
  } CNTL;
  union{
    volatile uint32_t w;
    struct { // RTC Alarm Register High @ 0x40002820 write-only reset: 0x0000FFFF
      uint32_t ALRH : 16; // RTC alarm register high  reset: 65535
      uint32_t RESERVED_216 : 16;
    };
  } ALRH;
  union{
    volatile uint32_t w;
    struct { // RTC Alarm Register Low @ 0x40002824 write-only reset: 0x0000FFFF
      uint32_t ALRL : 16; // RTC alarm register low  reset: 65535
      uint32_t RESERVED_217 : 16;
    };
  } ALRL;
} RTC_t;

// WWDG register struct @ 0x40002C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Control register (WWDG_CR) @ 0x40002C00 read-write reset: 0x0000007F
      uint32_t T : 7; // 7-bit counter (MSB to LSB)  reset: 127
      uint32_t WDGA : 1; // Activation bit  reset: 0
      uint32_t RESERVED_218 : 24;
    };
  } CR;
  union{
    volatile uint32_t w;
    struct { // Configuration register (WWDG_CFR) @ 0x40002C04 read-write reset: 0x0000007F
      uint32_t W : 7; // 7-bit window value  reset: 127
      uint32_t WDGTB : 2; // Timer Base  reset: 0
      uint32_t EWI : 1; // Early Wakeup Interrupt  reset: 0
      uint32_t RESERVED_219 : 22;
    };
  } CFR;
  union{
    volatile uint32_t w;
    struct { // Status register (WWDG_SR) @ 0x40002C08 read-write reset: 0x00000000
      uint32_t EWI : 1; // Early Wakeup Interrupt  reset: 0
      uint32_t RESERVED_220 : 31;
    };
  } SR;
} WWDG_t;

// IWDG register struct @ 0x40003000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Key register (IWDG_KR) @ 0x40003000 write-only reset: 0x00000000
      uint32_t KEY : 16; // Key value  reset: 0
      uint32_t RESERVED_221 : 16;
    };
  } KR;
  union{
    volatile uint32_t w;
    struct { // Prescaler register (IWDG_PR) @ 0x40003004 read-write reset: 0x00000000
      uint32_t PR : 3; // Prescaler divider  reset: 0
      uint32_t RESERVED_222 : 29;
    };
  } PR;
  union{
    volatile uint32_t w;
    struct { // Reload register (IWDG_RLR) @ 0x40003008 read-write reset: 0x00000FFF
      uint32_t RL : 12; // Watchdog counter reload value  reset: 4095
      uint32_t RESERVED_223 : 20;
    };
  } RLR;
  union{
    volatile uint32_t w;
    struct { // Status register (IWDG_SR) @ 0x4000300C read-only reset: 0x00000000
      uint32_t PVU : 1; // Watchdog prescaler value update  reset: 0
      uint32_t RVU : 1; // Watchdog counter reload value update  reset: 0
      uint32_t RESERVED_224 : 30;
    };
  } SR;
} IWDG_t;

// SPI2 register struct @ 0x40003800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40003800 read-write reset: 0x00000000
      uint32_t CPHA : 1; // Clock phase  reset: 0
      uint32_t CPOL : 1; // Clock polarity  reset: 0
      uint32_t MSTR : 1; // Master selection  reset: 0
      uint32_t BR : 3; // Baud rate control  reset: 0
      uint32_t SPE : 1; // SPI enable  reset: 0
      uint32_t LSBFIRST : 1; // Frame format  reset: 0
      uint32_t SSI : 1; // Internal slave select  reset: 0
      uint32_t SSM : 1; // Software slave management  reset: 0
      uint32_t RXONLY : 1; // Receive only  reset: 0
      uint32_t DFF : 1; // Data frame format  reset: 0
      uint32_t CRCNEXT : 1; // CRC transfer next  reset: 0
      uint32_t CRCEN : 1; // Hardware CRC calculation enable  reset: 0
      uint32_t BIDIOE : 1; // Output enable in bidirectional mode  reset: 0
      uint32_t BIDIMODE : 1; // Bidirectional data mode enable  reset: 0
      uint32_t RESERVED_225 : 16;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40003804 read-write reset: 0x00000000
      uint32_t RXDMAEN : 1; // Rx buffer DMA enable  reset: 0
      uint32_t TXDMAEN : 1; // Tx buffer DMA enable  reset: 0
      uint32_t SSOE : 1; // SS output enable  reset: 0
      uint32_t RESERVED_226 : 2;
      uint32_t ERRIE : 1; // Error interrupt enable  reset: 0
      uint32_t RXNEIE : 1; // RX buffer not empty interrupt enable  reset: 0
      uint32_t TXEIE : 1; // Tx buffer empty interrupt enable  reset: 0
      uint32_t RESERVED_227 : 24;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40003808  reset: 0x00000002
      uint32_t RXNE : 1; // Receive buffer not empty read-only reset: 0
      uint32_t TXE : 1; // Transmit buffer empty read-only reset: 1
      uint32_t CHSIDE : 1; // Channel side read-only reset: 0
      uint32_t UDR : 1; // Underrun flag read-only reset: 0
      uint32_t CRCERR : 1; // CRC error flag read-write reset: 0
      uint32_t MODF : 1; // Mode fault read-only reset: 0
      uint32_t OVR : 1; // Overrun flag read-only reset: 0
      uint32_t BSY : 1; // Busy flag read-only reset: 0
      uint32_t RESERVED_228 : 24;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // data register @ 0x4000380C read-write reset: 0x00000000
      uint32_t DR : 16; // Data register  reset: 0
      uint32_t RESERVED_229 : 16;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // CRC polynomial register @ 0x40003810 read-write reset: 0x00000007
      uint32_t CRCPOLY : 16; // CRC polynomial register  reset: 7
      uint32_t RESERVED_230 : 16;
    };
  } CRCPR;
  union{
    volatile uint32_t w;
    struct { // RX CRC register @ 0x40003814 read-only reset: 0x00000000
      uint32_t RxCRC : 16; // Rx CRC register  reset: 0
      uint32_t RESERVED_231 : 16;
    };
  } RXCRCR;
  union{
    volatile uint32_t w;
    struct { // TX CRC register @ 0x40003818 read-only reset: 0x00000000
      uint32_t TxCRC : 16; // Tx CRC register  reset: 0
      uint32_t RESERVED_232 : 16;
    };
  } TXCRCR;
  union{
    volatile uint32_t w;
    struct { // I2S configuration register @ 0x4000381C read-write reset: 0x00000000
      uint32_t CHLEN : 1; // Channel length (number of bits per audio channel)  reset: 0
      uint32_t DATLEN : 2; // Data length to be transferred  reset: 0
      uint32_t CKPOL : 1; // Steady state clock polarity  reset: 0
      uint32_t I2SSTD : 2; // I2S standard selection  reset: 0
      uint32_t RESERVED_233 : 1;
      uint32_t PCMSYNC : 1; // PCM frame synchronization  reset: 0
      uint32_t I2SCFG : 2; // I2S configuration mode  reset: 0
      uint32_t I2SE : 1; // I2S Enable  reset: 0
      uint32_t I2SMOD : 1; // I2S mode selection  reset: 0
      uint32_t RESERVED_234 : 20;
    };
  } I2SCFGR;
  union{
    volatile uint32_t w;
    struct { // I2S prescaler register @ 0x40003820 read-write reset: 0x0000000A
      uint32_t I2SDIV : 8; // I2S Linear prescaler  reset: 10
      uint32_t ODD : 1; // Odd factor for the prescaler  reset: 0
      uint32_t MCKOE : 1; // Master clock output enable  reset: 0
      uint32_t RESERVED_235 : 22;
    };
  } I2SPR;
} SPI2_t;

// SPI3 register struct @ 0x40003C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40003C00 read-write reset: 0x00000000
      uint32_t CPHA : 1; // Clock phase  reset: 0
      uint32_t CPOL : 1; // Clock polarity  reset: 0
      uint32_t MSTR : 1; // Master selection  reset: 0
      uint32_t BR : 3; // Baud rate control  reset: 0
      uint32_t SPE : 1; // SPI enable  reset: 0
      uint32_t LSBFIRST : 1; // Frame format  reset: 0
      uint32_t SSI : 1; // Internal slave select  reset: 0
      uint32_t SSM : 1; // Software slave management  reset: 0
      uint32_t RXONLY : 1; // Receive only  reset: 0
      uint32_t DFF : 1; // Data frame format  reset: 0
      uint32_t CRCNEXT : 1; // CRC transfer next  reset: 0
      uint32_t CRCEN : 1; // Hardware CRC calculation enable  reset: 0
      uint32_t BIDIOE : 1; // Output enable in bidirectional mode  reset: 0
      uint32_t BIDIMODE : 1; // Bidirectional data mode enable  reset: 0
      uint32_t RESERVED_236 : 16;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40003C04 read-write reset: 0x00000000
      uint32_t RXDMAEN : 1; // Rx buffer DMA enable  reset: 0
      uint32_t TXDMAEN : 1; // Tx buffer DMA enable  reset: 0
      uint32_t SSOE : 1; // SS output enable  reset: 0
      uint32_t RESERVED_237 : 2;
      uint32_t ERRIE : 1; // Error interrupt enable  reset: 0
      uint32_t RXNEIE : 1; // RX buffer not empty interrupt enable  reset: 0
      uint32_t TXEIE : 1; // Tx buffer empty interrupt enable  reset: 0
      uint32_t RESERVED_238 : 24;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40003C08  reset: 0x00000002
      uint32_t RXNE : 1; // Receive buffer not empty read-only reset: 0
      uint32_t TXE : 1; // Transmit buffer empty read-only reset: 1
      uint32_t CHSIDE : 1; // Channel side read-only reset: 0
      uint32_t UDR : 1; // Underrun flag read-only reset: 0
      uint32_t CRCERR : 1; // CRC error flag read-write reset: 0
      uint32_t MODF : 1; // Mode fault read-only reset: 0
      uint32_t OVR : 1; // Overrun flag read-only reset: 0
      uint32_t BSY : 1; // Busy flag read-only reset: 0
      uint32_t RESERVED_239 : 24;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // data register @ 0x40003C0C read-write reset: 0x00000000
      uint32_t DR : 16; // Data register  reset: 0
      uint32_t RESERVED_240 : 16;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // CRC polynomial register @ 0x40003C10 read-write reset: 0x00000007
      uint32_t CRCPOLY : 16; // CRC polynomial register  reset: 7
      uint32_t RESERVED_241 : 16;
    };
  } CRCPR;
  union{
    volatile uint32_t w;
    struct { // RX CRC register @ 0x40003C14 read-only reset: 0x00000000
      uint32_t RxCRC : 16; // Rx CRC register  reset: 0
      uint32_t RESERVED_242 : 16;
    };
  } RXCRCR;
  union{
    volatile uint32_t w;
    struct { // TX CRC register @ 0x40003C18 read-only reset: 0x00000000
      uint32_t TxCRC : 16; // Tx CRC register  reset: 0
      uint32_t RESERVED_243 : 16;
    };
  } TXCRCR;
  union{
    volatile uint32_t w;
    struct { // I2S configuration register @ 0x40003C1C read-write reset: 0x00000000
      uint32_t CHLEN : 1; // Channel length (number of bits per audio channel)  reset: 0
      uint32_t DATLEN : 2; // Data length to be transferred  reset: 0
      uint32_t CKPOL : 1; // Steady state clock polarity  reset: 0
      uint32_t I2SSTD : 2; // I2S standard selection  reset: 0
      uint32_t RESERVED_244 : 1;
      uint32_t PCMSYNC : 1; // PCM frame synchronization  reset: 0
      uint32_t I2SCFG : 2; // I2S configuration mode  reset: 0
      uint32_t I2SE : 1; // I2S Enable  reset: 0
      uint32_t I2SMOD : 1; // I2S mode selection  reset: 0
      uint32_t RESERVED_245 : 20;
    };
  } I2SCFGR;
  union{
    volatile uint32_t w;
    struct { // I2S prescaler register @ 0x40003C20 read-write reset: 0x0000000A
      uint32_t I2SDIV : 8; // I2S Linear prescaler  reset: 10
      uint32_t ODD : 1; // Odd factor for the prescaler  reset: 0
      uint32_t MCKOE : 1; // Master clock output enable  reset: 0
      uint32_t RESERVED_246 : 22;
    };
  } I2SPR;
} SPI3_t;

// USART2 register struct @ 0x40004400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Status register @ 0x40004400  reset: 0x000000C0
      uint32_t PE : 1; // Parity error read-only reset: 0
      uint32_t FE : 1; // Framing error read-only reset: 0
      uint32_t NE : 1; // Noise error flag read-only reset: 0
      uint32_t ORE : 1; // Overrun error read-only reset: 0
      uint32_t IDLE : 1; // IDLE line detected read-only reset: 0
      uint32_t RXNE : 1; // Read data register not empty read-write reset: 0
      uint32_t TC : 1; // Transmission complete read-write reset: 1
      uint32_t TXE : 1; // Transmit data register empty read-only reset: 1
      uint32_t LBD : 1; // LIN break detection flag read-write reset: 0
      uint32_t CTS : 1; // CTS flag read-write reset: 0
      uint32_t RESERVED_247 : 22;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // Data register @ 0x40004404 read-write reset: 0x00000000
      uint32_t DR : 9; // Data value  reset: 0
      uint32_t RESERVED_248 : 23;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // Baud rate register @ 0x40004408 read-write reset: 0x00000000
      uint32_t DIV_Fraction : 4; // fraction of USARTDIV  reset: 0
      uint32_t DIV_Mantissa : 12; // mantissa of USARTDIV  reset: 0
      uint32_t RESERVED_249 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Control register 1 @ 0x4000440C read-write reset: 0x00000000
      uint32_t SBK : 1; // Send break  reset: 0
      uint32_t RWU : 1; // Receiver wakeup  reset: 0
      uint32_t RE : 1; // Receiver enable  reset: 0
      uint32_t TE : 1; // Transmitter enable  reset: 0
      uint32_t IDLEIE : 1; // IDLE interrupt enable  reset: 0
      uint32_t RXNEIE : 1; // RXNE interrupt enable  reset: 0
      uint32_t TCIE : 1; // Transmission complete interrupt enable  reset: 0
      uint32_t TXEIE : 1; // TXE interrupt enable  reset: 0
      uint32_t PEIE : 1; // PE interrupt enable  reset: 0
      uint32_t PS : 1; // Parity selection  reset: 0
      uint32_t PCE : 1; // Parity control enable  reset: 0
      uint32_t WAKE : 1; // Wakeup method  reset: 0
      uint32_t M : 1; // Word length  reset: 0
      uint32_t UE : 1; // USART enable  reset: 0
      uint32_t RESERVED_250 : 18;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // Control register 2 @ 0x40004410 read-write reset: 0x00000000
      uint32_t ADD : 4; // Address of the USART node  reset: 0
      uint32_t RESERVED_251 : 1;
      uint32_t LBDL : 1; // lin break detection length  reset: 0
      uint32_t LBDIE : 1; // LIN break detection interrupt enable  reset: 0
      uint32_t RESERVED_252 : 1;
      uint32_t LBCL : 1; // Last bit clock pulse  reset: 0
      uint32_t CPHA : 1; // Clock phase  reset: 0
      uint32_t CPOL : 1; // Clock polarity  reset: 0
      uint32_t CLKEN : 1; // Clock enable  reset: 0
      uint32_t STOP : 2; // STOP bits  reset: 0
      uint32_t LINEN : 1; // LIN mode enable  reset: 0
      uint32_t RESERVED_253 : 17;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // Control register 3 @ 0x40004414 read-write reset: 0x00000000
      uint32_t EIE : 1; // Error interrupt enable  reset: 0
      uint32_t IREN : 1; // IrDA mode enable  reset: 0
      uint32_t IRLP : 1; // IrDA low-power  reset: 0
      uint32_t HDSEL : 1; // Half-duplex selection  reset: 0
      uint32_t NACK : 1; // Smartcard NACK enable  reset: 0
      uint32_t SCEN : 1; // Smartcard mode enable  reset: 0
      uint32_t DMAR : 1; // DMA enable receiver  reset: 0
      uint32_t DMAT : 1; // DMA enable transmitter  reset: 0
      uint32_t RTSE : 1; // RTS enable  reset: 0
      uint32_t CTSE : 1; // CTS enable  reset: 0
      uint32_t CTSIE : 1; // CTS interrupt enable  reset: 0
      uint32_t RESERVED_254 : 21;
    };
  } CR3;
  union{
    volatile uint32_t w;
    struct { // Guard time and prescaler register @ 0x40004418 read-write reset: 0x00000000
      uint32_t PSC : 8; // Prescaler value  reset: 0
      uint32_t GT : 8; // Guard time value  reset: 0
      uint32_t RESERVED_255 : 16;
    };
  } GTPR;
} USART2_t;

// USART3 register struct @ 0x40004800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Status register @ 0x40004800  reset: 0x000000C0
      uint32_t PE : 1; // Parity error read-only reset: 0
      uint32_t FE : 1; // Framing error read-only reset: 0
      uint32_t NE : 1; // Noise error flag read-only reset: 0
      uint32_t ORE : 1; // Overrun error read-only reset: 0
      uint32_t IDLE : 1; // IDLE line detected read-only reset: 0
      uint32_t RXNE : 1; // Read data register not empty read-write reset: 0
      uint32_t TC : 1; // Transmission complete read-write reset: 1
      uint32_t TXE : 1; // Transmit data register empty read-only reset: 1
      uint32_t LBD : 1; // LIN break detection flag read-write reset: 0
      uint32_t CTS : 1; // CTS flag read-write reset: 0
      uint32_t RESERVED_256 : 22;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // Data register @ 0x40004804 read-write reset: 0x00000000
      uint32_t DR : 9; // Data value  reset: 0
      uint32_t RESERVED_257 : 23;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // Baud rate register @ 0x40004808 read-write reset: 0x00000000
      uint32_t DIV_Fraction : 4; // fraction of USARTDIV  reset: 0
      uint32_t DIV_Mantissa : 12; // mantissa of USARTDIV  reset: 0
      uint32_t RESERVED_258 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Control register 1 @ 0x4000480C read-write reset: 0x00000000
      uint32_t SBK : 1; // Send break  reset: 0
      uint32_t RWU : 1; // Receiver wakeup  reset: 0
      uint32_t RE : 1; // Receiver enable  reset: 0
      uint32_t TE : 1; // Transmitter enable  reset: 0
      uint32_t IDLEIE : 1; // IDLE interrupt enable  reset: 0
      uint32_t RXNEIE : 1; // RXNE interrupt enable  reset: 0
      uint32_t TCIE : 1; // Transmission complete interrupt enable  reset: 0
      uint32_t TXEIE : 1; // TXE interrupt enable  reset: 0
      uint32_t PEIE : 1; // PE interrupt enable  reset: 0
      uint32_t PS : 1; // Parity selection  reset: 0
      uint32_t PCE : 1; // Parity control enable  reset: 0
      uint32_t WAKE : 1; // Wakeup method  reset: 0
      uint32_t M : 1; // Word length  reset: 0
      uint32_t UE : 1; // USART enable  reset: 0
      uint32_t RESERVED_259 : 18;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // Control register 2 @ 0x40004810 read-write reset: 0x00000000
      uint32_t ADD : 4; // Address of the USART node  reset: 0
      uint32_t RESERVED_260 : 1;
      uint32_t LBDL : 1; // lin break detection length  reset: 0
      uint32_t LBDIE : 1; // LIN break detection interrupt enable  reset: 0
      uint32_t RESERVED_261 : 1;
      uint32_t LBCL : 1; // Last bit clock pulse  reset: 0
      uint32_t CPHA : 1; // Clock phase  reset: 0
      uint32_t CPOL : 1; // Clock polarity  reset: 0
      uint32_t CLKEN : 1; // Clock enable  reset: 0
      uint32_t STOP : 2; // STOP bits  reset: 0
      uint32_t LINEN : 1; // LIN mode enable  reset: 0
      uint32_t RESERVED_262 : 17;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // Control register 3 @ 0x40004814 read-write reset: 0x00000000
      uint32_t EIE : 1; // Error interrupt enable  reset: 0
      uint32_t IREN : 1; // IrDA mode enable  reset: 0
      uint32_t IRLP : 1; // IrDA low-power  reset: 0
      uint32_t HDSEL : 1; // Half-duplex selection  reset: 0
      uint32_t NACK : 1; // Smartcard NACK enable  reset: 0
      uint32_t SCEN : 1; // Smartcard mode enable  reset: 0
      uint32_t DMAR : 1; // DMA enable receiver  reset: 0
      uint32_t DMAT : 1; // DMA enable transmitter  reset: 0
      uint32_t RTSE : 1; // RTS enable  reset: 0
      uint32_t CTSE : 1; // CTS enable  reset: 0
      uint32_t CTSIE : 1; // CTS interrupt enable  reset: 0
      uint32_t RESERVED_263 : 21;
    };
  } CR3;
  union{
    volatile uint32_t w;
    struct { // Guard time and prescaler register @ 0x40004818 read-write reset: 0x00000000
      uint32_t PSC : 8; // Prescaler value  reset: 0
      uint32_t GT : 8; // Guard time value  reset: 0
      uint32_t RESERVED_264 : 16;
    };
  } GTPR;
} USART3_t;

// UART4 register struct @ 0x40004C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // UART4_SR @ 0x40004C00  reset: 0x00000000
      uint32_t PE : 1; // Parity error read-only reset: 0
      uint32_t FE : 1; // Framing error read-only reset: 0
      uint32_t NE : 1; // Noise error flag read-only reset: 0
      uint32_t ORE : 1; // Overrun error read-only reset: 0
      uint32_t IDLE : 1; // IDLE line detected read-only reset: 0
      uint32_t RXNE : 1; // Read data register not empty read-write reset: 0
      uint32_t TC : 1; // Transmission complete read-write reset: 0
      uint32_t TXE : 1; // Transmit data register empty read-only reset: 0
      uint32_t LBD : 1; // LIN break detection flag read-write reset: 0
      uint32_t RESERVED_265 : 23;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // UART4_DR @ 0x40004C04 read-write reset: 0x00000000
      uint32_t DR : 9; // DR  reset: 0
      uint32_t RESERVED_266 : 23;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // UART4_BRR @ 0x40004C08 read-write reset: 0x00000000
      uint32_t DIV_Fraction : 4; // DIV_Fraction  reset: 0
      uint32_t DIV_Mantissa : 12; // DIV_Mantissa  reset: 0
      uint32_t RESERVED_267 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // UART4_CR1 @ 0x40004C0C read-write reset: 0x00000000
      uint32_t SBK : 1; // Send break  reset: 0
      uint32_t RWU : 1; // Receiver wakeup  reset: 0
      uint32_t RE : 1; // Receiver enable  reset: 0
      uint32_t TE : 1; // Transmitter enable  reset: 0
      uint32_t IDLEIE : 1; // IDLE interrupt enable  reset: 0
      uint32_t RXNEIE : 1; // RXNE interrupt enable  reset: 0
      uint32_t TCIE : 1; // Transmission complete interrupt enable  reset: 0
      uint32_t TXEIE : 1; // TXE interrupt enable  reset: 0
      uint32_t PEIE : 1; // PE interrupt enable  reset: 0
      uint32_t PS : 1; // Parity selection  reset: 0
      uint32_t PCE : 1; // Parity control enable  reset: 0
      uint32_t WAKE : 1; // Wakeup method  reset: 0
      uint32_t M : 1; // Word length  reset: 0
      uint32_t UE : 1; // USART enable  reset: 0
      uint32_t RESERVED_268 : 18;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // UART4_CR2 @ 0x40004C10 read-write reset: 0x00000000
      uint32_t ADD : 4; // Address of the USART node  reset: 0
      uint32_t RESERVED_269 : 1;
      uint32_t LBDL : 1; // lin break detection length  reset: 0
      uint32_t LBDIE : 1; // LIN break detection interrupt enable  reset: 0
      uint32_t RESERVED_270 : 5;
      uint32_t STOP : 2; // STOP bits  reset: 0
      uint32_t LINEN : 1; // LIN mode enable  reset: 0
      uint32_t RESERVED_271 : 17;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // UART4_CR3 @ 0x40004C14 read-write reset: 0x00000000
      uint32_t EIE : 1; // Error interrupt enable  reset: 0
      uint32_t IREN : 1; // IrDA mode enable  reset: 0
      uint32_t IRLP : 1; // IrDA low-power  reset: 0
      uint32_t HDSEL : 1; // Half-duplex selection  reset: 0
      uint32_t RESERVED_272 : 2;
      uint32_t DMAR : 1; // DMA enable receiver  reset: 0
      uint32_t DMAT : 1; // DMA enable transmitter  reset: 0
      uint32_t RESERVED_273 : 24;
    };
  } CR3;
} UART4_t;

// UART5 register struct @ 0x40005000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // UART4_SR @ 0x40005000  reset: 0x00000000
      uint32_t PE : 1; // PE read-only reset: 0
      uint32_t FE : 1; // FE read-only reset: 0
      uint32_t NE : 1; // NE read-only reset: 0
      uint32_t ORE : 1; // ORE read-only reset: 0
      uint32_t IDLE : 1; // IDLE read-only reset: 0
      uint32_t RXNE : 1; // RXNE read-write reset: 0
      uint32_t TC : 1; // TC read-write reset: 0
      uint32_t TXE : 1; // TXE read-only reset: 0
      uint32_t LBD : 1; // LBD read-write reset: 0
      uint32_t RESERVED_274 : 23;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // UART4_DR @ 0x40005004 read-write reset: 0x00000000
      uint32_t DR : 9; // DR  reset: 0
      uint32_t RESERVED_275 : 23;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // UART4_BRR @ 0x40005008 read-write reset: 0x00000000
      uint32_t DIV_Fraction : 4; // DIV_Fraction  reset: 0
      uint32_t DIV_Mantissa : 12; // DIV_Mantissa  reset: 0
      uint32_t RESERVED_276 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // UART4_CR1 @ 0x4000500C read-write reset: 0x00000000
      uint32_t SBK : 1; // SBK  reset: 0
      uint32_t RWU : 1; // RWU  reset: 0
      uint32_t RE : 1; // RE  reset: 0
      uint32_t TE : 1; // TE  reset: 0
      uint32_t IDLEIE : 1; // IDLEIE  reset: 0
      uint32_t RXNEIE : 1; // RXNEIE  reset: 0
      uint32_t TCIE : 1; // TCIE  reset: 0
      uint32_t TXEIE : 1; // TXEIE  reset: 0
      uint32_t PEIE : 1; // PEIE  reset: 0
      uint32_t PS : 1; // PS  reset: 0
      uint32_t PCE : 1; // PCE  reset: 0
      uint32_t WAKE : 1; // WAKE  reset: 0
      uint32_t M : 1; // M  reset: 0
      uint32_t UE : 1; // UE  reset: 0
      uint32_t RESERVED_277 : 18;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // UART4_CR2 @ 0x40005010 read-write reset: 0x00000000
      uint32_t ADD : 4; // ADD  reset: 0
      uint32_t RESERVED_278 : 1;
      uint32_t LBDL : 1; // LBDL  reset: 0
      uint32_t LBDIE : 1; // LBDIE  reset: 0
      uint32_t RESERVED_279 : 5;
      uint32_t STOP : 2; // STOP  reset: 0
      uint32_t LINEN : 1; // LINEN  reset: 0
      uint32_t RESERVED_280 : 17;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // UART4_CR3 @ 0x40005014 read-write reset: 0x00000000
      uint32_t EIE : 1; // Error interrupt enable  reset: 0
      uint32_t IREN : 1; // IrDA mode enable  reset: 0
      uint32_t IRLP : 1; // IrDA low-power  reset: 0
      uint32_t HDSEL : 1; // Half-duplex selection  reset: 0
      uint32_t RESERVED_281 : 3;
      uint32_t DMAT : 1; // DMA enable transmitter  reset: 0
      uint32_t RESERVED_282 : 24;
    };
  } CR3;
} UART5_t;

// I2C1 register struct @ 0x40005400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Control register 1 @ 0x40005400 read-write reset: 0x00000000
      uint32_t PE : 1; // Peripheral enable  reset: 0
      uint32_t SMBUS : 1; // SMBus mode  reset: 0
      uint32_t RESERVED_283 : 1;
      uint32_t SMBTYPE : 1; // SMBus type  reset: 0
      uint32_t ENARP : 1; // ARP enable  reset: 0
      uint32_t ENPEC : 1; // PEC enable  reset: 0
      uint32_t ENGC : 1; // General call enable  reset: 0
      uint32_t NOSTRETCH : 1; // Clock stretching disable (Slave mode)  reset: 0
      uint32_t START : 1; // Start generation  reset: 0
      uint32_t STOP : 1; // Stop generation  reset: 0
      uint32_t ACK : 1; // Acknowledge enable  reset: 0
      uint32_t POS : 1; // Acknowledge/PEC Position (for data reception)  reset: 0
      uint32_t PEC : 1; // Packet error checking  reset: 0
      uint32_t ALERT : 1; // SMBus alert  reset: 0
      uint32_t RESERVED_284 : 1;
      uint32_t SWRST : 1; // Software reset  reset: 0
      uint32_t RESERVED_285 : 16;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // Control register 2 @ 0x40005404 read-write reset: 0x00000000
      uint32_t FREQ : 6; // Peripheral clock frequency  reset: 0
      uint32_t RESERVED_286 : 2;
      uint32_t ITERREN : 1; // Error interrupt enable  reset: 0
      uint32_t ITEVTEN : 1; // Event interrupt enable  reset: 0
      uint32_t ITBUFEN : 1; // Buffer interrupt enable  reset: 0
      uint32_t DMAEN : 1; // DMA requests enable  reset: 0
      uint32_t LAST : 1; // DMA last transfer  reset: 0
      uint32_t RESERVED_287 : 19;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // Own address register 1 @ 0x40005408 read-write reset: 0x00000000
      uint32_t ADD0 : 1; // Interface address  reset: 0
      uint32_t ADD7 : 7; // Interface address  reset: 0
      uint32_t ADD10 : 2; // Interface address  reset: 0
      uint32_t RESERVED_288 : 5;
      uint32_t ADDMODE : 1; // Addressing mode (slave mode)  reset: 0
      uint32_t RESERVED_289 : 16;
    };
  } OAR1;
  union{
    volatile uint32_t w;
    struct { // Own address register 2 @ 0x4000540C read-write reset: 0x00000000
      uint32_t ENDUAL : 1; // Dual addressing mode enable  reset: 0
      uint32_t ADD2 : 7; // Interface address  reset: 0
      uint32_t RESERVED_290 : 24;
    };
  } OAR2;
  union{
    volatile uint32_t w;
    struct { // Data register @ 0x40005410 read-write reset: 0x00000000
      uint32_t DR : 8; // 8-bit data register  reset: 0
      uint32_t RESERVED_291 : 24;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // Status register 1 @ 0x40005414  reset: 0x00000000
      uint32_t SB : 1; // Start bit (Master mode) read-only reset: 0
      uint32_t ADDR : 1; // Address sent (master mode)/matched (slave mode) read-only reset: 0
      uint32_t BTF : 1; // Byte transfer finished read-only reset: 0
      uint32_t ADD10 : 1; // 10-bit header sent (Master mode) read-only reset: 0
      uint32_t STOPF : 1; // Stop detection (slave mode) read-only reset: 0
      uint32_t RESERVED_292 : 1;
      uint32_t RxNE : 1; // Data register not empty (receivers) read-only reset: 0
      uint32_t TxE : 1; // Data register empty (transmitters) read-only reset: 0
      uint32_t BERR : 1; // Bus error read-write reset: 0
      uint32_t ARLO : 1; // Arbitration lost (master mode) read-write reset: 0
      uint32_t AF : 1; // Acknowledge failure read-write reset: 0
      uint32_t OVR : 1; // Overrun/Underrun read-write reset: 0
      uint32_t PECERR : 1; // PEC Error in reception read-write reset: 0
      uint32_t RESERVED_293 : 1;
      uint32_t TIMEOUT : 1; // Timeout or Tlow error read-write reset: 0
      uint32_t SMBALERT : 1; // SMBus alert read-write reset: 0
      uint32_t RESERVED_294 : 16;
    };
  } SR1;
  union{
    volatile uint32_t w;
    struct { // Status register 2 @ 0x40005418 read-only reset: 0x00000000
      uint32_t MSL : 1; // Master/slave  reset: 0
      uint32_t BUSY : 1; // Bus busy  reset: 0
      uint32_t TRA : 1; // Transmitter/receiver  reset: 0
      uint32_t RESERVED_295 : 1;
      uint32_t GENCALL : 1; // General call address (Slave mode)  reset: 0
      uint32_t SMBDEFAULT : 1; // SMBus device default address (Slave mode)  reset: 0
      uint32_t SMBHOST : 1; // SMBus host header (Slave mode)  reset: 0
      uint32_t DUALF : 1; // Dual flag (Slave mode)  reset: 0
      uint32_t PEC : 8; // acket error checking register  reset: 0
      uint32_t RESERVED_296 : 16;
    };
  } SR2;
  union{
    volatile uint32_t w;
    struct { // Clock control register @ 0x4000541C read-write reset: 0x00000000
      uint32_t CCR : 12; // Clock control register in Fast/Standard mode (Master mode)  reset: 0
      uint32_t RESERVED_297 : 2;
      uint32_t DUTY : 1; // Fast mode duty cycle  reset: 0
      uint32_t F_S : 1; // I2C master mode selection  reset: 0
      uint32_t RESERVED_298 : 16;
    };
  } CCR;
  union{
    volatile uint32_t w;
    struct { // TRISE register @ 0x40005420 read-write reset: 0x00000002
      uint32_t TRISE : 6; // Maximum rise time in Fast/Standard mode (Master mode)  reset: 2
      uint32_t RESERVED_299 : 26;
    };
  } TRISE;
} I2C1_t;

// I2C2 register struct @ 0x40005800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Control register 1 @ 0x40005800 read-write reset: 0x00000000
      uint32_t PE : 1; // Peripheral enable  reset: 0
      uint32_t SMBUS : 1; // SMBus mode  reset: 0
      uint32_t RESERVED_300 : 1;
      uint32_t SMBTYPE : 1; // SMBus type  reset: 0
      uint32_t ENARP : 1; // ARP enable  reset: 0
      uint32_t ENPEC : 1; // PEC enable  reset: 0
      uint32_t ENGC : 1; // General call enable  reset: 0
      uint32_t NOSTRETCH : 1; // Clock stretching disable (Slave mode)  reset: 0
      uint32_t START : 1; // Start generation  reset: 0
      uint32_t STOP : 1; // Stop generation  reset: 0
      uint32_t ACK : 1; // Acknowledge enable  reset: 0
      uint32_t POS : 1; // Acknowledge/PEC Position (for data reception)  reset: 0
      uint32_t PEC : 1; // Packet error checking  reset: 0
      uint32_t ALERT : 1; // SMBus alert  reset: 0
      uint32_t RESERVED_301 : 1;
      uint32_t SWRST : 1; // Software reset  reset: 0
      uint32_t RESERVED_302 : 16;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // Control register 2 @ 0x40005804 read-write reset: 0x00000000
      uint32_t FREQ : 6; // Peripheral clock frequency  reset: 0
      uint32_t RESERVED_303 : 2;
      uint32_t ITERREN : 1; // Error interrupt enable  reset: 0
      uint32_t ITEVTEN : 1; // Event interrupt enable  reset: 0
      uint32_t ITBUFEN : 1; // Buffer interrupt enable  reset: 0
      uint32_t DMAEN : 1; // DMA requests enable  reset: 0
      uint32_t LAST : 1; // DMA last transfer  reset: 0
      uint32_t RESERVED_304 : 19;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // Own address register 1 @ 0x40005808 read-write reset: 0x00000000
      uint32_t ADD0 : 1; // Interface address  reset: 0
      uint32_t ADD7 : 7; // Interface address  reset: 0
      uint32_t ADD10 : 2; // Interface address  reset: 0
      uint32_t RESERVED_305 : 5;
      uint32_t ADDMODE : 1; // Addressing mode (slave mode)  reset: 0
      uint32_t RESERVED_306 : 16;
    };
  } OAR1;
  union{
    volatile uint32_t w;
    struct { // Own address register 2 @ 0x4000580C read-write reset: 0x00000000
      uint32_t ENDUAL : 1; // Dual addressing mode enable  reset: 0
      uint32_t ADD2 : 7; // Interface address  reset: 0
      uint32_t RESERVED_307 : 24;
    };
  } OAR2;
  union{
    volatile uint32_t w;
    struct { // Data register @ 0x40005810 read-write reset: 0x00000000
      uint32_t DR : 8; // 8-bit data register  reset: 0
      uint32_t RESERVED_308 : 24;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // Status register 1 @ 0x40005814  reset: 0x00000000
      uint32_t SB : 1; // Start bit (Master mode) read-only reset: 0
      uint32_t ADDR : 1; // Address sent (master mode)/matched (slave mode) read-only reset: 0
      uint32_t BTF : 1; // Byte transfer finished read-only reset: 0
      uint32_t ADD10 : 1; // 10-bit header sent (Master mode) read-only reset: 0
      uint32_t STOPF : 1; // Stop detection (slave mode) read-only reset: 0
      uint32_t RESERVED_309 : 1;
      uint32_t RxNE : 1; // Data register not empty (receivers) read-only reset: 0
      uint32_t TxE : 1; // Data register empty (transmitters) read-only reset: 0
      uint32_t BERR : 1; // Bus error read-write reset: 0
      uint32_t ARLO : 1; // Arbitration lost (master mode) read-write reset: 0
      uint32_t AF : 1; // Acknowledge failure read-write reset: 0
      uint32_t OVR : 1; // Overrun/Underrun read-write reset: 0
      uint32_t PECERR : 1; // PEC Error in reception read-write reset: 0
      uint32_t RESERVED_310 : 1;
      uint32_t TIMEOUT : 1; // Timeout or Tlow error read-write reset: 0
      uint32_t SMBALERT : 1; // SMBus alert read-write reset: 0
      uint32_t RESERVED_311 : 16;
    };
  } SR1;
  union{
    volatile uint32_t w;
    struct { // Status register 2 @ 0x40005818 read-only reset: 0x00000000
      uint32_t MSL : 1; // Master/slave  reset: 0
      uint32_t BUSY : 1; // Bus busy  reset: 0
      uint32_t TRA : 1; // Transmitter/receiver  reset: 0
      uint32_t RESERVED_312 : 1;
      uint32_t GENCALL : 1; // General call address (Slave mode)  reset: 0
      uint32_t SMBDEFAULT : 1; // SMBus device default address (Slave mode)  reset: 0
      uint32_t SMBHOST : 1; // SMBus host header (Slave mode)  reset: 0
      uint32_t DUALF : 1; // Dual flag (Slave mode)  reset: 0
      uint32_t PEC : 8; // acket error checking register  reset: 0
      uint32_t RESERVED_313 : 16;
    };
  } SR2;
  union{
    volatile uint32_t w;
    struct { // Clock control register @ 0x4000581C read-write reset: 0x00000000
      uint32_t CCR : 12; // Clock control register in Fast/Standard mode (Master mode)  reset: 0
      uint32_t RESERVED_314 : 2;
      uint32_t DUTY : 1; // Fast mode duty cycle  reset: 0
      uint32_t F_S : 1; // I2C master mode selection  reset: 0
      uint32_t RESERVED_315 : 16;
    };
  } CCR;
  union{
    volatile uint32_t w;
    struct { // TRISE register @ 0x40005820 read-write reset: 0x00000002
      uint32_t TRISE : 6; // Maximum rise time in Fast/Standard mode (Master mode)  reset: 2
      uint32_t RESERVED_316 : 26;
    };
  } TRISE;
} I2C2_t;

// USB register struct @ 0x40005C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // endpoint 0 register @ 0x40005C00 read-write reset: 0x00000000
      uint32_t EA : 4; // Endpoint address  reset: 0
      uint32_t STAT_TX : 2; // Status bits, for transmission transfers  reset: 0
      uint32_t DTOG_TX : 1; // Data Toggle, for transmission transfers  reset: 0
      uint32_t CTR_TX : 1; // Correct Transfer for transmission  reset: 0
      uint32_t EP_KIND : 1; // Endpoint kind  reset: 0
      uint32_t EP_TYPE : 2; // Endpoint type  reset: 0
      uint32_t SETUP : 1; // Setup transaction completed  reset: 0
      uint32_t STAT_RX : 2; // Status bits, for reception transfers  reset: 0
      uint32_t DTOG_RX : 1; // Data Toggle, for reception transfers  reset: 0
      uint32_t CTR_RX : 1; // Correct transfer for reception  reset: 0
      uint32_t RESERVED_317 : 16;
    };
  } EP0R;
  union{
    volatile uint32_t w;
    struct { // endpoint 1 register @ 0x40005C04 read-write reset: 0x00000000
      uint32_t EA : 4; // Endpoint address  reset: 0
      uint32_t STAT_TX : 2; // Status bits, for transmission transfers  reset: 0
      uint32_t DTOG_TX : 1; // Data Toggle, for transmission transfers  reset: 0
      uint32_t CTR_TX : 1; // Correct Transfer for transmission  reset: 0
      uint32_t EP_KIND : 1; // Endpoint kind  reset: 0
      uint32_t EP_TYPE : 2; // Endpoint type  reset: 0
      uint32_t SETUP : 1; // Setup transaction completed  reset: 0
      uint32_t STAT_RX : 2; // Status bits, for reception transfers  reset: 0
      uint32_t DTOG_RX : 1; // Data Toggle, for reception transfers  reset: 0
      uint32_t CTR_RX : 1; // Correct transfer for reception  reset: 0
      uint32_t RESERVED_318 : 16;
    };
  } EP1R;
  union{
    volatile uint32_t w;
    struct { // endpoint 2 register @ 0x40005C08 read-write reset: 0x00000000
      uint32_t EA : 4; // Endpoint address  reset: 0
      uint32_t STAT_TX : 2; // Status bits, for transmission transfers  reset: 0
      uint32_t DTOG_TX : 1; // Data Toggle, for transmission transfers  reset: 0
      uint32_t CTR_TX : 1; // Correct Transfer for transmission  reset: 0
      uint32_t EP_KIND : 1; // Endpoint kind  reset: 0
      uint32_t EP_TYPE : 2; // Endpoint type  reset: 0
      uint32_t SETUP : 1; // Setup transaction completed  reset: 0
      uint32_t STAT_RX : 2; // Status bits, for reception transfers  reset: 0
      uint32_t DTOG_RX : 1; // Data Toggle, for reception transfers  reset: 0
      uint32_t CTR_RX : 1; // Correct transfer for reception  reset: 0
      uint32_t RESERVED_319 : 16;
    };
  } EP2R;
  union{
    volatile uint32_t w;
    struct { // endpoint 3 register @ 0x40005C0C read-write reset: 0x00000000
      uint32_t EA : 4; // Endpoint address  reset: 0
      uint32_t STAT_TX : 2; // Status bits, for transmission transfers  reset: 0
      uint32_t DTOG_TX : 1; // Data Toggle, for transmission transfers  reset: 0
      uint32_t CTR_TX : 1; // Correct Transfer for transmission  reset: 0
      uint32_t EP_KIND : 1; // Endpoint kind  reset: 0
      uint32_t EP_TYPE : 2; // Endpoint type  reset: 0
      uint32_t SETUP : 1; // Setup transaction completed  reset: 0
      uint32_t STAT_RX : 2; // Status bits, for reception transfers  reset: 0
      uint32_t DTOG_RX : 1; // Data Toggle, for reception transfers  reset: 0
      uint32_t CTR_RX : 1; // Correct transfer for reception  reset: 0
      uint32_t RESERVED_320 : 16;
    };
  } EP3R;
  union{
    volatile uint32_t w;
    struct { // endpoint 4 register @ 0x40005C10 read-write reset: 0x00000000
      uint32_t EA : 4; // Endpoint address  reset: 0
      uint32_t STAT_TX : 2; // Status bits, for transmission transfers  reset: 0
      uint32_t DTOG_TX : 1; // Data Toggle, for transmission transfers  reset: 0
      uint32_t CTR_TX : 1; // Correct Transfer for transmission  reset: 0
      uint32_t EP_KIND : 1; // Endpoint kind  reset: 0
      uint32_t EP_TYPE : 2; // Endpoint type  reset: 0
      uint32_t SETUP : 1; // Setup transaction completed  reset: 0
      uint32_t STAT_RX : 2; // Status bits, for reception transfers  reset: 0
      uint32_t DTOG_RX : 1; // Data Toggle, for reception transfers  reset: 0
      uint32_t CTR_RX : 1; // Correct transfer for reception  reset: 0
      uint32_t RESERVED_321 : 16;
    };
  } EP4R;
  union{
    volatile uint32_t w;
    struct { // endpoint 5 register @ 0x40005C14 read-write reset: 0x00000000
      uint32_t EA : 4; // Endpoint address  reset: 0
      uint32_t STAT_TX : 2; // Status bits, for transmission transfers  reset: 0
      uint32_t DTOG_TX : 1; // Data Toggle, for transmission transfers  reset: 0
      uint32_t CTR_TX : 1; // Correct Transfer for transmission  reset: 0
      uint32_t EP_KIND : 1; // Endpoint kind  reset: 0
      uint32_t EP_TYPE : 2; // Endpoint type  reset: 0
      uint32_t SETUP : 1; // Setup transaction completed  reset: 0
      uint32_t STAT_RX : 2; // Status bits, for reception transfers  reset: 0
      uint32_t DTOG_RX : 1; // Data Toggle, for reception transfers  reset: 0
      uint32_t CTR_RX : 1; // Correct transfer for reception  reset: 0
      uint32_t RESERVED_322 : 16;
    };
  } EP5R;
  union{
    volatile uint32_t w;
    struct { // endpoint 6 register @ 0x40005C18 read-write reset: 0x00000000
      uint32_t EA : 4; // Endpoint address  reset: 0
      uint32_t STAT_TX : 2; // Status bits, for transmission transfers  reset: 0
      uint32_t DTOG_TX : 1; // Data Toggle, for transmission transfers  reset: 0
      uint32_t CTR_TX : 1; // Correct Transfer for transmission  reset: 0
      uint32_t EP_KIND : 1; // Endpoint kind  reset: 0
      uint32_t EP_TYPE : 2; // Endpoint type  reset: 0
      uint32_t SETUP : 1; // Setup transaction completed  reset: 0
      uint32_t STAT_RX : 2; // Status bits, for reception transfers  reset: 0
      uint32_t DTOG_RX : 1; // Data Toggle, for reception transfers  reset: 0
      uint32_t CTR_RX : 1; // Correct transfer for reception  reset: 0
      uint32_t RESERVED_323 : 16;
    };
  } EP6R;
  union{
    volatile uint32_t w;
    struct { // endpoint 7 register @ 0x40005C1C read-write reset: 0x00000000
      uint32_t EA : 4; // Endpoint address  reset: 0
      uint32_t STAT_TX : 2; // Status bits, for transmission transfers  reset: 0
      uint32_t DTOG_TX : 1; // Data Toggle, for transmission transfers  reset: 0
      uint32_t CTR_TX : 1; // Correct Transfer for transmission  reset: 0
      uint32_t EP_KIND : 1; // Endpoint kind  reset: 0
      uint32_t EP_TYPE : 2; // Endpoint type  reset: 0
      uint32_t SETUP : 1; // Setup transaction completed  reset: 0
      uint32_t STAT_RX : 2; // Status bits, for reception transfers  reset: 0
      uint32_t DTOG_RX : 1; // Data Toggle, for reception transfers  reset: 0
      uint32_t CTR_RX : 1; // Correct transfer for reception  reset: 0
      uint32_t RESERVED_324 : 16;
    };
  } EP7R;
  const uint8_t RESERVED_325[4];
  union{
    volatile uint32_t w;
    struct { // control register @ 0x40005C40 read-write reset: 0x00000003
      uint32_t FRES : 1; // Force USB Reset  reset: 1
      uint32_t PDWN : 1; // Power down  reset: 1
      uint32_t LPMODE : 1; // Low-power mode  reset: 0
      uint32_t FSUSP : 1; // Force suspend  reset: 0
      uint32_t RESUME : 1; // Resume request  reset: 0
      uint32_t RESERVED_326 : 3;
      uint32_t ESOFM : 1; // Expected start of frame interrupt mask  reset: 0
      uint32_t SOFM : 1; // Start of frame interrupt mask  reset: 0
      uint32_t RESETM : 1; // USB reset interrupt mask  reset: 0
      uint32_t SUSPM : 1; // Suspend mode interrupt mask  reset: 0
      uint32_t WKUPM : 1; // Wakeup interrupt mask  reset: 0
      uint32_t ERRM : 1; // Error interrupt mask  reset: 0
      uint32_t PMAOVRM : 1; // Packet memory area over / underrun interrupt mask  reset: 0
      uint32_t CTRM : 1; // Correct transfer interrupt mask  reset: 0
      uint32_t RESERVED_327 : 16;
    };
  } CNTR;
  union{
    volatile uint32_t w;
    struct { // interrupt status register @ 0x40005C44 read-write reset: 0x00000000
      uint32_t EP_ID : 4; // Endpoint Identifier  reset: 0
      uint32_t DIR : 1; // Direction of transaction  reset: 0
      uint32_t RESERVED_328 : 3;
      uint32_t ESOF : 1; // Expected start frame  reset: 0
      uint32_t SOF : 1; // start of frame  reset: 0
      uint32_t RESET : 1; // reset request  reset: 0
      uint32_t SUSP : 1; // Suspend mode request  reset: 0
      uint32_t WKUP : 1; // Wakeup  reset: 0
      uint32_t ERR : 1; // Error  reset: 0
      uint32_t PMAOVR : 1; // Packet memory area over / underrun  reset: 0
      uint32_t CTR : 1; // Correct transfer  reset: 0
      uint32_t RESERVED_329 : 16;
    };
  } ISTR;
  union{
    volatile uint32_t w;
    struct { // frame number register @ 0x40005C48 read-only reset: 0x00000000
      uint32_t FN : 11; // Frame number  reset: 0
      uint32_t LSOF : 2; // Lost SOF  reset: 0
      uint32_t LCK : 1; // Locked  reset: 0
      uint32_t RXDM : 1; // Receive data - line status  reset: 0
      uint32_t RXDP : 1; // Receive data + line status  reset: 0
      uint32_t RESERVED_330 : 16;
    };
  } FNR;
  union{
    volatile uint32_t w;
    struct { // device address @ 0x40005C4C read-write reset: 0x00000000
      uint32_t ADD : 7; // Device address  reset: 0
      uint32_t EF : 1; // Enable function  reset: 0
      uint32_t RESERVED_331 : 24;
    };
  } DADDR;
  union{
    volatile uint32_t w;
    struct { // Buffer table address @ 0x40005C50 read-write reset: 0x00000000
      uint32_t RESERVED_332 : 3;
      uint32_t BTABLE : 13; // Buffer table  reset: 0
      uint32_t RESERVED_333 : 16;
    };
  } BTABLE;
} USB_t;

// CAN register struct @ 0x40006400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // CAN_MCR @ 0x40006400 read-write reset: 0x00000000
      uint32_t INRQ : 1; // INRQ  reset: 0
      uint32_t SLEEP : 1; // SLEEP  reset: 0
      uint32_t TXFP : 1; // TXFP  reset: 0
      uint32_t RFLM : 1; // RFLM  reset: 0
      uint32_t NART : 1; // NART  reset: 0
      uint32_t AWUM : 1; // AWUM  reset: 0
      uint32_t ABOM : 1; // ABOM  reset: 0
      uint32_t TTCM : 1; // TTCM  reset: 0
      uint32_t RESERVED_334 : 7;
      uint32_t RESET : 1; // RESET  reset: 0
      uint32_t DBF : 1; // DBF  reset: 0
      uint32_t RESERVED_335 : 15;
    };
  } CAN_MCR;
  union{
    volatile uint32_t w;
    struct { // CAN_MSR @ 0x40006404  reset: 0x00000000
      uint32_t INAK : 1; // INAK read-only reset: 0
      uint32_t SLAK : 1; // SLAK read-only reset: 0
      uint32_t ERRI : 1; // ERRI read-write reset: 0
      uint32_t WKUI : 1; // WKUI read-write reset: 0
      uint32_t SLAKI : 1; // SLAKI read-write reset: 0
      uint32_t RESERVED_336 : 3;
      uint32_t TXM : 1; // TXM read-only reset: 0
      uint32_t RXM : 1; // RXM read-only reset: 0
      uint32_t SAMP : 1; // SAMP read-only reset: 0
      uint32_t RX : 1; // RX read-only reset: 0
      uint32_t RESERVED_337 : 20;
    };
  } CAN_MSR;
  union{
    volatile uint32_t w;
    struct { // CAN_TSR @ 0x40006408  reset: 0x00000000
      uint32_t RQCP0 : 1; // RQCP0 read-write reset: 0
      uint32_t TXOK0 : 1; // TXOK0 read-write reset: 0
      uint32_t ALST0 : 1; // ALST0 read-write reset: 0
      uint32_t TERR0 : 1; // TERR0 read-write reset: 0
      uint32_t RESERVED_338 : 3;
      uint32_t ABRQ0 : 1; // ABRQ0 read-write reset: 0
      uint32_t RQCP1 : 1; // RQCP1 read-write reset: 0
      uint32_t TXOK1 : 1; // TXOK1 read-write reset: 0
      uint32_t ALST1 : 1; // ALST1 read-write reset: 0
      uint32_t TERR1 : 1; // TERR1 read-write reset: 0
      uint32_t RESERVED_339 : 3;
      uint32_t ABRQ1 : 1; // ABRQ1 read-write reset: 0
      uint32_t RQCP2 : 1; // RQCP2 read-write reset: 0
      uint32_t TXOK2 : 1; // TXOK2 read-write reset: 0
      uint32_t ALST2 : 1; // ALST2 read-write reset: 0
      uint32_t TERR2 : 1; // TERR2 read-write reset: 0
      uint32_t RESERVED_340 : 3;
      uint32_t ABRQ2 : 1; // ABRQ2 read-write reset: 0
      uint32_t CODE : 2; // CODE read-only reset: 0
      uint32_t TME0 : 1; // Lowest priority flag for mailbox 0 read-only reset: 0
      uint32_t TME1 : 1; // Lowest priority flag for mailbox 1 read-only reset: 0
      uint32_t TME2 : 1; // Lowest priority flag for mailbox 2 read-only reset: 0
      uint32_t LOW0 : 1; // Lowest priority flag for mailbox 0 read-only reset: 0
      uint32_t LOW1 : 1; // Lowest priority flag for mailbox 1 read-only reset: 0
      uint32_t LOW2 : 1; // Lowest priority flag for mailbox 2 read-only reset: 0
    };
  } CAN_TSR;
  union{
    volatile uint32_t w;
    struct { // CAN_RF0R @ 0x4000640C  reset: 0x00000000
      uint32_t FMP0 : 2; // FMP0 read-only reset: 0
      uint32_t RESERVED_341 : 1;
      uint32_t FULL0 : 1; // FULL0 read-write reset: 0
      uint32_t FOVR0 : 1; // FOVR0 read-write reset: 0
      uint32_t RFOM0 : 1; // RFOM0 read-write reset: 0
      uint32_t RESERVED_342 : 26;
    };
  } CAN_RF0R;
  union{
    volatile uint32_t w;
    struct { // CAN_RF1R @ 0x40006410  reset: 0x00000000
      uint32_t FMP1 : 2; // FMP1 read-only reset: 0
      uint32_t RESERVED_343 : 1;
      uint32_t FULL1 : 1; // FULL1 read-write reset: 0
      uint32_t FOVR1 : 1; // FOVR1 read-write reset: 0
      uint32_t RFOM1 : 1; // RFOM1 read-write reset: 0
      uint32_t RESERVED_344 : 26;
    };
  } CAN_RF1R;
  union{
    volatile uint32_t w;
    struct { // CAN_IER @ 0x40006414 read-write reset: 0x00000000
      uint32_t TMEIE : 1; // TMEIE  reset: 0
      uint32_t FMPIE0 : 1; // FMPIE0  reset: 0
      uint32_t FFIE0 : 1; // FFIE0  reset: 0
      uint32_t FOVIE0 : 1; // FOVIE0  reset: 0
      uint32_t FMPIE1 : 1; // FMPIE1  reset: 0
      uint32_t FFIE1 : 1; // FFIE1  reset: 0
      uint32_t FOVIE1 : 1; // FOVIE1  reset: 0
      uint32_t RESERVED_345 : 1;
      uint32_t EWGIE : 1; // EWGIE  reset: 0
      uint32_t EPVIE : 1; // EPVIE  reset: 0
      uint32_t BOFIE : 1; // BOFIE  reset: 0
      uint32_t LECIE : 1; // LECIE  reset: 0
      uint32_t RESERVED_346 : 3;
      uint32_t ERRIE : 1; // ERRIE  reset: 0
      uint32_t WKUIE : 1; // WKUIE  reset: 0
      uint32_t SLKIE : 1; // SLKIE  reset: 0
      uint32_t RESERVED_347 : 14;
    };
  } CAN_IER;
  union{
    volatile uint32_t w;
    struct { // CAN_ESR @ 0x40006418  reset: 0x00000000
      uint32_t EWGF : 1; // EWGF read-only reset: 0
      uint32_t EPVF : 1; // EPVF read-only reset: 0
      uint32_t BOFF : 1; // BOFF read-only reset: 0
      uint32_t RESERVED_348 : 1;
      uint32_t LEC : 3; // LEC read-write reset: 0
      uint32_t RESERVED_349 : 9;
      uint32_t TEC : 8; // TEC read-only reset: 0
      uint32_t REC : 8; // REC read-only reset: 0
    };
  } CAN_ESR;
  union{
    volatile uint32_t w;
    struct { // CAN_BTR @ 0x4000641C read-write reset: 0x00000000
      uint32_t BRP : 10; // BRP  reset: 0
      uint32_t RESERVED_350 : 6;
      uint32_t TS1 : 4; // TS1  reset: 0
      uint32_t TS2 : 3; // TS2  reset: 0
      uint32_t RESERVED_351 : 1;
      uint32_t SJW : 2; // SJW  reset: 0
      uint32_t RESERVED_352 : 4;
      uint32_t LBKM : 1; // LBKM  reset: 0
      uint32_t SILM : 1; // SILM  reset: 0
    };
  } CAN_BTR;
  const uint8_t RESERVED_353[324];
  union{
    volatile uint32_t w;
    struct { // CAN_TI0R @ 0x40006580 read-write reset: 0x00000000
      uint32_t TXRQ : 1; // TXRQ  reset: 0
      uint32_t RTR : 1; // RTR  reset: 0
      uint32_t IDE : 1; // IDE  reset: 0
      uint32_t EXID : 18; // EXID  reset: 0
      uint32_t STID : 11; // STID  reset: 0
    };
  } CAN_TI0R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDT0R @ 0x40006584 read-write reset: 0x00000000
      uint32_t DLC : 4; // DLC  reset: 0
      uint32_t RESERVED_354 : 4;
      uint32_t TGT : 1; // TGT  reset: 0
      uint32_t RESERVED_355 : 7;
      uint32_t TIME : 16; // TIME  reset: 0
    };
  } CAN_TDT0R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDL0R @ 0x40006588 read-write reset: 0x00000000
      uint32_t DATA0 : 8; // DATA0  reset: 0
      uint32_t DATA1 : 8; // DATA1  reset: 0
      uint32_t DATA2 : 8; // DATA2  reset: 0
      uint32_t DATA3 : 8; // DATA3  reset: 0
    };
  } CAN_TDL0R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDH0R @ 0x4000658C read-write reset: 0x00000000
      uint32_t DATA4 : 8; // DATA4  reset: 0
      uint32_t DATA5 : 8; // DATA5  reset: 0
      uint32_t DATA6 : 8; // DATA6  reset: 0
      uint32_t DATA7 : 8; // DATA7  reset: 0
    };
  } CAN_TDH0R;
  union{
    volatile uint32_t w;
    struct { // CAN_TI1R @ 0x40006590 read-write reset: 0x00000000
      uint32_t TXRQ : 1; // TXRQ  reset: 0
      uint32_t RTR : 1; // RTR  reset: 0
      uint32_t IDE : 1; // IDE  reset: 0
      uint32_t EXID : 18; // EXID  reset: 0
      uint32_t STID : 11; // STID  reset: 0
    };
  } CAN_TI1R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDT1R @ 0x40006594 read-write reset: 0x00000000
      uint32_t DLC : 4; // DLC  reset: 0
      uint32_t RESERVED_356 : 4;
      uint32_t TGT : 1; // TGT  reset: 0
      uint32_t RESERVED_357 : 7;
      uint32_t TIME : 16; // TIME  reset: 0
    };
  } CAN_TDT1R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDL1R @ 0x40006598 read-write reset: 0x00000000
      uint32_t DATA0 : 8; // DATA0  reset: 0
      uint32_t DATA1 : 8; // DATA1  reset: 0
      uint32_t DATA2 : 8; // DATA2  reset: 0
      uint32_t DATA3 : 8; // DATA3  reset: 0
    };
  } CAN_TDL1R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDH1R @ 0x4000659C read-write reset: 0x00000000
      uint32_t DATA4 : 8; // DATA4  reset: 0
      uint32_t DATA5 : 8; // DATA5  reset: 0
      uint32_t DATA6 : 8; // DATA6  reset: 0
      uint32_t DATA7 : 8; // DATA7  reset: 0
    };
  } CAN_TDH1R;
  union{
    volatile uint32_t w;
    struct { // CAN_TI2R @ 0x400065A0 read-write reset: 0x00000000
      uint32_t TXRQ : 1; // TXRQ  reset: 0
      uint32_t RTR : 1; // RTR  reset: 0
      uint32_t IDE : 1; // IDE  reset: 0
      uint32_t EXID : 18; // EXID  reset: 0
      uint32_t STID : 11; // STID  reset: 0
    };
  } CAN_TI2R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDT2R @ 0x400065A4 read-write reset: 0x00000000
      uint32_t DLC : 4; // DLC  reset: 0
      uint32_t RESERVED_358 : 4;
      uint32_t TGT : 1; // TGT  reset: 0
      uint32_t RESERVED_359 : 7;
      uint32_t TIME : 16; // TIME  reset: 0
    };
  } CAN_TDT2R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDL2R @ 0x400065A8 read-write reset: 0x00000000
      uint32_t DATA0 : 8; // DATA0  reset: 0
      uint32_t DATA1 : 8; // DATA1  reset: 0
      uint32_t DATA2 : 8; // DATA2  reset: 0
      uint32_t DATA3 : 8; // DATA3  reset: 0
    };
  } CAN_TDL2R;
  union{
    volatile uint32_t w;
    struct { // CAN_TDH2R @ 0x400065AC read-write reset: 0x00000000
      uint32_t DATA4 : 8; // DATA4  reset: 0
      uint32_t DATA5 : 8; // DATA5  reset: 0
      uint32_t DATA6 : 8; // DATA6  reset: 0
      uint32_t DATA7 : 8; // DATA7  reset: 0
    };
  } CAN_TDH2R;
  union{
    volatile uint32_t w;
    struct { // CAN_RI0R @ 0x400065B0 read-only reset: 0x00000000
      uint32_t RESERVED_360 : 1;
      uint32_t RTR : 1; // RTR  reset: 0
      uint32_t IDE : 1; // IDE  reset: 0
      uint32_t EXID : 18; // EXID  reset: 0
      uint32_t STID : 11; // STID  reset: 0
    };
  } CAN_RI0R;
  union{
    volatile uint32_t w;
    struct { // CAN_RDT0R @ 0x400065B4 read-only reset: 0x00000000
      uint32_t DLC : 4; // DLC  reset: 0
      uint32_t RESERVED_361 : 4;
      uint32_t FMI : 8; // FMI  reset: 0
      uint32_t TIME : 16; // TIME  reset: 0
    };
  } CAN_RDT0R;
  union{
    volatile uint32_t w;
    struct { // CAN_RDL0R @ 0x400065B8 read-only reset: 0x00000000
      uint32_t DATA0 : 8; // DATA0  reset: 0
      uint32_t DATA1 : 8; // DATA1  reset: 0
      uint32_t DATA2 : 8; // DATA2  reset: 0
      uint32_t DATA3 : 8; // DATA3  reset: 0
    };
  } CAN_RDL0R;
  union{
    volatile uint32_t w;
    struct { // CAN_RDH0R @ 0x400065BC read-only reset: 0x00000000
      uint32_t DATA4 : 8; // DATA4  reset: 0
      uint32_t DATA5 : 8; // DATA5  reset: 0
      uint32_t DATA6 : 8; // DATA6  reset: 0
      uint32_t DATA7 : 8; // DATA7  reset: 0
    };
  } CAN_RDH0R;
  union{
    volatile uint32_t w;
    struct { // CAN_RI1R @ 0x400065C0 read-only reset: 0x00000000
      uint32_t RESERVED_362 : 1;
      uint32_t RTR : 1; // RTR  reset: 0
      uint32_t IDE : 1; // IDE  reset: 0
      uint32_t EXID : 18; // EXID  reset: 0
      uint32_t STID : 11; // STID  reset: 0
    };
  } CAN_RI1R;
  union{
    volatile uint32_t w;
    struct { // CAN_RDT1R @ 0x400065C4 read-only reset: 0x00000000
      uint32_t DLC : 4; // DLC  reset: 0
      uint32_t RESERVED_363 : 4;
      uint32_t FMI : 8; // FMI  reset: 0
      uint32_t TIME : 16; // TIME  reset: 0
    };
  } CAN_RDT1R;
  union{
    volatile uint32_t w;
    struct { // CAN_RDL1R @ 0x400065C8 read-only reset: 0x00000000
      uint32_t DATA0 : 8; // DATA0  reset: 0
      uint32_t DATA1 : 8; // DATA1  reset: 0
      uint32_t DATA2 : 8; // DATA2  reset: 0
      uint32_t DATA3 : 8; // DATA3  reset: 0
    };
  } CAN_RDL1R;
  union{
    volatile uint32_t w;
    struct { // CAN_RDH1R @ 0x400065CC read-only reset: 0x00000000
      uint32_t DATA4 : 8; // DATA4  reset: 0
      uint32_t DATA5 : 8; // DATA5  reset: 0
      uint32_t DATA6 : 8; // DATA6  reset: 0
      uint32_t DATA7 : 8; // DATA7  reset: 0
    };
  } CAN_RDH1R;
  const uint8_t RESERVED_364[20];
  union{
    volatile uint32_t w;
    struct { // CAN_FMR @ 0x40006600 read-write reset: 0x00000000
      uint32_t FINIT : 1; // FINIT  reset: 0
      uint32_t RESERVED_365 : 31;
    };
  } CAN_FMR;
  union{
    volatile uint32_t w;
    struct { // CAN_FM1R @ 0x40006604 read-write reset: 0x00000000
      uint32_t FBM0 : 1; // Filter mode  reset: 0
      uint32_t FBM1 : 1; // Filter mode  reset: 0
      uint32_t FBM2 : 1; // Filter mode  reset: 0
      uint32_t FBM3 : 1; // Filter mode  reset: 0
      uint32_t FBM4 : 1; // Filter mode  reset: 0
      uint32_t FBM5 : 1; // Filter mode  reset: 0
      uint32_t FBM6 : 1; // Filter mode  reset: 0
      uint32_t FBM7 : 1; // Filter mode  reset: 0
      uint32_t FBM8 : 1; // Filter mode  reset: 0
      uint32_t FBM9 : 1; // Filter mode  reset: 0
      uint32_t FBM10 : 1; // Filter mode  reset: 0
      uint32_t FBM11 : 1; // Filter mode  reset: 0
      uint32_t FBM12 : 1; // Filter mode  reset: 0
      uint32_t FBM13 : 1; // Filter mode  reset: 0
      uint32_t RESERVED_366 : 18;
    };
  } CAN_FM1R;
  union{
    volatile uint32_t w;
    struct { // CAN_FS1R @ 0x4000660C read-write reset: 0x00000000
      uint32_t FSC0 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC1 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC2 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC3 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC4 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC5 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC6 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC7 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC8 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC9 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC10 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC11 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC12 : 1; // Filter scale configuration  reset: 0
      uint32_t FSC13 : 1; // Filter scale configuration  reset: 0
      uint32_t RESERVED_367 : 18;
    };
  } CAN_FS1R;
  union{
    volatile uint32_t w;
    struct { // CAN_FFA1R @ 0x40006614 read-write reset: 0x00000000
      uint32_t FFA0 : 1; // Filter FIFO assignment for filter 0  reset: 0
      uint32_t FFA1 : 1; // Filter FIFO assignment for filter 1  reset: 0
      uint32_t FFA2 : 1; // Filter FIFO assignment for filter 2  reset: 0
      uint32_t FFA3 : 1; // Filter FIFO assignment for filter 3  reset: 0
      uint32_t FFA4 : 1; // Filter FIFO assignment for filter 4  reset: 0
      uint32_t FFA5 : 1; // Filter FIFO assignment for filter 5  reset: 0
      uint32_t FFA6 : 1; // Filter FIFO assignment for filter 6  reset: 0
      uint32_t FFA7 : 1; // Filter FIFO assignment for filter 7  reset: 0
      uint32_t FFA8 : 1; // Filter FIFO assignment for filter 8  reset: 0
      uint32_t FFA9 : 1; // Filter FIFO assignment for filter 9  reset: 0
      uint32_t FFA10 : 1; // Filter FIFO assignment for filter 10  reset: 0
      uint32_t FFA11 : 1; // Filter FIFO assignment for filter 11  reset: 0
      uint32_t FFA12 : 1; // Filter FIFO assignment for filter 12  reset: 0
      uint32_t FFA13 : 1; // Filter FIFO assignment for filter 13  reset: 0
      uint32_t RESERVED_368 : 18;
    };
  } CAN_FFA1R;
  union{
    volatile uint32_t w;
    struct { // CAN_FA1R @ 0x4000661C read-write reset: 0x00000000
      uint32_t FACT0 : 1; // Filter active  reset: 0
      uint32_t FACT1 : 1; // Filter active  reset: 0
      uint32_t FACT2 : 1; // Filter active  reset: 0
      uint32_t FACT3 : 1; // Filter active  reset: 0
      uint32_t FACT4 : 1; // Filter active  reset: 0
      uint32_t FACT5 : 1; // Filter active  reset: 0
      uint32_t FACT6 : 1; // Filter active  reset: 0
      uint32_t FACT7 : 1; // Filter active  reset: 0
      uint32_t FACT8 : 1; // Filter active  reset: 0
      uint32_t FACT9 : 1; // Filter active  reset: 0
      uint32_t FACT10 : 1; // Filter active  reset: 0
      uint32_t FACT11 : 1; // Filter active  reset: 0
      uint32_t FACT12 : 1; // Filter active  reset: 0
      uint32_t FACT13 : 1; // Filter active  reset: 0
      uint32_t RESERVED_369 : 18;
    };
  } CAN_FA1R;
  const uint8_t RESERVED_370[4];
  union{
    volatile uint32_t w;
    struct { // Filter bank 0 register 1 @ 0x40006640 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F0R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 0 register 2 @ 0x40006644 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F0R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 1 register 1 @ 0x40006648 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F1R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 1 register 2 @ 0x4000664C read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F1R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 2 register 1 @ 0x40006650 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F2R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 2 register 2 @ 0x40006654 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F2R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 3 register 1 @ 0x40006658 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F3R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 3 register 2 @ 0x4000665C read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F3R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 4 register 1 @ 0x40006660 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F4R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 4 register 2 @ 0x40006664 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F4R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 5 register 1 @ 0x40006668 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F5R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 5 register 2 @ 0x4000666C read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F5R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 6 register 1 @ 0x40006670 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F6R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 6 register 2 @ 0x40006674 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F6R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 7 register 1 @ 0x40006678 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F7R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 7 register 2 @ 0x4000667C read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F7R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 8 register 1 @ 0x40006680 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F8R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 8 register 2 @ 0x40006684 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F8R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 9 register 1 @ 0x40006688 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F9R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 9 register 2 @ 0x4000668C read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F9R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 10 register 1 @ 0x40006690 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F10R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 10 register 2 @ 0x40006694 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F10R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 11 register 1 @ 0x40006698 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F11R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 11 register 2 @ 0x4000669C read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F11R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 4 register 1 @ 0x400066A0 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F12R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 12 register 2 @ 0x400066A4 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F12R2;
  union{
    volatile uint32_t w;
    struct { // Filter bank 13 register 1 @ 0x400066A8 read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F13R1;
  union{
    volatile uint32_t w;
    struct { // Filter bank 13 register 2 @ 0x400066AC read-write reset: 0x00000000
      uint32_t FB0 : 1; // Filter bits  reset: 0
      uint32_t FB1 : 1; // Filter bits  reset: 0
      uint32_t FB2 : 1; // Filter bits  reset: 0
      uint32_t FB3 : 1; // Filter bits  reset: 0
      uint32_t FB4 : 1; // Filter bits  reset: 0
      uint32_t FB5 : 1; // Filter bits  reset: 0
      uint32_t FB6 : 1; // Filter bits  reset: 0
      uint32_t FB7 : 1; // Filter bits  reset: 0
      uint32_t FB8 : 1; // Filter bits  reset: 0
      uint32_t FB9 : 1; // Filter bits  reset: 0
      uint32_t FB10 : 1; // Filter bits  reset: 0
      uint32_t FB11 : 1; // Filter bits  reset: 0
      uint32_t FB12 : 1; // Filter bits  reset: 0
      uint32_t FB13 : 1; // Filter bits  reset: 0
      uint32_t FB14 : 1; // Filter bits  reset: 0
      uint32_t FB15 : 1; // Filter bits  reset: 0
      uint32_t FB16 : 1; // Filter bits  reset: 0
      uint32_t FB17 : 1; // Filter bits  reset: 0
      uint32_t FB18 : 1; // Filter bits  reset: 0
      uint32_t FB19 : 1; // Filter bits  reset: 0
      uint32_t FB20 : 1; // Filter bits  reset: 0
      uint32_t FB21 : 1; // Filter bits  reset: 0
      uint32_t FB22 : 1; // Filter bits  reset: 0
      uint32_t FB23 : 1; // Filter bits  reset: 0
      uint32_t FB24 : 1; // Filter bits  reset: 0
      uint32_t FB25 : 1; // Filter bits  reset: 0
      uint32_t FB26 : 1; // Filter bits  reset: 0
      uint32_t FB27 : 1; // Filter bits  reset: 0
      uint32_t FB28 : 1; // Filter bits  reset: 0
      uint32_t FB29 : 1; // Filter bits  reset: 0
      uint32_t FB30 : 1; // Filter bits  reset: 0
      uint32_t FB31 : 1; // Filter bits  reset: 0
    };
  } F13R2;
} CAN_t;

// BKP register struct @ 0x40006C04
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C04 read-write reset: 0x00000000
      uint32_t D1 : 16; // Backup data  reset: 0
      uint32_t RESERVED_371 : 16;
    };
  } DR1;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C08 read-write reset: 0x00000000
      uint32_t D2 : 16; // Backup data  reset: 0
      uint32_t RESERVED_372 : 16;
    };
  } DR2;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C0C read-write reset: 0x00000000
      uint32_t D3 : 16; // Backup data  reset: 0
      uint32_t RESERVED_373 : 16;
    };
  } DR3;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C10 read-write reset: 0x00000000
      uint32_t D4 : 16; // Backup data  reset: 0
      uint32_t RESERVED_374 : 16;
    };
  } DR4;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C14 read-write reset: 0x00000000
      uint32_t D5 : 16; // Backup data  reset: 0
      uint32_t RESERVED_375 : 16;
    };
  } DR5;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C18 read-write reset: 0x00000000
      uint32_t D6 : 16; // Backup data  reset: 0
      uint32_t RESERVED_376 : 16;
    };
  } DR6;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C1C read-write reset: 0x00000000
      uint32_t D7 : 16; // Backup data  reset: 0
      uint32_t RESERVED_377 : 16;
    };
  } DR7;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C20 read-write reset: 0x00000000
      uint32_t D8 : 16; // Backup data  reset: 0
      uint32_t RESERVED_378 : 16;
    };
  } DR8;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C24 read-write reset: 0x00000000
      uint32_t D9 : 16; // Backup data  reset: 0
      uint32_t RESERVED_379 : 16;
    };
  } DR9;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C28 read-write reset: 0x00000000
      uint32_t D10 : 16; // Backup data  reset: 0
      uint32_t RESERVED_380 : 16;
    };
  } DR10;
  union{
    volatile uint32_t w;
    struct { // RTC clock calibration register (BKP_RTCCR) @ 0x40006C2C read-write reset: 0x00000000
      uint32_t CAL : 7; // Calibration value  reset: 0
      uint32_t CCO : 1; // Calibration Clock Output  reset: 0
      uint32_t ASOE : 1; // Alarm or second output enable  reset: 0
      uint32_t ASOS : 1; // Alarm or second output selection  reset: 0
      uint32_t RESERVED_381 : 22;
    };
  } RTCCR;
  union{
    volatile uint32_t w;
    struct { // Backup control register (BKP_CR) @ 0x40006C30 read-write reset: 0x00000000
      uint32_t TPE : 1; // Tamper pin enable  reset: 0
      uint32_t TPAL : 1; // Tamper pin active level  reset: 0
      uint32_t RESERVED_382 : 30;
    };
  } CR;
  union{
    volatile uint32_t w;
    struct { // BKP_CSR control/status register (BKP_CSR) @ 0x40006C34  reset: 0x00000000
      uint32_t CTE : 1; // Clear Tamper event write-only reset: 0
      uint32_t CTI : 1; // Clear Tamper Interrupt write-only reset: 0
      uint32_t TPIE : 1; // Tamper Pin interrupt enable read-write reset: 0
      uint32_t RESERVED_383 : 5;
      uint32_t TEF : 1; // Tamper Event Flag read-only reset: 0
      uint32_t TIF : 1; // Tamper Interrupt Flag read-only reset: 0
      uint32_t RESERVED_384 : 22;
    };
  } CSR;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C40 read-write reset: 0x00000000
      uint32_t DR11 : 16; // Backup data  reset: 0
      uint32_t RESERVED_385 : 16;
    };
  } DR11;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C44 read-write reset: 0x00000000
      uint32_t DR12 : 16; // Backup data  reset: 0
      uint32_t RESERVED_386 : 16;
    };
  } DR12;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C48 read-write reset: 0x00000000
      uint32_t DR13 : 16; // Backup data  reset: 0
      uint32_t RESERVED_387 : 16;
    };
  } DR13;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C4C read-write reset: 0x00000000
      uint32_t D14 : 16; // Backup data  reset: 0
      uint32_t RESERVED_388 : 16;
    };
  } DR14;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C50 read-write reset: 0x00000000
      uint32_t D15 : 16; // Backup data  reset: 0
      uint32_t RESERVED_389 : 16;
    };
  } DR15;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C54 read-write reset: 0x00000000
      uint32_t D16 : 16; // Backup data  reset: 0
      uint32_t RESERVED_390 : 16;
    };
  } DR16;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C58 read-write reset: 0x00000000
      uint32_t D17 : 16; // Backup data  reset: 0
      uint32_t RESERVED_391 : 16;
    };
  } DR17;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C5C read-write reset: 0x00000000
      uint32_t D18 : 16; // Backup data  reset: 0
      uint32_t RESERVED_392 : 16;
    };
  } DR18;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C60 read-write reset: 0x00000000
      uint32_t D19 : 16; // Backup data  reset: 0
      uint32_t RESERVED_393 : 16;
    };
  } DR19;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C64 read-write reset: 0x00000000
      uint32_t D20 : 16; // Backup data  reset: 0
      uint32_t RESERVED_394 : 16;
    };
  } DR20;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C68 read-write reset: 0x00000000
      uint32_t D21 : 16; // Backup data  reset: 0
      uint32_t RESERVED_395 : 16;
    };
  } DR21;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C6C read-write reset: 0x00000000
      uint32_t D22 : 16; // Backup data  reset: 0
      uint32_t RESERVED_396 : 16;
    };
  } DR22;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C70 read-write reset: 0x00000000
      uint32_t D23 : 16; // Backup data  reset: 0
      uint32_t RESERVED_397 : 16;
    };
  } DR23;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C74 read-write reset: 0x00000000
      uint32_t D24 : 16; // Backup data  reset: 0
      uint32_t RESERVED_398 : 16;
    };
  } DR24;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C78 read-write reset: 0x00000000
      uint32_t D25 : 16; // Backup data  reset: 0
      uint32_t RESERVED_399 : 16;
    };
  } DR25;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C7C read-write reset: 0x00000000
      uint32_t D26 : 16; // Backup data  reset: 0
      uint32_t RESERVED_400 : 16;
    };
  } DR26;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C80 read-write reset: 0x00000000
      uint32_t D27 : 16; // Backup data  reset: 0
      uint32_t RESERVED_401 : 16;
    };
  } DR27;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C84 read-write reset: 0x00000000
      uint32_t D28 : 16; // Backup data  reset: 0
      uint32_t RESERVED_402 : 16;
    };
  } DR28;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C88 read-write reset: 0x00000000
      uint32_t D29 : 16; // Backup data  reset: 0
      uint32_t RESERVED_403 : 16;
    };
  } DR29;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C8C read-write reset: 0x00000000
      uint32_t D30 : 16; // Backup data  reset: 0
      uint32_t RESERVED_404 : 16;
    };
  } DR30;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C90 read-write reset: 0x00000000
      uint32_t D31 : 16; // Backup data  reset: 0
      uint32_t RESERVED_405 : 16;
    };
  } DR31;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C94 read-write reset: 0x00000000
      uint32_t D32 : 16; // Backup data  reset: 0
      uint32_t RESERVED_406 : 16;
    };
  } DR32;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C98 read-write reset: 0x00000000
      uint32_t D33 : 16; // Backup data  reset: 0
      uint32_t RESERVED_407 : 16;
    };
  } DR33;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006C9C read-write reset: 0x00000000
      uint32_t D34 : 16; // Backup data  reset: 0
      uint32_t RESERVED_408 : 16;
    };
  } DR34;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006CA0 read-write reset: 0x00000000
      uint32_t D35 : 16; // Backup data  reset: 0
      uint32_t RESERVED_409 : 16;
    };
  } DR35;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006CA4 read-write reset: 0x00000000
      uint32_t D36 : 16; // Backup data  reset: 0
      uint32_t RESERVED_410 : 16;
    };
  } DR36;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006CA8 read-write reset: 0x00000000
      uint32_t D37 : 16; // Backup data  reset: 0
      uint32_t RESERVED_411 : 16;
    };
  } DR37;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006CAC read-write reset: 0x00000000
      uint32_t D38 : 16; // Backup data  reset: 0
      uint32_t RESERVED_412 : 16;
    };
  } DR38;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006CB0 read-write reset: 0x00000000
      uint32_t D39 : 16; // Backup data  reset: 0
      uint32_t RESERVED_413 : 16;
    };
  } DR39;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006CB4 read-write reset: 0x00000000
      uint32_t D40 : 16; // Backup data  reset: 0
      uint32_t RESERVED_414 : 16;
    };
  } DR40;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006CB8 read-write reset: 0x00000000
      uint32_t D41 : 16; // Backup data  reset: 0
      uint32_t RESERVED_415 : 16;
    };
  } DR41;
  union{
    volatile uint32_t w;
    struct { // Backup data register (BKP_DR) @ 0x40006CBC read-write reset: 0x00000000
      uint32_t D42 : 16; // Backup data  reset: 0
      uint32_t RESERVED_416 : 16;
    };
  } DR42;
} BKP_t;

// PWR register struct @ 0x40007000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Power control register (PWR_CR) @ 0x40007000 read-write reset: 0x00000000
      uint32_t LPDS : 1; // Low Power Deep Sleep  reset: 0
      uint32_t PDDS : 1; // Power Down Deep Sleep  reset: 0
      uint32_t CWUF : 1; // Clear Wake-up Flag  reset: 0
      uint32_t CSBF : 1; // Clear STANDBY Flag  reset: 0
      uint32_t PVDE : 1; // Power Voltage Detector Enable  reset: 0
      uint32_t PLS : 3; // PVD Level Selection  reset: 0
      uint32_t DBP : 1; // Disable Backup Domain write protection  reset: 0
      uint32_t RESERVED_417 : 23;
    };
  } CR;
  union{
    volatile uint32_t w;
    struct { // Power control register (PWR_CR) @ 0x40007004  reset: 0x00000000
      uint32_t WUF : 1; // Wake-Up Flag read-only reset: 0
      uint32_t SBF : 1; // STANDBY Flag read-only reset: 0
      uint32_t PVDO : 1; // PVD Output read-only reset: 0
      uint32_t RESERVED_418 : 5;
      uint32_t EWUP : 1; // Enable WKUP pin read-write reset: 0
      uint32_t RESERVED_419 : 23;
    };
  } CSR;
} PWR_t;

// DAC register struct @ 0x40007400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Control register (DAC_CR) @ 0x40007400 read-write reset: 0x00000000
      uint32_t EN1 : 1; // DAC channel1 enable  reset: 0
      uint32_t BOFF1 : 1; // DAC channel1 output buffer disable  reset: 0
      uint32_t TEN1 : 1; // DAC channel1 trigger enable  reset: 0
      uint32_t TSEL1 : 3; // DAC channel1 trigger selection  reset: 0
      uint32_t WAVE1 : 2; // DAC channel1 noise/triangle wave generation enable  reset: 0
      uint32_t MAMP1 : 4; // DAC channel1 mask/amplitude selector  reset: 0
      uint32_t DMAEN1 : 1; // DAC channel1 DMA enable  reset: 0
      uint32_t RESERVED_420 : 3;
      uint32_t EN2 : 1; // DAC channel2 enable  reset: 0
      uint32_t BOFF2 : 1; // DAC channel2 output buffer disable  reset: 0
      uint32_t TEN2 : 1; // DAC channel2 trigger enable  reset: 0
      uint32_t TSEL2 : 3; // DAC channel2 trigger selection  reset: 0
      uint32_t WAVE2 : 2; // DAC channel2 noise/triangle wave generation enable  reset: 0
      uint32_t MAMP2 : 4; // DAC channel2 mask/amplitude selector  reset: 0
      uint32_t DMAEN2 : 1; // DAC channel2 DMA enable  reset: 0
      uint32_t RESERVED_421 : 3;
    };
  } CR;
  union{
    volatile uint32_t w;
    struct { // DAC software trigger register (DAC_SWTRIGR) @ 0x40007404 write-only reset: 0x00000000
      uint32_t SWTRIG1 : 1; // DAC channel1 software trigger  reset: 0
      uint32_t SWTRIG2 : 1; // DAC channel2 software trigger  reset: 0
      uint32_t RESERVED_422 : 30;
    };
  } SWTRIGR;
  union{
    volatile uint32_t w;
    struct { // DAC channel1 12-bit right-aligned data holding register(DAC_DHR12R1) @ 0x40007408 read-write reset: 0x00000000
      uint32_t DACC1DHR : 12; // DAC channel1 12-bit right-aligned data  reset: 0
      uint32_t RESERVED_423 : 20;
    };
  } DHR12R1;
  union{
    volatile uint32_t w;
    struct { // DAC channel1 12-bit left aligned data holding register (DAC_DHR12L1) @ 0x4000740C read-write reset: 0x00000000
      uint32_t RESERVED_424 : 4;
      uint32_t DACC1DHR : 12; // DAC channel1 12-bit left-aligned data  reset: 0
      uint32_t RESERVED_425 : 16;
    };
  } DHR12L1;
  union{
    volatile uint32_t w;
    struct { // DAC channel1 8-bit right aligned data holding register (DAC_DHR8R1) @ 0x40007410 read-write reset: 0x00000000
      uint32_t DACC1DHR : 8; // DAC channel1 8-bit right-aligned data  reset: 0
      uint32_t RESERVED_426 : 24;
    };
  } DHR8R1;
  union{
    volatile uint32_t w;
    struct { // DAC channel2 12-bit right aligned data holding register (DAC_DHR12R2) @ 0x40007414 read-write reset: 0x00000000
      uint32_t DACC2DHR : 12; // DAC channel2 12-bit right-aligned data  reset: 0
      uint32_t RESERVED_427 : 20;
    };
  } DHR12R2;
  union{
    volatile uint32_t w;
    struct { // DAC channel2 12-bit left aligned data holding register (DAC_DHR12L2) @ 0x40007418 read-write reset: 0x00000000
      uint32_t RESERVED_428 : 4;
      uint32_t DACC2DHR : 12; // DAC channel2 12-bit left-aligned data  reset: 0
      uint32_t RESERVED_429 : 16;
    };
  } DHR12L2;
  union{
    volatile uint32_t w;
    struct { // DAC channel2 8-bit right-aligned data holding register (DAC_DHR8R2) @ 0x4000741C read-write reset: 0x00000000
      uint32_t DACC2DHR : 8; // DAC channel2 8-bit right-aligned data  reset: 0
      uint32_t RESERVED_430 : 24;
    };
  } DHR8R2;
  union{
    volatile uint32_t w;
    struct { // Dual DAC 12-bit right-aligned data holding register (DAC_DHR12RD), Bits 31:28 Reserved, Bits 15:12 Reserved @ 0x40007420 read-write reset: 0x00000000
      uint32_t DACC1DHR : 12; // DAC channel1 12-bit right-aligned data  reset: 0
      uint32_t RESERVED_431 : 4;
      uint32_t DACC2DHR : 12; // DAC channel2 12-bit right-aligned data  reset: 0
      uint32_t RESERVED_432 : 4;
    };
  } DHR12RD;
  union{
    volatile uint32_t w;
    struct { // DUAL DAC 12-bit left aligned data holding register (DAC_DHR12LD), Bits 19:16 Reserved, Bits 3:0 Reserved @ 0x40007424 read-write reset: 0x00000000
      uint32_t RESERVED_433 : 4;
      uint32_t DACC1DHR : 12; // DAC channel1 12-bit left-aligned data  reset: 0
      uint32_t RESERVED_434 : 4;
      uint32_t DACC2DHR : 12; // DAC channel2 12-bit right-aligned data  reset: 0
    };
  } DHR12LD;
  union{
    volatile uint32_t w;
    struct { // DUAL DAC 8-bit right aligned data holding register (DAC_DHR8RD), Bits 31:16 Reserved @ 0x40007428 read-write reset: 0x00000000
      uint32_t DACC1DHR : 8; // DAC channel1 8-bit right-aligned data  reset: 0
      uint32_t DACC2DHR : 8; // DAC channel2 8-bit right-aligned data  reset: 0
      uint32_t RESERVED_435 : 16;
    };
  } DHR8RD;
  union{
    volatile uint32_t w;
    struct { // DAC channel1 data output register (DAC_DOR1) @ 0x4000742C read-only reset: 0x00000000
      uint32_t DACC1DOR : 12; // DAC channel1 data output  reset: 0
      uint32_t RESERVED_436 : 20;
    };
  } DOR1;
  union{
    volatile uint32_t w;
    struct { // DAC channel2 data output register (DAC_DOR2) @ 0x40007430 read-only reset: 0x00000000
      uint32_t DACC2DOR : 12; // DAC channel2 data output  reset: 0
      uint32_t RESERVED_437 : 20;
    };
  } DOR2;
} DAC_t;

// AFIO register struct @ 0x40010000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Event Control Register (AFIO_EVCR) @ 0x40010000 read-write reset: 0x00000000
      uint32_t PIN : 4; // Pin selection  reset: 0
      uint32_t PORT : 3; // Port selection  reset: 0
      uint32_t EVOE : 1; // Event Output Enable  reset: 0
      uint32_t RESERVED_438 : 24;
    };
  } EVCR;
  union{
    volatile uint32_t w;
    struct { // AF remap and debug I/O configuration register (AFIO_MAPR) @ 0x40010004  reset: 0x00000000
      uint32_t SPI1_REMAP : 1; // SPI1 remapping read-write reset: 0
      uint32_t I2C1_REMAP : 1; // I2C1 remapping read-write reset: 0
      uint32_t USART1_REMAP : 1; // USART1 remapping read-write reset: 0
      uint32_t USART2_REMAP : 1; // USART2 remapping read-write reset: 0
      uint32_t USART3_REMAP : 2; // USART3 remapping read-write reset: 0
      uint32_t TIM1_REMAP : 2; // TIM1 remapping read-write reset: 0
      uint32_t TIM2_REMAP : 2; // TIM2 remapping read-write reset: 0
      uint32_t TIM3_REMAP : 2; // TIM3 remapping read-write reset: 0
      uint32_t TIM4_REMAP : 1; // TIM4 remapping read-write reset: 0
      uint32_t CAN_REMAP : 2; // CAN1 remapping read-write reset: 0
      uint32_t PD01_REMAP : 1; // Port D0/Port D1 mapping on OSCIN/OSCOUT read-write reset: 0
      uint32_t TIM5CH4_IREMAP : 1; // Set and cleared by software read-write reset: 0
      uint32_t ADC1_ETRGINJ_REMAP : 1; // ADC 1 External trigger injected conversion remapping read-write reset: 0
      uint32_t ADC1_ETRGREG_REMAP : 1; // ADC 1 external trigger regular conversion remapping read-write reset: 0
      uint32_t ADC2_ETRGINJ_REMAP : 1; // ADC 2 external trigger injected conversion remapping read-write reset: 0
      uint32_t ADC2_ETRGREG_REMAP : 1; // ADC 2 external trigger regular conversion remapping read-write reset: 0
      uint32_t RESERVED_439 : 3;
      uint32_t SWJ_CFG : 3; // Serial wire JTAG configuration write-only reset: 0
      uint32_t RESERVED_440 : 5;
    };
  } MAPR;
  union{
    volatile uint32_t w;
    struct { // External interrupt configuration register 1 (AFIO_EXTICR1) @ 0x40010008 read-write reset: 0x00000000
      uint32_t EXTI0 : 4; // EXTI0 configuration  reset: 0
      uint32_t EXTI1 : 4; // EXTI1 configuration  reset: 0
      uint32_t EXTI2 : 4; // EXTI2 configuration  reset: 0
      uint32_t EXTI3 : 4; // EXTI3 configuration  reset: 0
      uint32_t RESERVED_441 : 16;
    };
  } EXTICR1;
  union{
    volatile uint32_t w;
    struct { // External interrupt configuration register 2 (AFIO_EXTICR2) @ 0x4001000C read-write reset: 0x00000000
      uint32_t EXTI4 : 4; // EXTI4 configuration  reset: 0
      uint32_t EXTI5 : 4; // EXTI5 configuration  reset: 0
      uint32_t EXTI6 : 4; // EXTI6 configuration  reset: 0
      uint32_t EXTI7 : 4; // EXTI7 configuration  reset: 0
      uint32_t RESERVED_442 : 16;
    };
  } EXTICR2;
  union{
    volatile uint32_t w;
    struct { // External interrupt configuration register 3 (AFIO_EXTICR3) @ 0x40010010 read-write reset: 0x00000000
      uint32_t EXTI8 : 4; // EXTI8 configuration  reset: 0
      uint32_t EXTI9 : 4; // EXTI9 configuration  reset: 0
      uint32_t EXTI10 : 4; // EXTI10 configuration  reset: 0
      uint32_t EXTI11 : 4; // EXTI11 configuration  reset: 0
      uint32_t RESERVED_443 : 16;
    };
  } EXTICR3;
  union{
    volatile uint32_t w;
    struct { // External interrupt configuration register 4 (AFIO_EXTICR4) @ 0x40010014 read-write reset: 0x00000000
      uint32_t EXTI12 : 4; // EXTI12 configuration  reset: 0
      uint32_t EXTI13 : 4; // EXTI13 configuration  reset: 0
      uint32_t EXTI14 : 4; // EXTI14 configuration  reset: 0
      uint32_t EXTI15 : 4; // EXTI15 configuration  reset: 0
      uint32_t RESERVED_444 : 16;
    };
  } EXTICR4;
  union{
    volatile uint32_t w;
    struct { // AF remap and debug I/O configuration register @ 0x4001001C read-write reset: 0x00000000
      uint32_t RESERVED_445 : 5;
      uint32_t TIM9_REMAP : 1; // TIM9 remapping  reset: 0
      uint32_t TIM10_REMAP : 1; // TIM10 remapping  reset: 0
      uint32_t TIM11_REMAP : 1; // TIM11 remapping  reset: 0
      uint32_t TIM13_REMAP : 1; // TIM13 remapping  reset: 0
      uint32_t TIM14_REMAP : 1; // TIM14 remapping  reset: 0
      uint32_t FSMC_NADV : 1; // NADV connect/disconnect  reset: 0
      uint32_t RESERVED_446 : 21;
    };
  } MAPR2;
} AFIO_t;

// EXTI register struct @ 0x40010400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Interrupt mask register (EXTI_IMR) @ 0x40010400 read-write reset: 0x00000000
      uint32_t MR0 : 1; // Interrupt Mask on line 0  reset: 0
      uint32_t MR1 : 1; // Interrupt Mask on line 1  reset: 0
      uint32_t MR2 : 1; // Interrupt Mask on line 2  reset: 0
      uint32_t MR3 : 1; // Interrupt Mask on line 3  reset: 0
      uint32_t MR4 : 1; // Interrupt Mask on line 4  reset: 0
      uint32_t MR5 : 1; // Interrupt Mask on line 5  reset: 0
      uint32_t MR6 : 1; // Interrupt Mask on line 6  reset: 0
      uint32_t MR7 : 1; // Interrupt Mask on line 7  reset: 0
      uint32_t MR8 : 1; // Interrupt Mask on line 8  reset: 0
      uint32_t MR9 : 1; // Interrupt Mask on line 9  reset: 0
      uint32_t MR10 : 1; // Interrupt Mask on line 10  reset: 0
      uint32_t MR11 : 1; // Interrupt Mask on line 11  reset: 0
      uint32_t MR12 : 1; // Interrupt Mask on line 12  reset: 0
      uint32_t MR13 : 1; // Interrupt Mask on line 13  reset: 0
      uint32_t MR14 : 1; // Interrupt Mask on line 14  reset: 0
      uint32_t MR15 : 1; // Interrupt Mask on line 15  reset: 0
      uint32_t MR16 : 1; // Interrupt Mask on line 16  reset: 0
      uint32_t MR17 : 1; // Interrupt Mask on line 17  reset: 0
      uint32_t MR18 : 1; // Interrupt Mask on line 18  reset: 0
      uint32_t RESERVED_447 : 13;
    };
  } IMR;
  union{
    volatile uint32_t w;
    struct { // Event mask register (EXTI_EMR) @ 0x40010404 read-write reset: 0x00000000
      uint32_t MR0 : 1; // Event Mask on line 0  reset: 0
      uint32_t MR1 : 1; // Event Mask on line 1  reset: 0
      uint32_t MR2 : 1; // Event Mask on line 2  reset: 0
      uint32_t MR3 : 1; // Event Mask on line 3  reset: 0
      uint32_t MR4 : 1; // Event Mask on line 4  reset: 0
      uint32_t MR5 : 1; // Event Mask on line 5  reset: 0
      uint32_t MR6 : 1; // Event Mask on line 6  reset: 0
      uint32_t MR7 : 1; // Event Mask on line 7  reset: 0
      uint32_t MR8 : 1; // Event Mask on line 8  reset: 0
      uint32_t MR9 : 1; // Event Mask on line 9  reset: 0
      uint32_t MR10 : 1; // Event Mask on line 10  reset: 0
      uint32_t MR11 : 1; // Event Mask on line 11  reset: 0
      uint32_t MR12 : 1; // Event Mask on line 12  reset: 0
      uint32_t MR13 : 1; // Event Mask on line 13  reset: 0
      uint32_t MR14 : 1; // Event Mask on line 14  reset: 0
      uint32_t MR15 : 1; // Event Mask on line 15  reset: 0
      uint32_t MR16 : 1; // Event Mask on line 16  reset: 0
      uint32_t MR17 : 1; // Event Mask on line 17  reset: 0
      uint32_t MR18 : 1; // Event Mask on line 18  reset: 0
      uint32_t RESERVED_448 : 13;
    };
  } EMR;
  union{
    volatile uint32_t w;
    struct { // Rising Trigger selection register (EXTI_RTSR) @ 0x40010408 read-write reset: 0x00000000
      uint32_t TR0 : 1; // Rising trigger event configuration of line 0  reset: 0
      uint32_t TR1 : 1; // Rising trigger event configuration of line 1  reset: 0
      uint32_t TR2 : 1; // Rising trigger event configuration of line 2  reset: 0
      uint32_t TR3 : 1; // Rising trigger event configuration of line 3  reset: 0
      uint32_t TR4 : 1; // Rising trigger event configuration of line 4  reset: 0
      uint32_t TR5 : 1; // Rising trigger event configuration of line 5  reset: 0
      uint32_t TR6 : 1; // Rising trigger event configuration of line 6  reset: 0
      uint32_t TR7 : 1; // Rising trigger event configuration of line 7  reset: 0
      uint32_t TR8 : 1; // Rising trigger event configuration of line 8  reset: 0
      uint32_t TR9 : 1; // Rising trigger event configuration of line 9  reset: 0
      uint32_t TR10 : 1; // Rising trigger event configuration of line 10  reset: 0
      uint32_t TR11 : 1; // Rising trigger event configuration of line 11  reset: 0
      uint32_t TR12 : 1; // Rising trigger event configuration of line 12  reset: 0
      uint32_t TR13 : 1; // Rising trigger event configuration of line 13  reset: 0
      uint32_t TR14 : 1; // Rising trigger event configuration of line 14  reset: 0
      uint32_t TR15 : 1; // Rising trigger event configuration of line 15  reset: 0
      uint32_t TR16 : 1; // Rising trigger event configuration of line 16  reset: 0
      uint32_t TR17 : 1; // Rising trigger event configuration of line 17  reset: 0
      uint32_t TR18 : 1; // Rising trigger event configuration of line 18  reset: 0
      uint32_t RESERVED_449 : 13;
    };
  } RTSR;
  union{
    volatile uint32_t w;
    struct { // Falling Trigger selection register (EXTI_FTSR) @ 0x4001040C read-write reset: 0x00000000
      uint32_t TR0 : 1; // Falling trigger event configuration of line 0  reset: 0
      uint32_t TR1 : 1; // Falling trigger event configuration of line 1  reset: 0
      uint32_t TR2 : 1; // Falling trigger event configuration of line 2  reset: 0
      uint32_t TR3 : 1; // Falling trigger event configuration of line 3  reset: 0
      uint32_t TR4 : 1; // Falling trigger event configuration of line 4  reset: 0
      uint32_t TR5 : 1; // Falling trigger event configuration of line 5  reset: 0
      uint32_t TR6 : 1; // Falling trigger event configuration of line 6  reset: 0
      uint32_t TR7 : 1; // Falling trigger event configuration of line 7  reset: 0
      uint32_t TR8 : 1; // Falling trigger event configuration of line 8  reset: 0
      uint32_t TR9 : 1; // Falling trigger event configuration of line 9  reset: 0
      uint32_t TR10 : 1; // Falling trigger event configuration of line 10  reset: 0
      uint32_t TR11 : 1; // Falling trigger event configuration of line 11  reset: 0
      uint32_t TR12 : 1; // Falling trigger event configuration of line 12  reset: 0
      uint32_t TR13 : 1; // Falling trigger event configuration of line 13  reset: 0
      uint32_t TR14 : 1; // Falling trigger event configuration of line 14  reset: 0
      uint32_t TR15 : 1; // Falling trigger event configuration of line 15  reset: 0
      uint32_t TR16 : 1; // Falling trigger event configuration of line 16  reset: 0
      uint32_t TR17 : 1; // Falling trigger event configuration of line 17  reset: 0
      uint32_t TR18 : 1; // Falling trigger event configuration of line 18  reset: 0
      uint32_t RESERVED_450 : 13;
    };
  } FTSR;
  union{
    volatile uint32_t w;
    struct { // Software interrupt event register (EXTI_SWIER) @ 0x40010410 read-write reset: 0x00000000
      uint32_t SWIER0 : 1; // Software Interrupt on line 0  reset: 0
      uint32_t SWIER1 : 1; // Software Interrupt on line 1  reset: 0
      uint32_t SWIER2 : 1; // Software Interrupt on line 2  reset: 0
      uint32_t SWIER3 : 1; // Software Interrupt on line 3  reset: 0
      uint32_t SWIER4 : 1; // Software Interrupt on line 4  reset: 0
      uint32_t SWIER5 : 1; // Software Interrupt on line 5  reset: 0
      uint32_t SWIER6 : 1; // Software Interrupt on line 6  reset: 0
      uint32_t SWIER7 : 1; // Software Interrupt on line 7  reset: 0
      uint32_t SWIER8 : 1; // Software Interrupt on line 8  reset: 0
      uint32_t SWIER9 : 1; // Software Interrupt on line 9  reset: 0
      uint32_t SWIER10 : 1; // Software Interrupt on line 10  reset: 0
      uint32_t SWIER11 : 1; // Software Interrupt on line 11  reset: 0
      uint32_t SWIER12 : 1; // Software Interrupt on line 12  reset: 0
      uint32_t SWIER13 : 1; // Software Interrupt on line 13  reset: 0
      uint32_t SWIER14 : 1; // Software Interrupt on line 14  reset: 0
      uint32_t SWIER15 : 1; // Software Interrupt on line 15  reset: 0
      uint32_t SWIER16 : 1; // Software Interrupt on line 16  reset: 0
      uint32_t SWIER17 : 1; // Software Interrupt on line 17  reset: 0
      uint32_t SWIER18 : 1; // Software Interrupt on line 18  reset: 0
      uint32_t RESERVED_451 : 13;
    };
  } SWIER;
  union{
    volatile uint32_t w;
    struct { // Pending register (EXTI_PR) @ 0x40010414 read-write reset: 0x00000000
      uint32_t PR0 : 1; // Pending bit 0  reset: 0
      uint32_t PR1 : 1; // Pending bit 1  reset: 0
      uint32_t PR2 : 1; // Pending bit 2  reset: 0
      uint32_t PR3 : 1; // Pending bit 3  reset: 0
      uint32_t PR4 : 1; // Pending bit 4  reset: 0
      uint32_t PR5 : 1; // Pending bit 5  reset: 0
      uint32_t PR6 : 1; // Pending bit 6  reset: 0
      uint32_t PR7 : 1; // Pending bit 7  reset: 0
      uint32_t PR8 : 1; // Pending bit 8  reset: 0
      uint32_t PR9 : 1; // Pending bit 9  reset: 0
      uint32_t PR10 : 1; // Pending bit 10  reset: 0
      uint32_t PR11 : 1; // Pending bit 11  reset: 0
      uint32_t PR12 : 1; // Pending bit 12  reset: 0
      uint32_t PR13 : 1; // Pending bit 13  reset: 0
      uint32_t PR14 : 1; // Pending bit 14  reset: 0
      uint32_t PR15 : 1; // Pending bit 15  reset: 0
      uint32_t PR16 : 1; // Pending bit 16  reset: 0
      uint32_t PR17 : 1; // Pending bit 17  reset: 0
      uint32_t PR18 : 1; // Pending bit 18  reset: 0
      uint32_t RESERVED_452 : 13;
    };
  } PR;
} EXTI_t;

// GPIOA register struct @ 0x40010800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Port configuration register low (GPIOn_CRL) @ 0x40010800 read-write reset: 0x44444444
      uint32_t MODE0 : 2; // Port n.0 mode bits  reset: 0
      uint32_t CNF0 : 2; // Port n.0 configuration bits  reset: 1
      uint32_t MODE1 : 2; // Port n.1 mode bits  reset: 0
      uint32_t CNF1 : 2; // Port n.1 configuration bits  reset: 1
      uint32_t MODE2 : 2; // Port n.2 mode bits  reset: 0
      uint32_t CNF2 : 2; // Port n.2 configuration bits  reset: 1
      uint32_t MODE3 : 2; // Port n.3 mode bits  reset: 0
      uint32_t CNF3 : 2; // Port n.3 configuration bits  reset: 1
      uint32_t MODE4 : 2; // Port n.4 mode bits  reset: 0
      uint32_t CNF4 : 2; // Port n.4 configuration bits  reset: 1
      uint32_t MODE5 : 2; // Port n.5 mode bits  reset: 0
      uint32_t CNF5 : 2; // Port n.5 configuration bits  reset: 1
      uint32_t MODE6 : 2; // Port n.6 mode bits  reset: 0
      uint32_t CNF6 : 2; // Port n.6 configuration bits  reset: 1
      uint32_t MODE7 : 2; // Port n.7 mode bits  reset: 0
      uint32_t CNF7 : 2; // Port n.7 configuration bits  reset: 1
    };
  } CRL;
  union{
    volatile uint32_t w;
    struct { // Port configuration register high (GPIOn_CRL) @ 0x40010804 read-write reset: 0x44444444
      uint32_t MODE8 : 2; // Port n.8 mode bits  reset: 0
      uint32_t CNF8 : 2; // Port n.8 configuration bits  reset: 1
      uint32_t MODE9 : 2; // Port n.9 mode bits  reset: 0
      uint32_t CNF9 : 2; // Port n.9 configuration bits  reset: 1
      uint32_t MODE10 : 2; // Port n.10 mode bits  reset: 0
      uint32_t CNF10 : 2; // Port n.10 configuration bits  reset: 1
      uint32_t MODE11 : 2; // Port n.11 mode bits  reset: 0
      uint32_t CNF11 : 2; // Port n.11 configuration bits  reset: 1
      uint32_t MODE12 : 2; // Port n.12 mode bits  reset: 0
      uint32_t CNF12 : 2; // Port n.12 configuration bits  reset: 1
      uint32_t MODE13 : 2; // Port n.13 mode bits  reset: 0
      uint32_t CNF13 : 2; // Port n.13 configuration bits  reset: 1
      uint32_t MODE14 : 2; // Port n.14 mode bits  reset: 0
      uint32_t CNF14 : 2; // Port n.14 configuration bits  reset: 1
      uint32_t MODE15 : 2; // Port n.15 mode bits  reset: 0
      uint32_t CNF15 : 2; // Port n.15 configuration bits  reset: 1
    };
  } CRH;
  union{
    volatile uint32_t w;
    struct { // Port input data register (GPIOn_IDR) @ 0x40010808 read-only reset: 0x00000000
      uint32_t IDR0 : 1; // Port input data  reset: 0
      uint32_t IDR1 : 1; // Port input data  reset: 0
      uint32_t IDR2 : 1; // Port input data  reset: 0
      uint32_t IDR3 : 1; // Port input data  reset: 0
      uint32_t IDR4 : 1; // Port input data  reset: 0
      uint32_t IDR5 : 1; // Port input data  reset: 0
      uint32_t IDR6 : 1; // Port input data  reset: 0
      uint32_t IDR7 : 1; // Port input data  reset: 0
      uint32_t IDR8 : 1; // Port input data  reset: 0
      uint32_t IDR9 : 1; // Port input data  reset: 0
      uint32_t IDR10 : 1; // Port input data  reset: 0
      uint32_t IDR11 : 1; // Port input data  reset: 0
      uint32_t IDR12 : 1; // Port input data  reset: 0
      uint32_t IDR13 : 1; // Port input data  reset: 0
      uint32_t IDR14 : 1; // Port input data  reset: 0
      uint32_t IDR15 : 1; // Port input data  reset: 0
      uint32_t RESERVED_453 : 16;
    };
  } IDR;
  union{
    volatile uint32_t w;
    struct { // Port output data register (GPIOn_ODR) @ 0x4001080C read-write reset: 0x00000000
      uint32_t ODR0 : 1; // Port output data  reset: 0
      uint32_t ODR1 : 1; // Port output data  reset: 0
      uint32_t ODR2 : 1; // Port output data  reset: 0
      uint32_t ODR3 : 1; // Port output data  reset: 0
      uint32_t ODR4 : 1; // Port output data  reset: 0
      uint32_t ODR5 : 1; // Port output data  reset: 0
      uint32_t ODR6 : 1; // Port output data  reset: 0
      uint32_t ODR7 : 1; // Port output data  reset: 0
      uint32_t ODR8 : 1; // Port output data  reset: 0
      uint32_t ODR9 : 1; // Port output data  reset: 0
      uint32_t ODR10 : 1; // Port output data  reset: 0
      uint32_t ODR11 : 1; // Port output data  reset: 0
      uint32_t ODR12 : 1; // Port output data  reset: 0
      uint32_t ODR13 : 1; // Port output data  reset: 0
      uint32_t ODR14 : 1; // Port output data  reset: 0
      uint32_t ODR15 : 1; // Port output data  reset: 0
      uint32_t RESERVED_454 : 16;
    };
  } ODR;
  union{
    volatile uint32_t w;
    struct { // Port bit set/reset register (GPIOn_BSRR) @ 0x40010810 write-only reset: 0x00000000
      uint32_t BS0 : 1; // Set bit 0  reset: 0
      uint32_t BS1 : 1; // Set bit 1  reset: 0
      uint32_t BS2 : 1; // Set bit 1  reset: 0
      uint32_t BS3 : 1; // Set bit 3  reset: 0
      uint32_t BS4 : 1; // Set bit 4  reset: 0
      uint32_t BS5 : 1; // Set bit 5  reset: 0
      uint32_t BS6 : 1; // Set bit 6  reset: 0
      uint32_t BS7 : 1; // Set bit 7  reset: 0
      uint32_t BS8 : 1; // Set bit 8  reset: 0
      uint32_t BS9 : 1; // Set bit 9  reset: 0
      uint32_t BS10 : 1; // Set bit 10  reset: 0
      uint32_t BS11 : 1; // Set bit 11  reset: 0
      uint32_t BS12 : 1; // Set bit 12  reset: 0
      uint32_t BS13 : 1; // Set bit 13  reset: 0
      uint32_t BS14 : 1; // Set bit 14  reset: 0
      uint32_t BS15 : 1; // Set bit 15  reset: 0
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 2  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
    };
  } BSRR;
  union{
    volatile uint32_t w;
    struct { // Port bit reset register (GPIOn_BRR) @ 0x40010814 write-only reset: 0x00000000
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 1  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
      uint32_t RESERVED_455 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Port configuration lock register @ 0x40010818 read-write reset: 0x00000000
      uint32_t LCK0 : 1; // Port A Lock bit 0  reset: 0
      uint32_t LCK1 : 1; // Port A Lock bit 1  reset: 0
      uint32_t LCK2 : 1; // Port A Lock bit 2  reset: 0
      uint32_t LCK3 : 1; // Port A Lock bit 3  reset: 0
      uint32_t LCK4 : 1; // Port A Lock bit 4  reset: 0
      uint32_t LCK5 : 1; // Port A Lock bit 5  reset: 0
      uint32_t LCK6 : 1; // Port A Lock bit 6  reset: 0
      uint32_t LCK7 : 1; // Port A Lock bit 7  reset: 0
      uint32_t LCK8 : 1; // Port A Lock bit 8  reset: 0
      uint32_t LCK9 : 1; // Port A Lock bit 9  reset: 0
      uint32_t LCK10 : 1; // Port A Lock bit 10  reset: 0
      uint32_t LCK11 : 1; // Port A Lock bit 11  reset: 0
      uint32_t LCK12 : 1; // Port A Lock bit 12  reset: 0
      uint32_t LCK13 : 1; // Port A Lock bit 13  reset: 0
      uint32_t LCK14 : 1; // Port A Lock bit 14  reset: 0
      uint32_t LCK15 : 1; // Port A Lock bit 15  reset: 0
      uint32_t LCKK : 1; // Lock key  reset: 0
      uint32_t RESERVED_456 : 15;
    };
  } LCKR;
} GPIOA_t;

// GPIOB register struct @ 0x40010C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Port configuration register low (GPIOn_CRL) @ 0x40010C00 read-write reset: 0x44444444
      uint32_t MODE0 : 2; // Port n.0 mode bits  reset: 0
      uint32_t CNF0 : 2; // Port n.0 configuration bits  reset: 1
      uint32_t MODE1 : 2; // Port n.1 mode bits  reset: 0
      uint32_t CNF1 : 2; // Port n.1 configuration bits  reset: 1
      uint32_t MODE2 : 2; // Port n.2 mode bits  reset: 0
      uint32_t CNF2 : 2; // Port n.2 configuration bits  reset: 1
      uint32_t MODE3 : 2; // Port n.3 mode bits  reset: 0
      uint32_t CNF3 : 2; // Port n.3 configuration bits  reset: 1
      uint32_t MODE4 : 2; // Port n.4 mode bits  reset: 0
      uint32_t CNF4 : 2; // Port n.4 configuration bits  reset: 1
      uint32_t MODE5 : 2; // Port n.5 mode bits  reset: 0
      uint32_t CNF5 : 2; // Port n.5 configuration bits  reset: 1
      uint32_t MODE6 : 2; // Port n.6 mode bits  reset: 0
      uint32_t CNF6 : 2; // Port n.6 configuration bits  reset: 1
      uint32_t MODE7 : 2; // Port n.7 mode bits  reset: 0
      uint32_t CNF7 : 2; // Port n.7 configuration bits  reset: 1
    };
  } CRL;
  union{
    volatile uint32_t w;
    struct { // Port configuration register high (GPIOn_CRL) @ 0x40010C04 read-write reset: 0x44444444
      uint32_t MODE8 : 2; // Port n.8 mode bits  reset: 0
      uint32_t CNF8 : 2; // Port n.8 configuration bits  reset: 1
      uint32_t MODE9 : 2; // Port n.9 mode bits  reset: 0
      uint32_t CNF9 : 2; // Port n.9 configuration bits  reset: 1
      uint32_t MODE10 : 2; // Port n.10 mode bits  reset: 0
      uint32_t CNF10 : 2; // Port n.10 configuration bits  reset: 1
      uint32_t MODE11 : 2; // Port n.11 mode bits  reset: 0
      uint32_t CNF11 : 2; // Port n.11 configuration bits  reset: 1
      uint32_t MODE12 : 2; // Port n.12 mode bits  reset: 0
      uint32_t CNF12 : 2; // Port n.12 configuration bits  reset: 1
      uint32_t MODE13 : 2; // Port n.13 mode bits  reset: 0
      uint32_t CNF13 : 2; // Port n.13 configuration bits  reset: 1
      uint32_t MODE14 : 2; // Port n.14 mode bits  reset: 0
      uint32_t CNF14 : 2; // Port n.14 configuration bits  reset: 1
      uint32_t MODE15 : 2; // Port n.15 mode bits  reset: 0
      uint32_t CNF15 : 2; // Port n.15 configuration bits  reset: 1
    };
  } CRH;
  union{
    volatile uint32_t w;
    struct { // Port input data register (GPIOn_IDR) @ 0x40010C08 read-only reset: 0x00000000
      uint32_t IDR0 : 1; // Port input data  reset: 0
      uint32_t IDR1 : 1; // Port input data  reset: 0
      uint32_t IDR2 : 1; // Port input data  reset: 0
      uint32_t IDR3 : 1; // Port input data  reset: 0
      uint32_t IDR4 : 1; // Port input data  reset: 0
      uint32_t IDR5 : 1; // Port input data  reset: 0
      uint32_t IDR6 : 1; // Port input data  reset: 0
      uint32_t IDR7 : 1; // Port input data  reset: 0
      uint32_t IDR8 : 1; // Port input data  reset: 0
      uint32_t IDR9 : 1; // Port input data  reset: 0
      uint32_t IDR10 : 1; // Port input data  reset: 0
      uint32_t IDR11 : 1; // Port input data  reset: 0
      uint32_t IDR12 : 1; // Port input data  reset: 0
      uint32_t IDR13 : 1; // Port input data  reset: 0
      uint32_t IDR14 : 1; // Port input data  reset: 0
      uint32_t IDR15 : 1; // Port input data  reset: 0
      uint32_t RESERVED_457 : 16;
    };
  } IDR;
  union{
    volatile uint32_t w;
    struct { // Port output data register (GPIOn_ODR) @ 0x40010C0C read-write reset: 0x00000000
      uint32_t ODR0 : 1; // Port output data  reset: 0
      uint32_t ODR1 : 1; // Port output data  reset: 0
      uint32_t ODR2 : 1; // Port output data  reset: 0
      uint32_t ODR3 : 1; // Port output data  reset: 0
      uint32_t ODR4 : 1; // Port output data  reset: 0
      uint32_t ODR5 : 1; // Port output data  reset: 0
      uint32_t ODR6 : 1; // Port output data  reset: 0
      uint32_t ODR7 : 1; // Port output data  reset: 0
      uint32_t ODR8 : 1; // Port output data  reset: 0
      uint32_t ODR9 : 1; // Port output data  reset: 0
      uint32_t ODR10 : 1; // Port output data  reset: 0
      uint32_t ODR11 : 1; // Port output data  reset: 0
      uint32_t ODR12 : 1; // Port output data  reset: 0
      uint32_t ODR13 : 1; // Port output data  reset: 0
      uint32_t ODR14 : 1; // Port output data  reset: 0
      uint32_t ODR15 : 1; // Port output data  reset: 0
      uint32_t RESERVED_458 : 16;
    };
  } ODR;
  union{
    volatile uint32_t w;
    struct { // Port bit set/reset register (GPIOn_BSRR) @ 0x40010C10 write-only reset: 0x00000000
      uint32_t BS0 : 1; // Set bit 0  reset: 0
      uint32_t BS1 : 1; // Set bit 1  reset: 0
      uint32_t BS2 : 1; // Set bit 1  reset: 0
      uint32_t BS3 : 1; // Set bit 3  reset: 0
      uint32_t BS4 : 1; // Set bit 4  reset: 0
      uint32_t BS5 : 1; // Set bit 5  reset: 0
      uint32_t BS6 : 1; // Set bit 6  reset: 0
      uint32_t BS7 : 1; // Set bit 7  reset: 0
      uint32_t BS8 : 1; // Set bit 8  reset: 0
      uint32_t BS9 : 1; // Set bit 9  reset: 0
      uint32_t BS10 : 1; // Set bit 10  reset: 0
      uint32_t BS11 : 1; // Set bit 11  reset: 0
      uint32_t BS12 : 1; // Set bit 12  reset: 0
      uint32_t BS13 : 1; // Set bit 13  reset: 0
      uint32_t BS14 : 1; // Set bit 14  reset: 0
      uint32_t BS15 : 1; // Set bit 15  reset: 0
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 2  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
    };
  } BSRR;
  union{
    volatile uint32_t w;
    struct { // Port bit reset register (GPIOn_BRR) @ 0x40010C14 write-only reset: 0x00000000
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 1  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
      uint32_t RESERVED_459 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Port configuration lock register @ 0x40010C18 read-write reset: 0x00000000
      uint32_t LCK0 : 1; // Port A Lock bit 0  reset: 0
      uint32_t LCK1 : 1; // Port A Lock bit 1  reset: 0
      uint32_t LCK2 : 1; // Port A Lock bit 2  reset: 0
      uint32_t LCK3 : 1; // Port A Lock bit 3  reset: 0
      uint32_t LCK4 : 1; // Port A Lock bit 4  reset: 0
      uint32_t LCK5 : 1; // Port A Lock bit 5  reset: 0
      uint32_t LCK6 : 1; // Port A Lock bit 6  reset: 0
      uint32_t LCK7 : 1; // Port A Lock bit 7  reset: 0
      uint32_t LCK8 : 1; // Port A Lock bit 8  reset: 0
      uint32_t LCK9 : 1; // Port A Lock bit 9  reset: 0
      uint32_t LCK10 : 1; // Port A Lock bit 10  reset: 0
      uint32_t LCK11 : 1; // Port A Lock bit 11  reset: 0
      uint32_t LCK12 : 1; // Port A Lock bit 12  reset: 0
      uint32_t LCK13 : 1; // Port A Lock bit 13  reset: 0
      uint32_t LCK14 : 1; // Port A Lock bit 14  reset: 0
      uint32_t LCK15 : 1; // Port A Lock bit 15  reset: 0
      uint32_t LCKK : 1; // Lock key  reset: 0
      uint32_t RESERVED_460 : 15;
    };
  } LCKR;
} GPIOB_t;

// GPIOC register struct @ 0x40011000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Port configuration register low (GPIOn_CRL) @ 0x40011000 read-write reset: 0x44444444
      uint32_t MODE0 : 2; // Port n.0 mode bits  reset: 0
      uint32_t CNF0 : 2; // Port n.0 configuration bits  reset: 1
      uint32_t MODE1 : 2; // Port n.1 mode bits  reset: 0
      uint32_t CNF1 : 2; // Port n.1 configuration bits  reset: 1
      uint32_t MODE2 : 2; // Port n.2 mode bits  reset: 0
      uint32_t CNF2 : 2; // Port n.2 configuration bits  reset: 1
      uint32_t MODE3 : 2; // Port n.3 mode bits  reset: 0
      uint32_t CNF3 : 2; // Port n.3 configuration bits  reset: 1
      uint32_t MODE4 : 2; // Port n.4 mode bits  reset: 0
      uint32_t CNF4 : 2; // Port n.4 configuration bits  reset: 1
      uint32_t MODE5 : 2; // Port n.5 mode bits  reset: 0
      uint32_t CNF5 : 2; // Port n.5 configuration bits  reset: 1
      uint32_t MODE6 : 2; // Port n.6 mode bits  reset: 0
      uint32_t CNF6 : 2; // Port n.6 configuration bits  reset: 1
      uint32_t MODE7 : 2; // Port n.7 mode bits  reset: 0
      uint32_t CNF7 : 2; // Port n.7 configuration bits  reset: 1
    };
  } CRL;
  union{
    volatile uint32_t w;
    struct { // Port configuration register high (GPIOn_CRL) @ 0x40011004 read-write reset: 0x44444444
      uint32_t MODE8 : 2; // Port n.8 mode bits  reset: 0
      uint32_t CNF8 : 2; // Port n.8 configuration bits  reset: 1
      uint32_t MODE9 : 2; // Port n.9 mode bits  reset: 0
      uint32_t CNF9 : 2; // Port n.9 configuration bits  reset: 1
      uint32_t MODE10 : 2; // Port n.10 mode bits  reset: 0
      uint32_t CNF10 : 2; // Port n.10 configuration bits  reset: 1
      uint32_t MODE11 : 2; // Port n.11 mode bits  reset: 0
      uint32_t CNF11 : 2; // Port n.11 configuration bits  reset: 1
      uint32_t MODE12 : 2; // Port n.12 mode bits  reset: 0
      uint32_t CNF12 : 2; // Port n.12 configuration bits  reset: 1
      uint32_t MODE13 : 2; // Port n.13 mode bits  reset: 0
      uint32_t CNF13 : 2; // Port n.13 configuration bits  reset: 1
      uint32_t MODE14 : 2; // Port n.14 mode bits  reset: 0
      uint32_t CNF14 : 2; // Port n.14 configuration bits  reset: 1
      uint32_t MODE15 : 2; // Port n.15 mode bits  reset: 0
      uint32_t CNF15 : 2; // Port n.15 configuration bits  reset: 1
    };
  } CRH;
  union{
    volatile uint32_t w;
    struct { // Port input data register (GPIOn_IDR) @ 0x40011008 read-only reset: 0x00000000
      uint32_t IDR0 : 1; // Port input data  reset: 0
      uint32_t IDR1 : 1; // Port input data  reset: 0
      uint32_t IDR2 : 1; // Port input data  reset: 0
      uint32_t IDR3 : 1; // Port input data  reset: 0
      uint32_t IDR4 : 1; // Port input data  reset: 0
      uint32_t IDR5 : 1; // Port input data  reset: 0
      uint32_t IDR6 : 1; // Port input data  reset: 0
      uint32_t IDR7 : 1; // Port input data  reset: 0
      uint32_t IDR8 : 1; // Port input data  reset: 0
      uint32_t IDR9 : 1; // Port input data  reset: 0
      uint32_t IDR10 : 1; // Port input data  reset: 0
      uint32_t IDR11 : 1; // Port input data  reset: 0
      uint32_t IDR12 : 1; // Port input data  reset: 0
      uint32_t IDR13 : 1; // Port input data  reset: 0
      uint32_t IDR14 : 1; // Port input data  reset: 0
      uint32_t IDR15 : 1; // Port input data  reset: 0
      uint32_t RESERVED_461 : 16;
    };
  } IDR;
  union{
    volatile uint32_t w;
    struct { // Port output data register (GPIOn_ODR) @ 0x4001100C read-write reset: 0x00000000
      uint32_t ODR0 : 1; // Port output data  reset: 0
      uint32_t ODR1 : 1; // Port output data  reset: 0
      uint32_t ODR2 : 1; // Port output data  reset: 0
      uint32_t ODR3 : 1; // Port output data  reset: 0
      uint32_t ODR4 : 1; // Port output data  reset: 0
      uint32_t ODR5 : 1; // Port output data  reset: 0
      uint32_t ODR6 : 1; // Port output data  reset: 0
      uint32_t ODR7 : 1; // Port output data  reset: 0
      uint32_t ODR8 : 1; // Port output data  reset: 0
      uint32_t ODR9 : 1; // Port output data  reset: 0
      uint32_t ODR10 : 1; // Port output data  reset: 0
      uint32_t ODR11 : 1; // Port output data  reset: 0
      uint32_t ODR12 : 1; // Port output data  reset: 0
      uint32_t ODR13 : 1; // Port output data  reset: 0
      uint32_t ODR14 : 1; // Port output data  reset: 0
      uint32_t ODR15 : 1; // Port output data  reset: 0
      uint32_t RESERVED_462 : 16;
    };
  } ODR;
  union{
    volatile uint32_t w;
    struct { // Port bit set/reset register (GPIOn_BSRR) @ 0x40011010 write-only reset: 0x00000000
      uint32_t BS0 : 1; // Set bit 0  reset: 0
      uint32_t BS1 : 1; // Set bit 1  reset: 0
      uint32_t BS2 : 1; // Set bit 1  reset: 0
      uint32_t BS3 : 1; // Set bit 3  reset: 0
      uint32_t BS4 : 1; // Set bit 4  reset: 0
      uint32_t BS5 : 1; // Set bit 5  reset: 0
      uint32_t BS6 : 1; // Set bit 6  reset: 0
      uint32_t BS7 : 1; // Set bit 7  reset: 0
      uint32_t BS8 : 1; // Set bit 8  reset: 0
      uint32_t BS9 : 1; // Set bit 9  reset: 0
      uint32_t BS10 : 1; // Set bit 10  reset: 0
      uint32_t BS11 : 1; // Set bit 11  reset: 0
      uint32_t BS12 : 1; // Set bit 12  reset: 0
      uint32_t BS13 : 1; // Set bit 13  reset: 0
      uint32_t BS14 : 1; // Set bit 14  reset: 0
      uint32_t BS15 : 1; // Set bit 15  reset: 0
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 2  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
    };
  } BSRR;
  union{
    volatile uint32_t w;
    struct { // Port bit reset register (GPIOn_BRR) @ 0x40011014 write-only reset: 0x00000000
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 1  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
      uint32_t RESERVED_463 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Port configuration lock register @ 0x40011018 read-write reset: 0x00000000
      uint32_t LCK0 : 1; // Port A Lock bit 0  reset: 0
      uint32_t LCK1 : 1; // Port A Lock bit 1  reset: 0
      uint32_t LCK2 : 1; // Port A Lock bit 2  reset: 0
      uint32_t LCK3 : 1; // Port A Lock bit 3  reset: 0
      uint32_t LCK4 : 1; // Port A Lock bit 4  reset: 0
      uint32_t LCK5 : 1; // Port A Lock bit 5  reset: 0
      uint32_t LCK6 : 1; // Port A Lock bit 6  reset: 0
      uint32_t LCK7 : 1; // Port A Lock bit 7  reset: 0
      uint32_t LCK8 : 1; // Port A Lock bit 8  reset: 0
      uint32_t LCK9 : 1; // Port A Lock bit 9  reset: 0
      uint32_t LCK10 : 1; // Port A Lock bit 10  reset: 0
      uint32_t LCK11 : 1; // Port A Lock bit 11  reset: 0
      uint32_t LCK12 : 1; // Port A Lock bit 12  reset: 0
      uint32_t LCK13 : 1; // Port A Lock bit 13  reset: 0
      uint32_t LCK14 : 1; // Port A Lock bit 14  reset: 0
      uint32_t LCK15 : 1; // Port A Lock bit 15  reset: 0
      uint32_t LCKK : 1; // Lock key  reset: 0
      uint32_t RESERVED_464 : 15;
    };
  } LCKR;
} GPIOC_t;

// GPIOD register struct @ 0x40011400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Port configuration register low (GPIOn_CRL) @ 0x40011400 read-write reset: 0x44444444
      uint32_t MODE0 : 2; // Port n.0 mode bits  reset: 0
      uint32_t CNF0 : 2; // Port n.0 configuration bits  reset: 1
      uint32_t MODE1 : 2; // Port n.1 mode bits  reset: 0
      uint32_t CNF1 : 2; // Port n.1 configuration bits  reset: 1
      uint32_t MODE2 : 2; // Port n.2 mode bits  reset: 0
      uint32_t CNF2 : 2; // Port n.2 configuration bits  reset: 1
      uint32_t MODE3 : 2; // Port n.3 mode bits  reset: 0
      uint32_t CNF3 : 2; // Port n.3 configuration bits  reset: 1
      uint32_t MODE4 : 2; // Port n.4 mode bits  reset: 0
      uint32_t CNF4 : 2; // Port n.4 configuration bits  reset: 1
      uint32_t MODE5 : 2; // Port n.5 mode bits  reset: 0
      uint32_t CNF5 : 2; // Port n.5 configuration bits  reset: 1
      uint32_t MODE6 : 2; // Port n.6 mode bits  reset: 0
      uint32_t CNF6 : 2; // Port n.6 configuration bits  reset: 1
      uint32_t MODE7 : 2; // Port n.7 mode bits  reset: 0
      uint32_t CNF7 : 2; // Port n.7 configuration bits  reset: 1
    };
  } CRL;
  union{
    volatile uint32_t w;
    struct { // Port configuration register high (GPIOn_CRL) @ 0x40011404 read-write reset: 0x44444444
      uint32_t MODE8 : 2; // Port n.8 mode bits  reset: 0
      uint32_t CNF8 : 2; // Port n.8 configuration bits  reset: 1
      uint32_t MODE9 : 2; // Port n.9 mode bits  reset: 0
      uint32_t CNF9 : 2; // Port n.9 configuration bits  reset: 1
      uint32_t MODE10 : 2; // Port n.10 mode bits  reset: 0
      uint32_t CNF10 : 2; // Port n.10 configuration bits  reset: 1
      uint32_t MODE11 : 2; // Port n.11 mode bits  reset: 0
      uint32_t CNF11 : 2; // Port n.11 configuration bits  reset: 1
      uint32_t MODE12 : 2; // Port n.12 mode bits  reset: 0
      uint32_t CNF12 : 2; // Port n.12 configuration bits  reset: 1
      uint32_t MODE13 : 2; // Port n.13 mode bits  reset: 0
      uint32_t CNF13 : 2; // Port n.13 configuration bits  reset: 1
      uint32_t MODE14 : 2; // Port n.14 mode bits  reset: 0
      uint32_t CNF14 : 2; // Port n.14 configuration bits  reset: 1
      uint32_t MODE15 : 2; // Port n.15 mode bits  reset: 0
      uint32_t CNF15 : 2; // Port n.15 configuration bits  reset: 1
    };
  } CRH;
  union{
    volatile uint32_t w;
    struct { // Port input data register (GPIOn_IDR) @ 0x40011408 read-only reset: 0x00000000
      uint32_t IDR0 : 1; // Port input data  reset: 0
      uint32_t IDR1 : 1; // Port input data  reset: 0
      uint32_t IDR2 : 1; // Port input data  reset: 0
      uint32_t IDR3 : 1; // Port input data  reset: 0
      uint32_t IDR4 : 1; // Port input data  reset: 0
      uint32_t IDR5 : 1; // Port input data  reset: 0
      uint32_t IDR6 : 1; // Port input data  reset: 0
      uint32_t IDR7 : 1; // Port input data  reset: 0
      uint32_t IDR8 : 1; // Port input data  reset: 0
      uint32_t IDR9 : 1; // Port input data  reset: 0
      uint32_t IDR10 : 1; // Port input data  reset: 0
      uint32_t IDR11 : 1; // Port input data  reset: 0
      uint32_t IDR12 : 1; // Port input data  reset: 0
      uint32_t IDR13 : 1; // Port input data  reset: 0
      uint32_t IDR14 : 1; // Port input data  reset: 0
      uint32_t IDR15 : 1; // Port input data  reset: 0
      uint32_t RESERVED_465 : 16;
    };
  } IDR;
  union{
    volatile uint32_t w;
    struct { // Port output data register (GPIOn_ODR) @ 0x4001140C read-write reset: 0x00000000
      uint32_t ODR0 : 1; // Port output data  reset: 0
      uint32_t ODR1 : 1; // Port output data  reset: 0
      uint32_t ODR2 : 1; // Port output data  reset: 0
      uint32_t ODR3 : 1; // Port output data  reset: 0
      uint32_t ODR4 : 1; // Port output data  reset: 0
      uint32_t ODR5 : 1; // Port output data  reset: 0
      uint32_t ODR6 : 1; // Port output data  reset: 0
      uint32_t ODR7 : 1; // Port output data  reset: 0
      uint32_t ODR8 : 1; // Port output data  reset: 0
      uint32_t ODR9 : 1; // Port output data  reset: 0
      uint32_t ODR10 : 1; // Port output data  reset: 0
      uint32_t ODR11 : 1; // Port output data  reset: 0
      uint32_t ODR12 : 1; // Port output data  reset: 0
      uint32_t ODR13 : 1; // Port output data  reset: 0
      uint32_t ODR14 : 1; // Port output data  reset: 0
      uint32_t ODR15 : 1; // Port output data  reset: 0
      uint32_t RESERVED_466 : 16;
    };
  } ODR;
  union{
    volatile uint32_t w;
    struct { // Port bit set/reset register (GPIOn_BSRR) @ 0x40011410 write-only reset: 0x00000000
      uint32_t BS0 : 1; // Set bit 0  reset: 0
      uint32_t BS1 : 1; // Set bit 1  reset: 0
      uint32_t BS2 : 1; // Set bit 1  reset: 0
      uint32_t BS3 : 1; // Set bit 3  reset: 0
      uint32_t BS4 : 1; // Set bit 4  reset: 0
      uint32_t BS5 : 1; // Set bit 5  reset: 0
      uint32_t BS6 : 1; // Set bit 6  reset: 0
      uint32_t BS7 : 1; // Set bit 7  reset: 0
      uint32_t BS8 : 1; // Set bit 8  reset: 0
      uint32_t BS9 : 1; // Set bit 9  reset: 0
      uint32_t BS10 : 1; // Set bit 10  reset: 0
      uint32_t BS11 : 1; // Set bit 11  reset: 0
      uint32_t BS12 : 1; // Set bit 12  reset: 0
      uint32_t BS13 : 1; // Set bit 13  reset: 0
      uint32_t BS14 : 1; // Set bit 14  reset: 0
      uint32_t BS15 : 1; // Set bit 15  reset: 0
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 2  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
    };
  } BSRR;
  union{
    volatile uint32_t w;
    struct { // Port bit reset register (GPIOn_BRR) @ 0x40011414 write-only reset: 0x00000000
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 1  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
      uint32_t RESERVED_467 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Port configuration lock register @ 0x40011418 read-write reset: 0x00000000
      uint32_t LCK0 : 1; // Port A Lock bit 0  reset: 0
      uint32_t LCK1 : 1; // Port A Lock bit 1  reset: 0
      uint32_t LCK2 : 1; // Port A Lock bit 2  reset: 0
      uint32_t LCK3 : 1; // Port A Lock bit 3  reset: 0
      uint32_t LCK4 : 1; // Port A Lock bit 4  reset: 0
      uint32_t LCK5 : 1; // Port A Lock bit 5  reset: 0
      uint32_t LCK6 : 1; // Port A Lock bit 6  reset: 0
      uint32_t LCK7 : 1; // Port A Lock bit 7  reset: 0
      uint32_t LCK8 : 1; // Port A Lock bit 8  reset: 0
      uint32_t LCK9 : 1; // Port A Lock bit 9  reset: 0
      uint32_t LCK10 : 1; // Port A Lock bit 10  reset: 0
      uint32_t LCK11 : 1; // Port A Lock bit 11  reset: 0
      uint32_t LCK12 : 1; // Port A Lock bit 12  reset: 0
      uint32_t LCK13 : 1; // Port A Lock bit 13  reset: 0
      uint32_t LCK14 : 1; // Port A Lock bit 14  reset: 0
      uint32_t LCK15 : 1; // Port A Lock bit 15  reset: 0
      uint32_t LCKK : 1; // Lock key  reset: 0
      uint32_t RESERVED_468 : 15;
    };
  } LCKR;
} GPIOD_t;

// GPIOE register struct @ 0x40011800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Port configuration register low (GPIOn_CRL) @ 0x40011800 read-write reset: 0x44444444
      uint32_t MODE0 : 2; // Port n.0 mode bits  reset: 0
      uint32_t CNF0 : 2; // Port n.0 configuration bits  reset: 1
      uint32_t MODE1 : 2; // Port n.1 mode bits  reset: 0
      uint32_t CNF1 : 2; // Port n.1 configuration bits  reset: 1
      uint32_t MODE2 : 2; // Port n.2 mode bits  reset: 0
      uint32_t CNF2 : 2; // Port n.2 configuration bits  reset: 1
      uint32_t MODE3 : 2; // Port n.3 mode bits  reset: 0
      uint32_t CNF3 : 2; // Port n.3 configuration bits  reset: 1
      uint32_t MODE4 : 2; // Port n.4 mode bits  reset: 0
      uint32_t CNF4 : 2; // Port n.4 configuration bits  reset: 1
      uint32_t MODE5 : 2; // Port n.5 mode bits  reset: 0
      uint32_t CNF5 : 2; // Port n.5 configuration bits  reset: 1
      uint32_t MODE6 : 2; // Port n.6 mode bits  reset: 0
      uint32_t CNF6 : 2; // Port n.6 configuration bits  reset: 1
      uint32_t MODE7 : 2; // Port n.7 mode bits  reset: 0
      uint32_t CNF7 : 2; // Port n.7 configuration bits  reset: 1
    };
  } CRL;
  union{
    volatile uint32_t w;
    struct { // Port configuration register high (GPIOn_CRL) @ 0x40011804 read-write reset: 0x44444444
      uint32_t MODE8 : 2; // Port n.8 mode bits  reset: 0
      uint32_t CNF8 : 2; // Port n.8 configuration bits  reset: 1
      uint32_t MODE9 : 2; // Port n.9 mode bits  reset: 0
      uint32_t CNF9 : 2; // Port n.9 configuration bits  reset: 1
      uint32_t MODE10 : 2; // Port n.10 mode bits  reset: 0
      uint32_t CNF10 : 2; // Port n.10 configuration bits  reset: 1
      uint32_t MODE11 : 2; // Port n.11 mode bits  reset: 0
      uint32_t CNF11 : 2; // Port n.11 configuration bits  reset: 1
      uint32_t MODE12 : 2; // Port n.12 mode bits  reset: 0
      uint32_t CNF12 : 2; // Port n.12 configuration bits  reset: 1
      uint32_t MODE13 : 2; // Port n.13 mode bits  reset: 0
      uint32_t CNF13 : 2; // Port n.13 configuration bits  reset: 1
      uint32_t MODE14 : 2; // Port n.14 mode bits  reset: 0
      uint32_t CNF14 : 2; // Port n.14 configuration bits  reset: 1
      uint32_t MODE15 : 2; // Port n.15 mode bits  reset: 0
      uint32_t CNF15 : 2; // Port n.15 configuration bits  reset: 1
    };
  } CRH;
  union{
    volatile uint32_t w;
    struct { // Port input data register (GPIOn_IDR) @ 0x40011808 read-only reset: 0x00000000
      uint32_t IDR0 : 1; // Port input data  reset: 0
      uint32_t IDR1 : 1; // Port input data  reset: 0
      uint32_t IDR2 : 1; // Port input data  reset: 0
      uint32_t IDR3 : 1; // Port input data  reset: 0
      uint32_t IDR4 : 1; // Port input data  reset: 0
      uint32_t IDR5 : 1; // Port input data  reset: 0
      uint32_t IDR6 : 1; // Port input data  reset: 0
      uint32_t IDR7 : 1; // Port input data  reset: 0
      uint32_t IDR8 : 1; // Port input data  reset: 0
      uint32_t IDR9 : 1; // Port input data  reset: 0
      uint32_t IDR10 : 1; // Port input data  reset: 0
      uint32_t IDR11 : 1; // Port input data  reset: 0
      uint32_t IDR12 : 1; // Port input data  reset: 0
      uint32_t IDR13 : 1; // Port input data  reset: 0
      uint32_t IDR14 : 1; // Port input data  reset: 0
      uint32_t IDR15 : 1; // Port input data  reset: 0
      uint32_t RESERVED_469 : 16;
    };
  } IDR;
  union{
    volatile uint32_t w;
    struct { // Port output data register (GPIOn_ODR) @ 0x4001180C read-write reset: 0x00000000
      uint32_t ODR0 : 1; // Port output data  reset: 0
      uint32_t ODR1 : 1; // Port output data  reset: 0
      uint32_t ODR2 : 1; // Port output data  reset: 0
      uint32_t ODR3 : 1; // Port output data  reset: 0
      uint32_t ODR4 : 1; // Port output data  reset: 0
      uint32_t ODR5 : 1; // Port output data  reset: 0
      uint32_t ODR6 : 1; // Port output data  reset: 0
      uint32_t ODR7 : 1; // Port output data  reset: 0
      uint32_t ODR8 : 1; // Port output data  reset: 0
      uint32_t ODR9 : 1; // Port output data  reset: 0
      uint32_t ODR10 : 1; // Port output data  reset: 0
      uint32_t ODR11 : 1; // Port output data  reset: 0
      uint32_t ODR12 : 1; // Port output data  reset: 0
      uint32_t ODR13 : 1; // Port output data  reset: 0
      uint32_t ODR14 : 1; // Port output data  reset: 0
      uint32_t ODR15 : 1; // Port output data  reset: 0
      uint32_t RESERVED_470 : 16;
    };
  } ODR;
  union{
    volatile uint32_t w;
    struct { // Port bit set/reset register (GPIOn_BSRR) @ 0x40011810 write-only reset: 0x00000000
      uint32_t BS0 : 1; // Set bit 0  reset: 0
      uint32_t BS1 : 1; // Set bit 1  reset: 0
      uint32_t BS2 : 1; // Set bit 1  reset: 0
      uint32_t BS3 : 1; // Set bit 3  reset: 0
      uint32_t BS4 : 1; // Set bit 4  reset: 0
      uint32_t BS5 : 1; // Set bit 5  reset: 0
      uint32_t BS6 : 1; // Set bit 6  reset: 0
      uint32_t BS7 : 1; // Set bit 7  reset: 0
      uint32_t BS8 : 1; // Set bit 8  reset: 0
      uint32_t BS9 : 1; // Set bit 9  reset: 0
      uint32_t BS10 : 1; // Set bit 10  reset: 0
      uint32_t BS11 : 1; // Set bit 11  reset: 0
      uint32_t BS12 : 1; // Set bit 12  reset: 0
      uint32_t BS13 : 1; // Set bit 13  reset: 0
      uint32_t BS14 : 1; // Set bit 14  reset: 0
      uint32_t BS15 : 1; // Set bit 15  reset: 0
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 2  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
    };
  } BSRR;
  union{
    volatile uint32_t w;
    struct { // Port bit reset register (GPIOn_BRR) @ 0x40011814 write-only reset: 0x00000000
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 1  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
      uint32_t RESERVED_471 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Port configuration lock register @ 0x40011818 read-write reset: 0x00000000
      uint32_t LCK0 : 1; // Port A Lock bit 0  reset: 0
      uint32_t LCK1 : 1; // Port A Lock bit 1  reset: 0
      uint32_t LCK2 : 1; // Port A Lock bit 2  reset: 0
      uint32_t LCK3 : 1; // Port A Lock bit 3  reset: 0
      uint32_t LCK4 : 1; // Port A Lock bit 4  reset: 0
      uint32_t LCK5 : 1; // Port A Lock bit 5  reset: 0
      uint32_t LCK6 : 1; // Port A Lock bit 6  reset: 0
      uint32_t LCK7 : 1; // Port A Lock bit 7  reset: 0
      uint32_t LCK8 : 1; // Port A Lock bit 8  reset: 0
      uint32_t LCK9 : 1; // Port A Lock bit 9  reset: 0
      uint32_t LCK10 : 1; // Port A Lock bit 10  reset: 0
      uint32_t LCK11 : 1; // Port A Lock bit 11  reset: 0
      uint32_t LCK12 : 1; // Port A Lock bit 12  reset: 0
      uint32_t LCK13 : 1; // Port A Lock bit 13  reset: 0
      uint32_t LCK14 : 1; // Port A Lock bit 14  reset: 0
      uint32_t LCK15 : 1; // Port A Lock bit 15  reset: 0
      uint32_t LCKK : 1; // Lock key  reset: 0
      uint32_t RESERVED_472 : 15;
    };
  } LCKR;
} GPIOE_t;

// GPIOF register struct @ 0x40011C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Port configuration register low (GPIOn_CRL) @ 0x40011C00 read-write reset: 0x44444444
      uint32_t MODE0 : 2; // Port n.0 mode bits  reset: 0
      uint32_t CNF0 : 2; // Port n.0 configuration bits  reset: 1
      uint32_t MODE1 : 2; // Port n.1 mode bits  reset: 0
      uint32_t CNF1 : 2; // Port n.1 configuration bits  reset: 1
      uint32_t MODE2 : 2; // Port n.2 mode bits  reset: 0
      uint32_t CNF2 : 2; // Port n.2 configuration bits  reset: 1
      uint32_t MODE3 : 2; // Port n.3 mode bits  reset: 0
      uint32_t CNF3 : 2; // Port n.3 configuration bits  reset: 1
      uint32_t MODE4 : 2; // Port n.4 mode bits  reset: 0
      uint32_t CNF4 : 2; // Port n.4 configuration bits  reset: 1
      uint32_t MODE5 : 2; // Port n.5 mode bits  reset: 0
      uint32_t CNF5 : 2; // Port n.5 configuration bits  reset: 1
      uint32_t MODE6 : 2; // Port n.6 mode bits  reset: 0
      uint32_t CNF6 : 2; // Port n.6 configuration bits  reset: 1
      uint32_t MODE7 : 2; // Port n.7 mode bits  reset: 0
      uint32_t CNF7 : 2; // Port n.7 configuration bits  reset: 1
    };
  } CRL;
  union{
    volatile uint32_t w;
    struct { // Port configuration register high (GPIOn_CRL) @ 0x40011C04 read-write reset: 0x44444444
      uint32_t MODE8 : 2; // Port n.8 mode bits  reset: 0
      uint32_t CNF8 : 2; // Port n.8 configuration bits  reset: 1
      uint32_t MODE9 : 2; // Port n.9 mode bits  reset: 0
      uint32_t CNF9 : 2; // Port n.9 configuration bits  reset: 1
      uint32_t MODE10 : 2; // Port n.10 mode bits  reset: 0
      uint32_t CNF10 : 2; // Port n.10 configuration bits  reset: 1
      uint32_t MODE11 : 2; // Port n.11 mode bits  reset: 0
      uint32_t CNF11 : 2; // Port n.11 configuration bits  reset: 1
      uint32_t MODE12 : 2; // Port n.12 mode bits  reset: 0
      uint32_t CNF12 : 2; // Port n.12 configuration bits  reset: 1
      uint32_t MODE13 : 2; // Port n.13 mode bits  reset: 0
      uint32_t CNF13 : 2; // Port n.13 configuration bits  reset: 1
      uint32_t MODE14 : 2; // Port n.14 mode bits  reset: 0
      uint32_t CNF14 : 2; // Port n.14 configuration bits  reset: 1
      uint32_t MODE15 : 2; // Port n.15 mode bits  reset: 0
      uint32_t CNF15 : 2; // Port n.15 configuration bits  reset: 1
    };
  } CRH;
  union{
    volatile uint32_t w;
    struct { // Port input data register (GPIOn_IDR) @ 0x40011C08 read-only reset: 0x00000000
      uint32_t IDR0 : 1; // Port input data  reset: 0
      uint32_t IDR1 : 1; // Port input data  reset: 0
      uint32_t IDR2 : 1; // Port input data  reset: 0
      uint32_t IDR3 : 1; // Port input data  reset: 0
      uint32_t IDR4 : 1; // Port input data  reset: 0
      uint32_t IDR5 : 1; // Port input data  reset: 0
      uint32_t IDR6 : 1; // Port input data  reset: 0
      uint32_t IDR7 : 1; // Port input data  reset: 0
      uint32_t IDR8 : 1; // Port input data  reset: 0
      uint32_t IDR9 : 1; // Port input data  reset: 0
      uint32_t IDR10 : 1; // Port input data  reset: 0
      uint32_t IDR11 : 1; // Port input data  reset: 0
      uint32_t IDR12 : 1; // Port input data  reset: 0
      uint32_t IDR13 : 1; // Port input data  reset: 0
      uint32_t IDR14 : 1; // Port input data  reset: 0
      uint32_t IDR15 : 1; // Port input data  reset: 0
      uint32_t RESERVED_473 : 16;
    };
  } IDR;
  union{
    volatile uint32_t w;
    struct { // Port output data register (GPIOn_ODR) @ 0x40011C0C read-write reset: 0x00000000
      uint32_t ODR0 : 1; // Port output data  reset: 0
      uint32_t ODR1 : 1; // Port output data  reset: 0
      uint32_t ODR2 : 1; // Port output data  reset: 0
      uint32_t ODR3 : 1; // Port output data  reset: 0
      uint32_t ODR4 : 1; // Port output data  reset: 0
      uint32_t ODR5 : 1; // Port output data  reset: 0
      uint32_t ODR6 : 1; // Port output data  reset: 0
      uint32_t ODR7 : 1; // Port output data  reset: 0
      uint32_t ODR8 : 1; // Port output data  reset: 0
      uint32_t ODR9 : 1; // Port output data  reset: 0
      uint32_t ODR10 : 1; // Port output data  reset: 0
      uint32_t ODR11 : 1; // Port output data  reset: 0
      uint32_t ODR12 : 1; // Port output data  reset: 0
      uint32_t ODR13 : 1; // Port output data  reset: 0
      uint32_t ODR14 : 1; // Port output data  reset: 0
      uint32_t ODR15 : 1; // Port output data  reset: 0
      uint32_t RESERVED_474 : 16;
    };
  } ODR;
  union{
    volatile uint32_t w;
    struct { // Port bit set/reset register (GPIOn_BSRR) @ 0x40011C10 write-only reset: 0x00000000
      uint32_t BS0 : 1; // Set bit 0  reset: 0
      uint32_t BS1 : 1; // Set bit 1  reset: 0
      uint32_t BS2 : 1; // Set bit 1  reset: 0
      uint32_t BS3 : 1; // Set bit 3  reset: 0
      uint32_t BS4 : 1; // Set bit 4  reset: 0
      uint32_t BS5 : 1; // Set bit 5  reset: 0
      uint32_t BS6 : 1; // Set bit 6  reset: 0
      uint32_t BS7 : 1; // Set bit 7  reset: 0
      uint32_t BS8 : 1; // Set bit 8  reset: 0
      uint32_t BS9 : 1; // Set bit 9  reset: 0
      uint32_t BS10 : 1; // Set bit 10  reset: 0
      uint32_t BS11 : 1; // Set bit 11  reset: 0
      uint32_t BS12 : 1; // Set bit 12  reset: 0
      uint32_t BS13 : 1; // Set bit 13  reset: 0
      uint32_t BS14 : 1; // Set bit 14  reset: 0
      uint32_t BS15 : 1; // Set bit 15  reset: 0
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 2  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
    };
  } BSRR;
  union{
    volatile uint32_t w;
    struct { // Port bit reset register (GPIOn_BRR) @ 0x40011C14 write-only reset: 0x00000000
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 1  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
      uint32_t RESERVED_475 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Port configuration lock register @ 0x40011C18 read-write reset: 0x00000000
      uint32_t LCK0 : 1; // Port A Lock bit 0  reset: 0
      uint32_t LCK1 : 1; // Port A Lock bit 1  reset: 0
      uint32_t LCK2 : 1; // Port A Lock bit 2  reset: 0
      uint32_t LCK3 : 1; // Port A Lock bit 3  reset: 0
      uint32_t LCK4 : 1; // Port A Lock bit 4  reset: 0
      uint32_t LCK5 : 1; // Port A Lock bit 5  reset: 0
      uint32_t LCK6 : 1; // Port A Lock bit 6  reset: 0
      uint32_t LCK7 : 1; // Port A Lock bit 7  reset: 0
      uint32_t LCK8 : 1; // Port A Lock bit 8  reset: 0
      uint32_t LCK9 : 1; // Port A Lock bit 9  reset: 0
      uint32_t LCK10 : 1; // Port A Lock bit 10  reset: 0
      uint32_t LCK11 : 1; // Port A Lock bit 11  reset: 0
      uint32_t LCK12 : 1; // Port A Lock bit 12  reset: 0
      uint32_t LCK13 : 1; // Port A Lock bit 13  reset: 0
      uint32_t LCK14 : 1; // Port A Lock bit 14  reset: 0
      uint32_t LCK15 : 1; // Port A Lock bit 15  reset: 0
      uint32_t LCKK : 1; // Lock key  reset: 0
      uint32_t RESERVED_476 : 15;
    };
  } LCKR;
} GPIOF_t;

// GPIOG register struct @ 0x40012000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Port configuration register low (GPIOn_CRL) @ 0x40012000 read-write reset: 0x44444444
      uint32_t MODE0 : 2; // Port n.0 mode bits  reset: 0
      uint32_t CNF0 : 2; // Port n.0 configuration bits  reset: 1
      uint32_t MODE1 : 2; // Port n.1 mode bits  reset: 0
      uint32_t CNF1 : 2; // Port n.1 configuration bits  reset: 1
      uint32_t MODE2 : 2; // Port n.2 mode bits  reset: 0
      uint32_t CNF2 : 2; // Port n.2 configuration bits  reset: 1
      uint32_t MODE3 : 2; // Port n.3 mode bits  reset: 0
      uint32_t CNF3 : 2; // Port n.3 configuration bits  reset: 1
      uint32_t MODE4 : 2; // Port n.4 mode bits  reset: 0
      uint32_t CNF4 : 2; // Port n.4 configuration bits  reset: 1
      uint32_t MODE5 : 2; // Port n.5 mode bits  reset: 0
      uint32_t CNF5 : 2; // Port n.5 configuration bits  reset: 1
      uint32_t MODE6 : 2; // Port n.6 mode bits  reset: 0
      uint32_t CNF6 : 2; // Port n.6 configuration bits  reset: 1
      uint32_t MODE7 : 2; // Port n.7 mode bits  reset: 0
      uint32_t CNF7 : 2; // Port n.7 configuration bits  reset: 1
    };
  } CRL;
  union{
    volatile uint32_t w;
    struct { // Port configuration register high (GPIOn_CRL) @ 0x40012004 read-write reset: 0x44444444
      uint32_t MODE8 : 2; // Port n.8 mode bits  reset: 0
      uint32_t CNF8 : 2; // Port n.8 configuration bits  reset: 1
      uint32_t MODE9 : 2; // Port n.9 mode bits  reset: 0
      uint32_t CNF9 : 2; // Port n.9 configuration bits  reset: 1
      uint32_t MODE10 : 2; // Port n.10 mode bits  reset: 0
      uint32_t CNF10 : 2; // Port n.10 configuration bits  reset: 1
      uint32_t MODE11 : 2; // Port n.11 mode bits  reset: 0
      uint32_t CNF11 : 2; // Port n.11 configuration bits  reset: 1
      uint32_t MODE12 : 2; // Port n.12 mode bits  reset: 0
      uint32_t CNF12 : 2; // Port n.12 configuration bits  reset: 1
      uint32_t MODE13 : 2; // Port n.13 mode bits  reset: 0
      uint32_t CNF13 : 2; // Port n.13 configuration bits  reset: 1
      uint32_t MODE14 : 2; // Port n.14 mode bits  reset: 0
      uint32_t CNF14 : 2; // Port n.14 configuration bits  reset: 1
      uint32_t MODE15 : 2; // Port n.15 mode bits  reset: 0
      uint32_t CNF15 : 2; // Port n.15 configuration bits  reset: 1
    };
  } CRH;
  union{
    volatile uint32_t w;
    struct { // Port input data register (GPIOn_IDR) @ 0x40012008 read-only reset: 0x00000000
      uint32_t IDR0 : 1; // Port input data  reset: 0
      uint32_t IDR1 : 1; // Port input data  reset: 0
      uint32_t IDR2 : 1; // Port input data  reset: 0
      uint32_t IDR3 : 1; // Port input data  reset: 0
      uint32_t IDR4 : 1; // Port input data  reset: 0
      uint32_t IDR5 : 1; // Port input data  reset: 0
      uint32_t IDR6 : 1; // Port input data  reset: 0
      uint32_t IDR7 : 1; // Port input data  reset: 0
      uint32_t IDR8 : 1; // Port input data  reset: 0
      uint32_t IDR9 : 1; // Port input data  reset: 0
      uint32_t IDR10 : 1; // Port input data  reset: 0
      uint32_t IDR11 : 1; // Port input data  reset: 0
      uint32_t IDR12 : 1; // Port input data  reset: 0
      uint32_t IDR13 : 1; // Port input data  reset: 0
      uint32_t IDR14 : 1; // Port input data  reset: 0
      uint32_t IDR15 : 1; // Port input data  reset: 0
      uint32_t RESERVED_477 : 16;
    };
  } IDR;
  union{
    volatile uint32_t w;
    struct { // Port output data register (GPIOn_ODR) @ 0x4001200C read-write reset: 0x00000000
      uint32_t ODR0 : 1; // Port output data  reset: 0
      uint32_t ODR1 : 1; // Port output data  reset: 0
      uint32_t ODR2 : 1; // Port output data  reset: 0
      uint32_t ODR3 : 1; // Port output data  reset: 0
      uint32_t ODR4 : 1; // Port output data  reset: 0
      uint32_t ODR5 : 1; // Port output data  reset: 0
      uint32_t ODR6 : 1; // Port output data  reset: 0
      uint32_t ODR7 : 1; // Port output data  reset: 0
      uint32_t ODR8 : 1; // Port output data  reset: 0
      uint32_t ODR9 : 1; // Port output data  reset: 0
      uint32_t ODR10 : 1; // Port output data  reset: 0
      uint32_t ODR11 : 1; // Port output data  reset: 0
      uint32_t ODR12 : 1; // Port output data  reset: 0
      uint32_t ODR13 : 1; // Port output data  reset: 0
      uint32_t ODR14 : 1; // Port output data  reset: 0
      uint32_t ODR15 : 1; // Port output data  reset: 0
      uint32_t RESERVED_478 : 16;
    };
  } ODR;
  union{
    volatile uint32_t w;
    struct { // Port bit set/reset register (GPIOn_BSRR) @ 0x40012010 write-only reset: 0x00000000
      uint32_t BS0 : 1; // Set bit 0  reset: 0
      uint32_t BS1 : 1; // Set bit 1  reset: 0
      uint32_t BS2 : 1; // Set bit 1  reset: 0
      uint32_t BS3 : 1; // Set bit 3  reset: 0
      uint32_t BS4 : 1; // Set bit 4  reset: 0
      uint32_t BS5 : 1; // Set bit 5  reset: 0
      uint32_t BS6 : 1; // Set bit 6  reset: 0
      uint32_t BS7 : 1; // Set bit 7  reset: 0
      uint32_t BS8 : 1; // Set bit 8  reset: 0
      uint32_t BS9 : 1; // Set bit 9  reset: 0
      uint32_t BS10 : 1; // Set bit 10  reset: 0
      uint32_t BS11 : 1; // Set bit 11  reset: 0
      uint32_t BS12 : 1; // Set bit 12  reset: 0
      uint32_t BS13 : 1; // Set bit 13  reset: 0
      uint32_t BS14 : 1; // Set bit 14  reset: 0
      uint32_t BS15 : 1; // Set bit 15  reset: 0
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 2  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
    };
  } BSRR;
  union{
    volatile uint32_t w;
    struct { // Port bit reset register (GPIOn_BRR) @ 0x40012014 write-only reset: 0x00000000
      uint32_t BR0 : 1; // Reset bit 0  reset: 0
      uint32_t BR1 : 1; // Reset bit 1  reset: 0
      uint32_t BR2 : 1; // Reset bit 1  reset: 0
      uint32_t BR3 : 1; // Reset bit 3  reset: 0
      uint32_t BR4 : 1; // Reset bit 4  reset: 0
      uint32_t BR5 : 1; // Reset bit 5  reset: 0
      uint32_t BR6 : 1; // Reset bit 6  reset: 0
      uint32_t BR7 : 1; // Reset bit 7  reset: 0
      uint32_t BR8 : 1; // Reset bit 8  reset: 0
      uint32_t BR9 : 1; // Reset bit 9  reset: 0
      uint32_t BR10 : 1; // Reset bit 10  reset: 0
      uint32_t BR11 : 1; // Reset bit 11  reset: 0
      uint32_t BR12 : 1; // Reset bit 12  reset: 0
      uint32_t BR13 : 1; // Reset bit 13  reset: 0
      uint32_t BR14 : 1; // Reset bit 14  reset: 0
      uint32_t BR15 : 1; // Reset bit 15  reset: 0
      uint32_t RESERVED_479 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Port configuration lock register @ 0x40012018 read-write reset: 0x00000000
      uint32_t LCK0 : 1; // Port A Lock bit 0  reset: 0
      uint32_t LCK1 : 1; // Port A Lock bit 1  reset: 0
      uint32_t LCK2 : 1; // Port A Lock bit 2  reset: 0
      uint32_t LCK3 : 1; // Port A Lock bit 3  reset: 0
      uint32_t LCK4 : 1; // Port A Lock bit 4  reset: 0
      uint32_t LCK5 : 1; // Port A Lock bit 5  reset: 0
      uint32_t LCK6 : 1; // Port A Lock bit 6  reset: 0
      uint32_t LCK7 : 1; // Port A Lock bit 7  reset: 0
      uint32_t LCK8 : 1; // Port A Lock bit 8  reset: 0
      uint32_t LCK9 : 1; // Port A Lock bit 9  reset: 0
      uint32_t LCK10 : 1; // Port A Lock bit 10  reset: 0
      uint32_t LCK11 : 1; // Port A Lock bit 11  reset: 0
      uint32_t LCK12 : 1; // Port A Lock bit 12  reset: 0
      uint32_t LCK13 : 1; // Port A Lock bit 13  reset: 0
      uint32_t LCK14 : 1; // Port A Lock bit 14  reset: 0
      uint32_t LCK15 : 1; // Port A Lock bit 15  reset: 0
      uint32_t LCKK : 1; // Lock key  reset: 0
      uint32_t RESERVED_480 : 15;
    };
  } LCKR;
} GPIOG_t;

// ADC1 register struct @ 0x40012400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40012400 read-write reset: 0x00000000
      uint32_t AWD : 1; // Analog watchdog flag  reset: 0
      uint32_t EOC : 1; // Regular channel end of conversion  reset: 0
      uint32_t JEOC : 1; // Injected channel end of conversion  reset: 0
      uint32_t JSTRT : 1; // Injected channel start flag  reset: 0
      uint32_t STRT : 1; // Regular channel start flag  reset: 0
      uint32_t RESERVED_481 : 27;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40012404 read-write reset: 0x00000000
      uint32_t AWDCH : 5; // Analog watchdog channel select bits  reset: 0
      uint32_t EOCIE : 1; // Interrupt enable for EOC  reset: 0
      uint32_t AWDIE : 1; // Analog watchdog interrupt enable  reset: 0
      uint32_t JEOCIE : 1; // Interrupt enable for injected channels  reset: 0
      uint32_t SCAN : 1; // Scan mode  reset: 0
      uint32_t AWDSGL : 1; // Enable the watchdog on a single channel in scan mode  reset: 0
      uint32_t JAUTO : 1; // Automatic injected group conversion  reset: 0
      uint32_t DISCEN : 1; // Discontinuous mode on regular channels  reset: 0
      uint32_t JDISCEN : 1; // Discontinuous mode on injected channels  reset: 0
      uint32_t DISCNUM : 3; // Discontinuous mode channel count  reset: 0
      uint32_t DUALMOD : 4; // Dual mode selection  reset: 0
      uint32_t RESERVED_482 : 2;
      uint32_t JAWDEN : 1; // Analog watchdog enable on injected channels  reset: 0
      uint32_t AWDEN : 1; // Analog watchdog enable on regular channels  reset: 0
      uint32_t RESERVED_483 : 8;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40012408 read-write reset: 0x00000000
      uint32_t ADON : 1; // A/D converter ON / OFF  reset: 0
      uint32_t CONT : 1; // Continuous conversion  reset: 0
      uint32_t CAL : 1; // A/D calibration  reset: 0
      uint32_t RSTCAL : 1; // Reset calibration  reset: 0
      uint32_t RESERVED_484 : 4;
      uint32_t DMA : 1; // Direct memory access mode  reset: 0
      uint32_t RESERVED_485 : 2;
      uint32_t ALIGN : 1; // Data alignment  reset: 0
      uint32_t JEXTSEL : 3; // External event select for injected group  reset: 0
      uint32_t JEXTTRIG : 1; // External trigger conversion mode for injected channels  reset: 0
      uint32_t RESERVED_486 : 1;
      uint32_t EXTSEL : 3; // External event select for regular group  reset: 0
      uint32_t EXTTRIG : 1; // External trigger conversion mode for regular channels  reset: 0
      uint32_t JSWSTART : 1; // Start conversion of injected channels  reset: 0
      uint32_t SWSTART : 1; // Start conversion of regular channels  reset: 0
      uint32_t TSVREFE : 1; // Temperature sensor and VREFINT enable  reset: 0
      uint32_t RESERVED_487 : 8;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // sample time register 1 @ 0x4001240C read-write reset: 0x00000000
      uint32_t SMP10 : 3; // Channel 10 sample time selection  reset: 0
      uint32_t SMP11 : 3; // Channel 11 sample time selection  reset: 0
      uint32_t SMP12 : 3; // Channel 12 sample time selection  reset: 0
      uint32_t SMP13 : 3; // Channel 13 sample time selection  reset: 0
      uint32_t SMP14 : 3; // Channel 14 sample time selection  reset: 0
      uint32_t SMP15 : 3; // Channel 15 sample time selection  reset: 0
      uint32_t SMP16 : 3; // Channel 16 sample time selection  reset: 0
      uint32_t SMP17 : 3; // Channel 17 sample time selection  reset: 0
      uint32_t RESERVED_488 : 8;
    };
  } SMPR1;
  union{
    volatile uint32_t w;
    struct { // sample time register 2 @ 0x40012410 read-write reset: 0x00000000
      uint32_t SMP0 : 3; // Channel 0 sample time selection  reset: 0
      uint32_t SMP1 : 3; // Channel 1 sample time selection  reset: 0
      uint32_t SMP2 : 3; // Channel 2 sample time selection  reset: 0
      uint32_t SMP3 : 3; // Channel 3 sample time selection  reset: 0
      uint32_t SMP4 : 3; // Channel 4 sample time selection  reset: 0
      uint32_t SMP5 : 3; // Channel 5 sample time selection  reset: 0
      uint32_t SMP6 : 3; // Channel 6 sample time selection  reset: 0
      uint32_t SMP7 : 3; // Channel 7 sample time selection  reset: 0
      uint32_t SMP8 : 3; // Channel 8 sample time selection  reset: 0
      uint32_t SMP9 : 3; // Channel 9 sample time selection  reset: 0
      uint32_t RESERVED_489 : 2;
    };
  } SMPR2;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40012414 read-write reset: 0x00000000
      uint32_t JOFFSET1 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_490 : 20;
    };
  } JOFR1;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40012418 read-write reset: 0x00000000
      uint32_t JOFFSET2 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_491 : 20;
    };
  } JOFR2;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x4001241C read-write reset: 0x00000000
      uint32_t JOFFSET3 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_492 : 20;
    };
  } JOFR3;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40012420 read-write reset: 0x00000000
      uint32_t JOFFSET4 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_493 : 20;
    };
  } JOFR4;
  union{
    volatile uint32_t w;
    struct { // watchdog higher threshold register @ 0x40012424 read-write reset: 0x00000FFF
      uint32_t HT : 12; // Analog watchdog higher threshold  reset: 4095
      uint32_t RESERVED_494 : 20;
    };
  } HTR;
  union{
    volatile uint32_t w;
    struct { // watchdog lower threshold register @ 0x40012428 read-write reset: 0x00000000
      uint32_t LT : 12; // Analog watchdog lower threshold  reset: 0
      uint32_t RESERVED_495 : 20;
    };
  } LTR;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 1 @ 0x4001242C read-write reset: 0x00000000
      uint32_t SQ13 : 5; // 13th conversion in regular sequence  reset: 0
      uint32_t SQ14 : 5; // 14th conversion in regular sequence  reset: 0
      uint32_t SQ15 : 5; // 15th conversion in regular sequence  reset: 0
      uint32_t SQ16 : 5; // 16th conversion in regular sequence  reset: 0
      uint32_t L : 4; // Regular channel sequence length  reset: 0
      uint32_t RESERVED_496 : 8;
    };
  } SQR1;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 2 @ 0x40012430 read-write reset: 0x00000000
      uint32_t SQ7 : 5; // 7th conversion in regular sequence  reset: 0
      uint32_t SQ8 : 5; // 8th conversion in regular sequence  reset: 0
      uint32_t SQ9 : 5; // 9th conversion in regular sequence  reset: 0
      uint32_t SQ10 : 5; // 10th conversion in regular sequence  reset: 0
      uint32_t SQ11 : 5; // 11th conversion in regular sequence  reset: 0
      uint32_t SQ12 : 5; // 12th conversion in regular sequence  reset: 0
      uint32_t RESERVED_497 : 2;
    };
  } SQR2;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 3 @ 0x40012434 read-write reset: 0x00000000
      uint32_t SQ1 : 5; // 1st conversion in regular sequence  reset: 0
      uint32_t SQ2 : 5; // 2nd conversion in regular sequence  reset: 0
      uint32_t SQ3 : 5; // 3rd conversion in regular sequence  reset: 0
      uint32_t SQ4 : 5; // 4th conversion in regular sequence  reset: 0
      uint32_t SQ5 : 5; // 5th conversion in regular sequence  reset: 0
      uint32_t SQ6 : 5; // 6th conversion in regular sequence  reset: 0
      uint32_t RESERVED_498 : 2;
    };
  } SQR3;
  union{
    volatile uint32_t w;
    struct { // injected sequence register @ 0x40012438 read-write reset: 0x00000000
      uint32_t JSQ1 : 5; // 1st conversion in injected sequence  reset: 0
      uint32_t JSQ2 : 5; // 2nd conversion in injected sequence  reset: 0
      uint32_t JSQ3 : 5; // 3rd conversion in injected sequence  reset: 0
      uint32_t JSQ4 : 5; // 4th conversion in injected sequence  reset: 0
      uint32_t JL : 2; // Injected sequence length  reset: 0
      uint32_t RESERVED_499 : 10;
    };
  } JSQR;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x4001243C read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_500 : 16;
    };
  } JDR1;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40012440 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_501 : 16;
    };
  } JDR2;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40012444 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_502 : 16;
    };
  } JDR3;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40012448 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_503 : 16;
    };
  } JDR4;
  union{
    volatile uint32_t w;
    struct { // regular data register @ 0x4001244C read-only reset: 0x00000000
      uint32_t DATA : 16; // Regular data  reset: 0
      uint32_t ADC2DATA : 16; // ADC2 data  reset: 0
    };
  } DR;
} ADC1_t;

// ADC2 register struct @ 0x40012800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40012800 read-write reset: 0x00000000
      uint32_t AWD : 1; // Analog watchdog flag  reset: 0
      uint32_t EOC : 1; // Regular channel end of conversion  reset: 0
      uint32_t JEOC : 1; // Injected channel end of conversion  reset: 0
      uint32_t JSTRT : 1; // Injected channel start flag  reset: 0
      uint32_t STRT : 1; // Regular channel start flag  reset: 0
      uint32_t RESERVED_504 : 27;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40012804 read-write reset: 0x00000000
      uint32_t AWDCH : 5; // Analog watchdog channel select bits  reset: 0
      uint32_t EOCIE : 1; // Interrupt enable for EOC  reset: 0
      uint32_t AWDIE : 1; // Analog watchdog interrupt enable  reset: 0
      uint32_t JEOCIE : 1; // Interrupt enable for injected channels  reset: 0
      uint32_t SCAN : 1; // Scan mode  reset: 0
      uint32_t AWDSGL : 1; // Enable the watchdog on a single channel in scan mode  reset: 0
      uint32_t JAUTO : 1; // Automatic injected group conversion  reset: 0
      uint32_t DISCEN : 1; // Discontinuous mode on regular channels  reset: 0
      uint32_t JDISCEN : 1; // Discontinuous mode on injected channels  reset: 0
      uint32_t DISCNUM : 3; // Discontinuous mode channel count  reset: 0
      uint32_t RESERVED_505 : 6;
      uint32_t JAWDEN : 1; // Analog watchdog enable on injected channels  reset: 0
      uint32_t AWDEN : 1; // Analog watchdog enable on regular channels  reset: 0
      uint32_t RESERVED_506 : 8;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40012808 read-write reset: 0x00000000
      uint32_t ADON : 1; // A/D converter ON / OFF  reset: 0
      uint32_t CONT : 1; // Continuous conversion  reset: 0
      uint32_t CAL : 1; // A/D calibration  reset: 0
      uint32_t RSTCAL : 1; // Reset calibration  reset: 0
      uint32_t RESERVED_507 : 4;
      uint32_t DMA : 1; // Direct memory access mode  reset: 0
      uint32_t RESERVED_508 : 2;
      uint32_t ALIGN : 1; // Data alignment  reset: 0
      uint32_t JEXTSEL : 3; // External event select for injected group  reset: 0
      uint32_t JEXTTRIG : 1; // External trigger conversion mode for injected channels  reset: 0
      uint32_t RESERVED_509 : 1;
      uint32_t EXTSEL : 3; // External event select for regular group  reset: 0
      uint32_t EXTTRIG : 1; // External trigger conversion mode for regular channels  reset: 0
      uint32_t JSWSTART : 1; // Start conversion of injected channels  reset: 0
      uint32_t SWSTART : 1; // Start conversion of regular channels  reset: 0
      uint32_t TSVREFE : 1; // Temperature sensor and VREFINT enable  reset: 0
      uint32_t RESERVED_510 : 8;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // sample time register 1 @ 0x4001280C read-write reset: 0x00000000
      uint32_t SMP10 : 3; // Channel 10 sample time selection  reset: 0
      uint32_t SMP11 : 3; // Channel 11 sample time selection  reset: 0
      uint32_t SMP12 : 3; // Channel 12 sample time selection  reset: 0
      uint32_t SMP13 : 3; // Channel 13 sample time selection  reset: 0
      uint32_t SMP14 : 3; // Channel 14 sample time selection  reset: 0
      uint32_t SMP15 : 3; // Channel 15 sample time selection  reset: 0
      uint32_t SMP16 : 3; // Channel 16 sample time selection  reset: 0
      uint32_t SMP17 : 3; // Channel 17 sample time selection  reset: 0
      uint32_t RESERVED_511 : 8;
    };
  } SMPR1;
  union{
    volatile uint32_t w;
    struct { // sample time register 2 @ 0x40012810 read-write reset: 0x00000000
      uint32_t SMP0 : 3; // Channel 0 sample time selection  reset: 0
      uint32_t SMP1 : 3; // Channel 1 sample time selection  reset: 0
      uint32_t SMP2 : 3; // Channel 2 sample time selection  reset: 0
      uint32_t SMP3 : 3; // Channel 3 sample time selection  reset: 0
      uint32_t SMP4 : 3; // Channel 4 sample time selection  reset: 0
      uint32_t SMP5 : 3; // Channel 5 sample time selection  reset: 0
      uint32_t SMP6 : 3; // Channel 6 sample time selection  reset: 0
      uint32_t SMP7 : 3; // Channel 7 sample time selection  reset: 0
      uint32_t SMP8 : 3; // Channel 8 sample time selection  reset: 0
      uint32_t SMP9 : 3; // Channel 9 sample time selection  reset: 0
      uint32_t RESERVED_512 : 2;
    };
  } SMPR2;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40012814 read-write reset: 0x00000000
      uint32_t JOFFSET1 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_513 : 20;
    };
  } JOFR1;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40012818 read-write reset: 0x00000000
      uint32_t JOFFSET2 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_514 : 20;
    };
  } JOFR2;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x4001281C read-write reset: 0x00000000
      uint32_t JOFFSET3 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_515 : 20;
    };
  } JOFR3;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40012820 read-write reset: 0x00000000
      uint32_t JOFFSET4 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_516 : 20;
    };
  } JOFR4;
  union{
    volatile uint32_t w;
    struct { // watchdog higher threshold register @ 0x40012824 read-write reset: 0x00000FFF
      uint32_t HT : 12; // Analog watchdog higher threshold  reset: 4095
      uint32_t RESERVED_517 : 20;
    };
  } HTR;
  union{
    volatile uint32_t w;
    struct { // watchdog lower threshold register @ 0x40012828 read-write reset: 0x00000000
      uint32_t LT : 12; // Analog watchdog lower threshold  reset: 0
      uint32_t RESERVED_518 : 20;
    };
  } LTR;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 1 @ 0x4001282C read-write reset: 0x00000000
      uint32_t SQ13 : 5; // 13th conversion in regular sequence  reset: 0
      uint32_t SQ14 : 5; // 14th conversion in regular sequence  reset: 0
      uint32_t SQ15 : 5; // 15th conversion in regular sequence  reset: 0
      uint32_t SQ16 : 5; // 16th conversion in regular sequence  reset: 0
      uint32_t L : 4; // Regular channel sequence length  reset: 0
      uint32_t RESERVED_519 : 8;
    };
  } SQR1;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 2 @ 0x40012830 read-write reset: 0x00000000
      uint32_t SQ7 : 5; // 7th conversion in regular sequence  reset: 0
      uint32_t SQ8 : 5; // 8th conversion in regular sequence  reset: 0
      uint32_t SQ9 : 5; // 9th conversion in regular sequence  reset: 0
      uint32_t SQ10 : 5; // 10th conversion in regular sequence  reset: 0
      uint32_t SQ11 : 5; // 11th conversion in regular sequence  reset: 0
      uint32_t SQ12 : 5; // 12th conversion in regular sequence  reset: 0
      uint32_t RESERVED_520 : 2;
    };
  } SQR2;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 3 @ 0x40012834 read-write reset: 0x00000000
      uint32_t SQ1 : 5; // 1st conversion in regular sequence  reset: 0
      uint32_t SQ2 : 5; // 2nd conversion in regular sequence  reset: 0
      uint32_t SQ3 : 5; // 3rd conversion in regular sequence  reset: 0
      uint32_t SQ4 : 5; // 4th conversion in regular sequence  reset: 0
      uint32_t SQ5 : 5; // 5th conversion in regular sequence  reset: 0
      uint32_t SQ6 : 5; // 6th conversion in regular sequence  reset: 0
      uint32_t RESERVED_521 : 2;
    };
  } SQR3;
  union{
    volatile uint32_t w;
    struct { // injected sequence register @ 0x40012838 read-write reset: 0x00000000
      uint32_t JSQ1 : 5; // 1st conversion in injected sequence  reset: 0
      uint32_t JSQ2 : 5; // 2nd conversion in injected sequence  reset: 0
      uint32_t JSQ3 : 5; // 3rd conversion in injected sequence  reset: 0
      uint32_t JSQ4 : 5; // 4th conversion in injected sequence  reset: 0
      uint32_t JL : 2; // Injected sequence length  reset: 0
      uint32_t RESERVED_522 : 10;
    };
  } JSQR;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x4001283C read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_523 : 16;
    };
  } JDR1;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40012840 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_524 : 16;
    };
  } JDR2;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40012844 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_525 : 16;
    };
  } JDR3;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40012848 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_526 : 16;
    };
  } JDR4;
  union{
    volatile uint32_t w;
    struct { // regular data register @ 0x4001284C read-only reset: 0x00000000
      uint32_t DATA : 16; // Regular data  reset: 0
      uint32_t RESERVED_527 : 16;
    };
  } DR;
} ADC2_t;

// TIM1 register struct @ 0x40012C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40012C00 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t DIR : 1; // Direction  reset: 0
      uint32_t CMS : 2; // Center-aligned mode selection  reset: 0
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_528 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40012C04 read-write reset: 0x00000000
      uint32_t CCPC : 1; // Capture/compare preloaded control  reset: 0
      uint32_t RESERVED_529 : 1;
      uint32_t CCUS : 1; // Capture/compare control update selection  reset: 0
      uint32_t CCDS : 1; // Capture/compare DMA selection  reset: 0
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t TI1S : 1; // TI1 selection  reset: 0
      uint32_t OIS1 : 1; // Output Idle state 1  reset: 0
      uint32_t OIS1N : 1; // Output Idle state 1  reset: 0
      uint32_t OIS2 : 1; // Output Idle state 2  reset: 0
      uint32_t OIS2N : 1; // Output Idle state 2  reset: 0
      uint32_t OIS3 : 1; // Output Idle state 3  reset: 0
      uint32_t OIS3N : 1; // Output Idle state 3  reset: 0
      uint32_t OIS4 : 1; // Output Idle state 4  reset: 0
      uint32_t RESERVED_530 : 17;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // slave mode control register @ 0x40012C08 read-write reset: 0x00000000
      uint32_t SMS : 3; // Slave mode selection  reset: 0
      uint32_t RESERVED_531 : 1;
      uint32_t TS : 3; // Trigger selection  reset: 0
      uint32_t MSM : 1; // Master/Slave mode  reset: 0
      uint32_t ETF : 4; // External trigger filter  reset: 0
      uint32_t ETPS : 2; // External trigger prescaler  reset: 0
      uint32_t ECE : 1; // External clock enable  reset: 0
      uint32_t ETP : 1; // External trigger polarity  reset: 0
      uint32_t RESERVED_532 : 16;
    };
  } SMCR;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x40012C0C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable  reset: 0
      uint32_t CC3IE : 1; // Capture/Compare 3 interrupt enable  reset: 0
      uint32_t CC4IE : 1; // Capture/Compare 4 interrupt enable  reset: 0
      uint32_t COMIE : 1; // COM interrupt enable  reset: 0
      uint32_t TIE : 1; // Trigger interrupt enable  reset: 0
      uint32_t BIE : 1; // Break interrupt enable  reset: 0
      uint32_t UDE : 1; // Update DMA request enable  reset: 0
      uint32_t CC1DE : 1; // Capture/Compare 1 DMA request enable  reset: 0
      uint32_t CC2DE : 1; // Capture/Compare 2 DMA request enable  reset: 0
      uint32_t CC3DE : 1; // Capture/Compare 3 DMA request enable  reset: 0
      uint32_t CC4DE : 1; // Capture/Compare 4 DMA request enable  reset: 0
      uint32_t COMDE : 1; // COM DMA request enable  reset: 0
      uint32_t TDE : 1; // Trigger DMA request enable  reset: 0
      uint32_t RESERVED_533 : 17;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40012C10 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t CC2IF : 1; // Capture/Compare 2 interrupt flag  reset: 0
      uint32_t CC3IF : 1; // Capture/Compare 3 interrupt flag  reset: 0
      uint32_t CC4IF : 1; // Capture/Compare 4 interrupt flag  reset: 0
      uint32_t COMIF : 1; // COM interrupt flag  reset: 0
      uint32_t TIF : 1; // Trigger interrupt flag  reset: 0
      uint32_t BIF : 1; // Break interrupt flag  reset: 0
      uint32_t RESERVED_534 : 1;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t CC2OF : 1; // Capture/compare 2 overcapture flag  reset: 0
      uint32_t CC3OF : 1; // Capture/Compare 3 overcapture flag  reset: 0
      uint32_t CC4OF : 1; // Capture/Compare 4 overcapture flag  reset: 0
      uint32_t RESERVED_535 : 19;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40012C14 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t CC2G : 1; // Capture/compare 2 generation  reset: 0
      uint32_t CC3G : 1; // Capture/compare 3 generation  reset: 0
      uint32_t CC4G : 1; // Capture/compare 4 generation  reset: 0
      uint32_t COMG : 1; // Capture/Compare control update generation  reset: 0
      uint32_t TG : 1; // Trigger generation  reset: 0
      uint32_t BG : 1; // Break generation  reset: 0
      uint32_t RESERVED_536 : 24;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (output mode) @ 0x40012C18 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t OC1FE : 1; // Output Compare 1 fast enable  reset: 0
      uint32_t OC1PE : 1; // Output Compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output Compare 1 mode  reset: 0
      uint32_t OC1CE : 1; // Output Compare 1 clear enable  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t OC2FE : 1; // Output Compare 2 fast enable  reset: 0
      uint32_t OC2PE : 1; // Output Compare 2 preload enable  reset: 0
      uint32_t OC2M : 3; // Output Compare 2 mode  reset: 0
      uint32_t OC2CE : 1; // Output Compare 2 clear enable  reset: 0
      uint32_t RESERVED_537 : 16;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (input mode) @ 0x40012C18 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t ICPCS : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t IC2PCS : 2; // Input capture 2 prescaler  reset: 0
      uint32_t IC2F : 4; // Input capture 2 filter  reset: 0
      uint32_t RESERVED_538 : 16;
    };
  } CCMR1_Input;
  };
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (output mode) @ 0x40012C1C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t OC3FE : 1; // Output compare 3 fast enable  reset: 0
      uint32_t OC3PE : 1; // Output compare 3 preload enable  reset: 0
      uint32_t OC3M : 3; // Output compare 3 mode  reset: 0
      uint32_t OC3CE : 1; // Output compare 3 clear enable  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t OC4FE : 1; // Output compare 4 fast enable  reset: 0
      uint32_t OC4PE : 1; // Output compare 4 preload enable  reset: 0
      uint32_t OC4M : 3; // Output compare 4 mode  reset: 0
      uint32_t OC4CE : 1; // Output compare 4 clear enable  reset: 0
      uint32_t RESERVED_539 : 16;
    };
  } CCMR2_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (input mode) @ 0x40012C1C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/compare 3 selection  reset: 0
      uint32_t IC3PSC : 2; // Input capture 3 prescaler  reset: 0
      uint32_t IC3F : 4; // Input capture 3 filter  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t IC4PSC : 2; // Input capture 4 prescaler  reset: 0
      uint32_t IC4F : 4; // Input capture 4 filter  reset: 0
      uint32_t RESERVED_540 : 16;
    };
  } CCMR2_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40012C20 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t CC1NE : 1; // Capture/Compare 1 complementary output enable  reset: 0
      uint32_t CC1NP : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t CC2E : 1; // Capture/Compare 2 output enable  reset: 0
      uint32_t CC2P : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t CC2NE : 1; // Capture/Compare 2 complementary output enable  reset: 0
      uint32_t CC2NP : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t CC3E : 1; // Capture/Compare 3 output enable  reset: 0
      uint32_t CC3P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t CC3NE : 1; // Capture/Compare 3 complementary output enable  reset: 0
      uint32_t CC3NP : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t CC4E : 1; // Capture/Compare 4 output enable  reset: 0
      uint32_t CC4P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_541 : 18;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40012C24 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_542 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40012C28 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_543 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x40012C2C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_544 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // repetition counter register @ 0x40012C30 read-write reset: 0x00000000
      uint32_t REP : 8; // Repetition counter value  reset: 0
      uint32_t RESERVED_545 : 24;
    };
  } RCR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40012C34 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_546 : 16;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 2 @ 0x40012C38 read-write reset: 0x00000000
      uint32_t CCR2 : 16; // Capture/Compare 2 value  reset: 0
      uint32_t RESERVED_547 : 16;
    };
  } CCR2;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 3 @ 0x40012C3C read-write reset: 0x00000000
      uint32_t CCR3 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_548 : 16;
    };
  } CCR3;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 4 @ 0x40012C40 read-write reset: 0x00000000
      uint32_t CCR4 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_549 : 16;
    };
  } CCR4;
  union{
    volatile uint32_t w;
    struct { // break and dead-time register @ 0x40012C44 read-write reset: 0x00000000
      uint32_t DTG : 8; // Dead-time generator setup  reset: 0
      uint32_t LOCK : 2; // Lock configuration  reset: 0
      uint32_t OSSI : 1; // Off-state selection for Idle mode  reset: 0
      uint32_t OSSR : 1; // Off-state selection for Run mode  reset: 0
      uint32_t BKE : 1; // Break enable  reset: 0
      uint32_t BKP : 1; // Break polarity  reset: 0
      uint32_t AOE : 1; // Automatic output enable  reset: 0
      uint32_t MOE : 1; // Main output enable  reset: 0
      uint32_t RESERVED_550 : 16;
    };
  } BDTR;
  union{
    volatile uint32_t w;
    struct { // DMA control register @ 0x40012C48 read-write reset: 0x00000000
      uint32_t DBA : 5; // DMA base address  reset: 0
      uint32_t RESERVED_551 : 3;
      uint32_t DBL : 5; // DMA burst length  reset: 0
      uint32_t RESERVED_552 : 19;
    };
  } DCR;
  union{
    volatile uint32_t w;
    struct { // DMA address for full transfer @ 0x40012C4C read-write reset: 0x00000000
      uint32_t DMAB : 16; // DMA register for burst accesses  reset: 0
      uint32_t RESERVED_553 : 16;
    };
  } DMAR;
} TIM1_t;

// SPI1 register struct @ 0x40013000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40013000 read-write reset: 0x00000000
      uint32_t CPHA : 1; // Clock phase  reset: 0
      uint32_t CPOL : 1; // Clock polarity  reset: 0
      uint32_t MSTR : 1; // Master selection  reset: 0
      uint32_t BR : 3; // Baud rate control  reset: 0
      uint32_t SPE : 1; // SPI enable  reset: 0
      uint32_t LSBFIRST : 1; // Frame format  reset: 0
      uint32_t SSI : 1; // Internal slave select  reset: 0
      uint32_t SSM : 1; // Software slave management  reset: 0
      uint32_t RXONLY : 1; // Receive only  reset: 0
      uint32_t DFF : 1; // Data frame format  reset: 0
      uint32_t CRCNEXT : 1; // CRC transfer next  reset: 0
      uint32_t CRCEN : 1; // Hardware CRC calculation enable  reset: 0
      uint32_t BIDIOE : 1; // Output enable in bidirectional mode  reset: 0
      uint32_t BIDIMODE : 1; // Bidirectional data mode enable  reset: 0
      uint32_t RESERVED_554 : 16;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40013004 read-write reset: 0x00000000
      uint32_t RXDMAEN : 1; // Rx buffer DMA enable  reset: 0
      uint32_t TXDMAEN : 1; // Tx buffer DMA enable  reset: 0
      uint32_t SSOE : 1; // SS output enable  reset: 0
      uint32_t RESERVED_555 : 2;
      uint32_t ERRIE : 1; // Error interrupt enable  reset: 0
      uint32_t RXNEIE : 1; // RX buffer not empty interrupt enable  reset: 0
      uint32_t TXEIE : 1; // Tx buffer empty interrupt enable  reset: 0
      uint32_t RESERVED_556 : 24;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40013008  reset: 0x00000002
      uint32_t RXNE : 1; // Receive buffer not empty read-only reset: 0
      uint32_t TXE : 1; // Transmit buffer empty read-only reset: 1
      uint32_t CHSIDE : 1; // Channel side read-only reset: 0
      uint32_t UDR : 1; // Underrun flag read-only reset: 0
      uint32_t CRCERR : 1; // CRC error flag read-write reset: 0
      uint32_t MODF : 1; // Mode fault read-only reset: 0
      uint32_t OVR : 1; // Overrun flag read-only reset: 0
      uint32_t BSY : 1; // Busy flag read-only reset: 0
      uint32_t RESERVED_557 : 24;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // data register @ 0x4001300C read-write reset: 0x00000000
      uint32_t DR : 16; // Data register  reset: 0
      uint32_t RESERVED_558 : 16;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // CRC polynomial register @ 0x40013010 read-write reset: 0x00000007
      uint32_t CRCPOLY : 16; // CRC polynomial register  reset: 7
      uint32_t RESERVED_559 : 16;
    };
  } CRCPR;
  union{
    volatile uint32_t w;
    struct { // RX CRC register @ 0x40013014 read-only reset: 0x00000000
      uint32_t RxCRC : 16; // Rx CRC register  reset: 0
      uint32_t RESERVED_560 : 16;
    };
  } RXCRCR;
  union{
    volatile uint32_t w;
    struct { // TX CRC register @ 0x40013018 read-only reset: 0x00000000
      uint32_t TxCRC : 16; // Tx CRC register  reset: 0
      uint32_t RESERVED_561 : 16;
    };
  } TXCRCR;
  union{
    volatile uint32_t w;
    struct { // I2S configuration register @ 0x4001301C read-write reset: 0x00000000
      uint32_t CHLEN : 1; // Channel length (number of bits per audio channel)  reset: 0
      uint32_t DATLEN : 2; // Data length to be transferred  reset: 0
      uint32_t CKPOL : 1; // Steady state clock polarity  reset: 0
      uint32_t I2SSTD : 2; // I2S standard selection  reset: 0
      uint32_t RESERVED_562 : 1;
      uint32_t PCMSYNC : 1; // PCM frame synchronization  reset: 0
      uint32_t I2SCFG : 2; // I2S configuration mode  reset: 0
      uint32_t I2SE : 1; // I2S Enable  reset: 0
      uint32_t I2SMOD : 1; // I2S mode selection  reset: 0
      uint32_t RESERVED_563 : 20;
    };
  } I2SCFGR;
  union{
    volatile uint32_t w;
    struct { // I2S prescaler register @ 0x40013020 read-write reset: 0x0000000A
      uint32_t I2SDIV : 8; // I2S Linear prescaler  reset: 10
      uint32_t ODD : 1; // Odd factor for the prescaler  reset: 0
      uint32_t MCKOE : 1; // Master clock output enable  reset: 0
      uint32_t RESERVED_564 : 22;
    };
  } I2SPR;
} SPI1_t;

// TIM8 register struct @ 0x40013400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40013400 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t DIR : 1; // Direction  reset: 0
      uint32_t CMS : 2; // Center-aligned mode selection  reset: 0
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_565 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40013404 read-write reset: 0x00000000
      uint32_t CCPC : 1; // Capture/compare preloaded control  reset: 0
      uint32_t RESERVED_566 : 1;
      uint32_t CCUS : 1; // Capture/compare control update selection  reset: 0
      uint32_t CCDS : 1; // Capture/compare DMA selection  reset: 0
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t TI1S : 1; // TI1 selection  reset: 0
      uint32_t OIS1 : 1; // Output Idle state 1  reset: 0
      uint32_t OIS1N : 1; // Output Idle state 1  reset: 0
      uint32_t OIS2 : 1; // Output Idle state 2  reset: 0
      uint32_t OIS2N : 1; // Output Idle state 2  reset: 0
      uint32_t OIS3 : 1; // Output Idle state 3  reset: 0
      uint32_t OIS3N : 1; // Output Idle state 3  reset: 0
      uint32_t OIS4 : 1; // Output Idle state 4  reset: 0
      uint32_t RESERVED_567 : 17;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // slave mode control register @ 0x40013408 read-write reset: 0x00000000
      uint32_t SMS : 3; // Slave mode selection  reset: 0
      uint32_t RESERVED_568 : 1;
      uint32_t TS : 3; // Trigger selection  reset: 0
      uint32_t MSM : 1; // Master/Slave mode  reset: 0
      uint32_t ETF : 4; // External trigger filter  reset: 0
      uint32_t ETPS : 2; // External trigger prescaler  reset: 0
      uint32_t ECE : 1; // External clock enable  reset: 0
      uint32_t ETP : 1; // External trigger polarity  reset: 0
      uint32_t RESERVED_569 : 16;
    };
  } SMCR;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4001340C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable  reset: 0
      uint32_t CC3IE : 1; // Capture/Compare 3 interrupt enable  reset: 0
      uint32_t CC4IE : 1; // Capture/Compare 4 interrupt enable  reset: 0
      uint32_t COMIE : 1; // COM interrupt enable  reset: 0
      uint32_t TIE : 1; // Trigger interrupt enable  reset: 0
      uint32_t BIE : 1; // Break interrupt enable  reset: 0
      uint32_t UDE : 1; // Update DMA request enable  reset: 0
      uint32_t CC1DE : 1; // Capture/Compare 1 DMA request enable  reset: 0
      uint32_t CC2DE : 1; // Capture/Compare 2 DMA request enable  reset: 0
      uint32_t CC3DE : 1; // Capture/Compare 3 DMA request enable  reset: 0
      uint32_t CC4DE : 1; // Capture/Compare 4 DMA request enable  reset: 0
      uint32_t COMDE : 1; // COM DMA request enable  reset: 0
      uint32_t TDE : 1; // Trigger DMA request enable  reset: 0
      uint32_t RESERVED_570 : 17;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40013410 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t CC2IF : 1; // Capture/Compare 2 interrupt flag  reset: 0
      uint32_t CC3IF : 1; // Capture/Compare 3 interrupt flag  reset: 0
      uint32_t CC4IF : 1; // Capture/Compare 4 interrupt flag  reset: 0
      uint32_t COMIF : 1; // COM interrupt flag  reset: 0
      uint32_t TIF : 1; // Trigger interrupt flag  reset: 0
      uint32_t BIF : 1; // Break interrupt flag  reset: 0
      uint32_t RESERVED_571 : 1;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t CC2OF : 1; // Capture/compare 2 overcapture flag  reset: 0
      uint32_t CC3OF : 1; // Capture/Compare 3 overcapture flag  reset: 0
      uint32_t CC4OF : 1; // Capture/Compare 4 overcapture flag  reset: 0
      uint32_t RESERVED_572 : 19;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40013414 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t CC2G : 1; // Capture/compare 2 generation  reset: 0
      uint32_t CC3G : 1; // Capture/compare 3 generation  reset: 0
      uint32_t CC4G : 1; // Capture/compare 4 generation  reset: 0
      uint32_t COMG : 1; // Capture/Compare control update generation  reset: 0
      uint32_t TG : 1; // Trigger generation  reset: 0
      uint32_t BG : 1; // Break generation  reset: 0
      uint32_t RESERVED_573 : 24;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (output mode) @ 0x40013418 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t OC1FE : 1; // Output Compare 1 fast enable  reset: 0
      uint32_t OC1PE : 1; // Output Compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output Compare 1 mode  reset: 0
      uint32_t OC1CE : 1; // Output Compare 1 clear enable  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t OC2FE : 1; // Output Compare 2 fast enable  reset: 0
      uint32_t OC2PE : 1; // Output Compare 2 preload enable  reset: 0
      uint32_t OC2M : 3; // Output Compare 2 mode  reset: 0
      uint32_t OC2CE : 1; // Output Compare 2 clear enable  reset: 0
      uint32_t RESERVED_574 : 16;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (input mode) @ 0x40013418 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t ICPCS : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t IC2PCS : 2; // Input capture 2 prescaler  reset: 0
      uint32_t IC2F : 4; // Input capture 2 filter  reset: 0
      uint32_t RESERVED_575 : 16;
    };
  } CCMR1_Input;
  };
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (output mode) @ 0x4001341C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/Compare 3 selection  reset: 0
      uint32_t OC3FE : 1; // Output compare 3 fast enable  reset: 0
      uint32_t OC3PE : 1; // Output compare 3 preload enable  reset: 0
      uint32_t OC3M : 3; // Output compare 3 mode  reset: 0
      uint32_t OC3CE : 1; // Output compare 3 clear enable  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t OC4FE : 1; // Output compare 4 fast enable  reset: 0
      uint32_t OC4PE : 1; // Output compare 4 preload enable  reset: 0
      uint32_t OC4M : 3; // Output compare 4 mode  reset: 0
      uint32_t OC4CE : 1; // Output compare 4 clear enable  reset: 0
      uint32_t RESERVED_576 : 16;
    };
  } CCMR2_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 2 (input mode) @ 0x4001341C read-write reset: 0x00000000
      uint32_t CC3S : 2; // Capture/compare 3 selection  reset: 0
      uint32_t IC3PSC : 2; // Input capture 3 prescaler  reset: 0
      uint32_t IC3F : 4; // Input capture 3 filter  reset: 0
      uint32_t CC4S : 2; // Capture/Compare 4 selection  reset: 0
      uint32_t IC4PSC : 2; // Input capture 4 prescaler  reset: 0
      uint32_t IC4F : 4; // Input capture 4 filter  reset: 0
      uint32_t RESERVED_577 : 16;
    };
  } CCMR2_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40013420 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t CC1NE : 1; // Capture/Compare 1 complementary output enable  reset: 0
      uint32_t CC1NP : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t CC2E : 1; // Capture/Compare 2 output enable  reset: 0
      uint32_t CC2P : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t CC2NE : 1; // Capture/Compare 2 complementary output enable  reset: 0
      uint32_t CC2NP : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t CC3E : 1; // Capture/Compare 3 output enable  reset: 0
      uint32_t CC3P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t CC3NE : 1; // Capture/Compare 3 complementary output enable  reset: 0
      uint32_t CC3NP : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t CC4E : 1; // Capture/Compare 4 output enable  reset: 0
      uint32_t CC4P : 1; // Capture/Compare 3 output Polarity  reset: 0
      uint32_t RESERVED_578 : 18;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40013424 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_579 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40013428 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_580 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4001342C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_581 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // repetition counter register @ 0x40013430 read-write reset: 0x00000000
      uint32_t REP : 8; // Repetition counter value  reset: 0
      uint32_t RESERVED_582 : 24;
    };
  } RCR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40013434 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_583 : 16;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 2 @ 0x40013438 read-write reset: 0x00000000
      uint32_t CCR2 : 16; // Capture/Compare 2 value  reset: 0
      uint32_t RESERVED_584 : 16;
    };
  } CCR2;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 3 @ 0x4001343C read-write reset: 0x00000000
      uint32_t CCR3 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_585 : 16;
    };
  } CCR3;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 4 @ 0x40013440 read-write reset: 0x00000000
      uint32_t CCR4 : 16; // Capture/Compare value  reset: 0
      uint32_t RESERVED_586 : 16;
    };
  } CCR4;
  union{
    volatile uint32_t w;
    struct { // break and dead-time register @ 0x40013444 read-write reset: 0x00000000
      uint32_t DTG : 8; // Dead-time generator setup  reset: 0
      uint32_t LOCK : 2; // Lock configuration  reset: 0
      uint32_t OSSI : 1; // Off-state selection for Idle mode  reset: 0
      uint32_t OSSR : 1; // Off-state selection for Run mode  reset: 0
      uint32_t BKE : 1; // Break enable  reset: 0
      uint32_t BKP : 1; // Break polarity  reset: 0
      uint32_t AOE : 1; // Automatic output enable  reset: 0
      uint32_t MOE : 1; // Main output enable  reset: 0
      uint32_t RESERVED_587 : 16;
    };
  } BDTR;
  union{
    volatile uint32_t w;
    struct { // DMA control register @ 0x40013448 read-write reset: 0x00000000
      uint32_t DBA : 5; // DMA base address  reset: 0
      uint32_t RESERVED_588 : 3;
      uint32_t DBL : 5; // DMA burst length  reset: 0
      uint32_t RESERVED_589 : 19;
    };
  } DCR;
  union{
    volatile uint32_t w;
    struct { // DMA address for full transfer @ 0x4001344C read-write reset: 0x00000000
      uint32_t DMAB : 16; // DMA register for burst accesses  reset: 0
      uint32_t RESERVED_590 : 16;
    };
  } DMAR;
} TIM8_t;

// USART1 register struct @ 0x40013800
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Status register @ 0x40013800  reset: 0x000000C0
      uint32_t PE : 1; // Parity error read-only reset: 0
      uint32_t FE : 1; // Framing error read-only reset: 0
      uint32_t NE : 1; // Noise error flag read-only reset: 0
      uint32_t ORE : 1; // Overrun error read-only reset: 0
      uint32_t IDLE : 1; // IDLE line detected read-only reset: 0
      uint32_t RXNE : 1; // Read data register not empty read-write reset: 0
      uint32_t TC : 1; // Transmission complete read-write reset: 1
      uint32_t TXE : 1; // Transmit data register empty read-only reset: 1
      uint32_t LBD : 1; // LIN break detection flag read-write reset: 0
      uint32_t CTS : 1; // CTS flag read-write reset: 0
      uint32_t RESERVED_591 : 22;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // Data register @ 0x40013804 read-write reset: 0x00000000
      uint32_t DR : 9; // Data value  reset: 0
      uint32_t RESERVED_592 : 23;
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // Baud rate register @ 0x40013808 read-write reset: 0x00000000
      uint32_t DIV_Fraction : 4; // fraction of USARTDIV  reset: 0
      uint32_t DIV_Mantissa : 12; // mantissa of USARTDIV  reset: 0
      uint32_t RESERVED_593 : 16;
    };
  } BRR;
  union{
    volatile uint32_t w;
    struct { // Control register 1 @ 0x4001380C read-write reset: 0x00000000
      uint32_t SBK : 1; // Send break  reset: 0
      uint32_t RWU : 1; // Receiver wakeup  reset: 0
      uint32_t RE : 1; // Receiver enable  reset: 0
      uint32_t TE : 1; // Transmitter enable  reset: 0
      uint32_t IDLEIE : 1; // IDLE interrupt enable  reset: 0
      uint32_t RXNEIE : 1; // RXNE interrupt enable  reset: 0
      uint32_t TCIE : 1; // Transmission complete interrupt enable  reset: 0
      uint32_t TXEIE : 1; // TXE interrupt enable  reset: 0
      uint32_t PEIE : 1; // PE interrupt enable  reset: 0
      uint32_t PS : 1; // Parity selection  reset: 0
      uint32_t PCE : 1; // Parity control enable  reset: 0
      uint32_t WAKE : 1; // Wakeup method  reset: 0
      uint32_t M : 1; // Word length  reset: 0
      uint32_t UE : 1; // USART enable  reset: 0
      uint32_t RESERVED_594 : 18;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // Control register 2 @ 0x40013810 read-write reset: 0x00000000
      uint32_t ADD : 4; // Address of the USART node  reset: 0
      uint32_t RESERVED_595 : 1;
      uint32_t LBDL : 1; // lin break detection length  reset: 0
      uint32_t LBDIE : 1; // LIN break detection interrupt enable  reset: 0
      uint32_t RESERVED_596 : 1;
      uint32_t LBCL : 1; // Last bit clock pulse  reset: 0
      uint32_t CPHA : 1; // Clock phase  reset: 0
      uint32_t CPOL : 1; // Clock polarity  reset: 0
      uint32_t CLKEN : 1; // Clock enable  reset: 0
      uint32_t STOP : 2; // STOP bits  reset: 0
      uint32_t LINEN : 1; // LIN mode enable  reset: 0
      uint32_t RESERVED_597 : 17;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // Control register 3 @ 0x40013814 read-write reset: 0x00000000
      uint32_t EIE : 1; // Error interrupt enable  reset: 0
      uint32_t IREN : 1; // IrDA mode enable  reset: 0
      uint32_t IRLP : 1; // IrDA low-power  reset: 0
      uint32_t HDSEL : 1; // Half-duplex selection  reset: 0
      uint32_t NACK : 1; // Smartcard NACK enable  reset: 0
      uint32_t SCEN : 1; // Smartcard mode enable  reset: 0
      uint32_t DMAR : 1; // DMA enable receiver  reset: 0
      uint32_t DMAT : 1; // DMA enable transmitter  reset: 0
      uint32_t RTSE : 1; // RTS enable  reset: 0
      uint32_t CTSE : 1; // CTS enable  reset: 0
      uint32_t CTSIE : 1; // CTS interrupt enable  reset: 0
      uint32_t RESERVED_598 : 21;
    };
  } CR3;
  union{
    volatile uint32_t w;
    struct { // Guard time and prescaler register @ 0x40013818 read-write reset: 0x00000000
      uint32_t PSC : 8; // Prescaler value  reset: 0
      uint32_t GT : 8; // Guard time value  reset: 0
      uint32_t RESERVED_599 : 16;
    };
  } GTPR;
} USART1_t;

// ADC3 register struct @ 0x40013C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40013C00 read-write reset: 0x00000000
      uint32_t AWD : 1; // Analog watchdog flag  reset: 0
      uint32_t EOC : 1; // Regular channel end of conversion  reset: 0
      uint32_t JEOC : 1; // Injected channel end of conversion  reset: 0
      uint32_t JSTRT : 1; // Injected channel start flag  reset: 0
      uint32_t STRT : 1; // Regular channel start flag  reset: 0
      uint32_t RESERVED_600 : 27;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40013C04 read-write reset: 0x00000000
      uint32_t AWDCH : 5; // Analog watchdog channel select bits  reset: 0
      uint32_t EOCIE : 1; // Interrupt enable for EOC  reset: 0
      uint32_t AWDIE : 1; // Analog watchdog interrupt enable  reset: 0
      uint32_t JEOCIE : 1; // Interrupt enable for injected channels  reset: 0
      uint32_t SCAN : 1; // Scan mode  reset: 0
      uint32_t AWDSGL : 1; // Enable the watchdog on a single channel in scan mode  reset: 0
      uint32_t JAUTO : 1; // Automatic injected group conversion  reset: 0
      uint32_t DISCEN : 1; // Discontinuous mode on regular channels  reset: 0
      uint32_t JDISCEN : 1; // Discontinuous mode on injected channels  reset: 0
      uint32_t DISCNUM : 3; // Discontinuous mode channel count  reset: 0
      uint32_t RESERVED_601 : 6;
      uint32_t JAWDEN : 1; // Analog watchdog enable on injected channels  reset: 0
      uint32_t AWDEN : 1; // Analog watchdog enable on regular channels  reset: 0
      uint32_t RESERVED_602 : 8;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40013C08 read-write reset: 0x00000000
      uint32_t ADON : 1; // A/D converter ON / OFF  reset: 0
      uint32_t CONT : 1; // Continuous conversion  reset: 0
      uint32_t CAL : 1; // A/D calibration  reset: 0
      uint32_t RSTCAL : 1; // Reset calibration  reset: 0
      uint32_t RESERVED_603 : 4;
      uint32_t DMA : 1; // Direct memory access mode  reset: 0
      uint32_t RESERVED_604 : 2;
      uint32_t ALIGN : 1; // Data alignment  reset: 0
      uint32_t JEXTSEL : 3; // External event select for injected group  reset: 0
      uint32_t JEXTTRIG : 1; // External trigger conversion mode for injected channels  reset: 0
      uint32_t RESERVED_605 : 1;
      uint32_t EXTSEL : 3; // External event select for regular group  reset: 0
      uint32_t EXTTRIG : 1; // External trigger conversion mode for regular channels  reset: 0
      uint32_t JSWSTART : 1; // Start conversion of injected channels  reset: 0
      uint32_t SWSTART : 1; // Start conversion of regular channels  reset: 0
      uint32_t TSVREFE : 1; // Temperature sensor and VREFINT enable  reset: 0
      uint32_t RESERVED_606 : 8;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // sample time register 1 @ 0x40013C0C read-write reset: 0x00000000
      uint32_t SMP10 : 3; // Channel 10 sample time selection  reset: 0
      uint32_t SMP11 : 3; // Channel 11 sample time selection  reset: 0
      uint32_t SMP12 : 3; // Channel 12 sample time selection  reset: 0
      uint32_t SMP13 : 3; // Channel 13 sample time selection  reset: 0
      uint32_t SMP14 : 3; // Channel 14 sample time selection  reset: 0
      uint32_t SMP15 : 3; // Channel 15 sample time selection  reset: 0
      uint32_t SMP16 : 3; // Channel 16 sample time selection  reset: 0
      uint32_t SMP17 : 3; // Channel 17 sample time selection  reset: 0
      uint32_t RESERVED_607 : 8;
    };
  } SMPR1;
  union{
    volatile uint32_t w;
    struct { // sample time register 2 @ 0x40013C10 read-write reset: 0x00000000
      uint32_t SMP0 : 3; // Channel 0 sample time selection  reset: 0
      uint32_t SMP1 : 3; // Channel 1 sample time selection  reset: 0
      uint32_t SMP2 : 3; // Channel 2 sample time selection  reset: 0
      uint32_t SMP3 : 3; // Channel 3 sample time selection  reset: 0
      uint32_t SMP4 : 3; // Channel 4 sample time selection  reset: 0
      uint32_t SMP5 : 3; // Channel 5 sample time selection  reset: 0
      uint32_t SMP6 : 3; // Channel 6 sample time selection  reset: 0
      uint32_t SMP7 : 3; // Channel 7 sample time selection  reset: 0
      uint32_t SMP8 : 3; // Channel 8 sample time selection  reset: 0
      uint32_t SMP9 : 3; // Channel 9 sample time selection  reset: 0
      uint32_t RESERVED_608 : 2;
    };
  } SMPR2;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40013C14 read-write reset: 0x00000000
      uint32_t JOFFSET1 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_609 : 20;
    };
  } JOFR1;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40013C18 read-write reset: 0x00000000
      uint32_t JOFFSET2 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_610 : 20;
    };
  } JOFR2;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40013C1C read-write reset: 0x00000000
      uint32_t JOFFSET3 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_611 : 20;
    };
  } JOFR3;
  union{
    volatile uint32_t w;
    struct { // injected channel data offset register x @ 0x40013C20 read-write reset: 0x00000000
      uint32_t JOFFSET4 : 12; // Data offset for injected channel x  reset: 0
      uint32_t RESERVED_612 : 20;
    };
  } JOFR4;
  union{
    volatile uint32_t w;
    struct { // watchdog higher threshold register @ 0x40013C24 read-write reset: 0x00000FFF
      uint32_t HT : 12; // Analog watchdog higher threshold  reset: 4095
      uint32_t RESERVED_613 : 20;
    };
  } HTR;
  union{
    volatile uint32_t w;
    struct { // watchdog lower threshold register @ 0x40013C28 read-write reset: 0x00000000
      uint32_t LT : 12; // Analog watchdog lower threshold  reset: 0
      uint32_t RESERVED_614 : 20;
    };
  } LTR;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 1 @ 0x40013C2C read-write reset: 0x00000000
      uint32_t SQ13 : 5; // 13th conversion in regular sequence  reset: 0
      uint32_t SQ14 : 5; // 14th conversion in regular sequence  reset: 0
      uint32_t SQ15 : 5; // 15th conversion in regular sequence  reset: 0
      uint32_t SQ16 : 5; // 16th conversion in regular sequence  reset: 0
      uint32_t L : 4; // Regular channel sequence length  reset: 0
      uint32_t RESERVED_615 : 8;
    };
  } SQR1;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 2 @ 0x40013C30 read-write reset: 0x00000000
      uint32_t SQ7 : 5; // 7th conversion in regular sequence  reset: 0
      uint32_t SQ8 : 5; // 8th conversion in regular sequence  reset: 0
      uint32_t SQ9 : 5; // 9th conversion in regular sequence  reset: 0
      uint32_t SQ10 : 5; // 10th conversion in regular sequence  reset: 0
      uint32_t SQ11 : 5; // 11th conversion in regular sequence  reset: 0
      uint32_t SQ12 : 5; // 12th conversion in regular sequence  reset: 0
      uint32_t RESERVED_616 : 2;
    };
  } SQR2;
  union{
    volatile uint32_t w;
    struct { // regular sequence register 3 @ 0x40013C34 read-write reset: 0x00000000
      uint32_t SQ1 : 5; // 1st conversion in regular sequence  reset: 0
      uint32_t SQ2 : 5; // 2nd conversion in regular sequence  reset: 0
      uint32_t SQ3 : 5; // 3rd conversion in regular sequence  reset: 0
      uint32_t SQ4 : 5; // 4th conversion in regular sequence  reset: 0
      uint32_t SQ5 : 5; // 5th conversion in regular sequence  reset: 0
      uint32_t SQ6 : 5; // 6th conversion in regular sequence  reset: 0
      uint32_t RESERVED_617 : 2;
    };
  } SQR3;
  union{
    volatile uint32_t w;
    struct { // injected sequence register @ 0x40013C38 read-write reset: 0x00000000
      uint32_t JSQ1 : 5; // 1st conversion in injected sequence  reset: 0
      uint32_t JSQ2 : 5; // 2nd conversion in injected sequence  reset: 0
      uint32_t JSQ3 : 5; // 3rd conversion in injected sequence  reset: 0
      uint32_t JSQ4 : 5; // 4th conversion in injected sequence  reset: 0
      uint32_t JL : 2; // Injected sequence length  reset: 0
      uint32_t RESERVED_618 : 10;
    };
  } JSQR;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40013C3C read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_619 : 16;
    };
  } JDR1;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40013C40 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_620 : 16;
    };
  } JDR2;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40013C44 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_621 : 16;
    };
  } JDR3;
  union{
    volatile uint32_t w;
    struct { // injected data register x @ 0x40013C48 read-only reset: 0x00000000
      uint32_t JDATA : 16; // Injected data  reset: 0
      uint32_t RESERVED_622 : 16;
    };
  } JDR4;
  union{
    volatile uint32_t w;
    struct { // regular data register @ 0x40013C4C read-only reset: 0x00000000
      uint32_t DATA : 16; // Regular data  reset: 0
      uint32_t RESERVED_623 : 16;
    };
  } DR;
} ADC3_t;

// TIM9 register struct @ 0x40014C00
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40014C00 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t OPM : 1; // One-pulse mode  reset: 0
      uint32_t RESERVED_624 : 3;
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_625 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40014C04 read-write reset: 0x00000000
      uint32_t RESERVED_626 : 4;
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t RESERVED_627 : 25;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // slave mode control register @ 0x40014C08 read-write reset: 0x00000000
      uint32_t SMS : 3; // Slave mode selection  reset: 0
      uint32_t RESERVED_628 : 1;
      uint32_t TS : 3; // Trigger selection  reset: 0
      uint32_t MSM : 1; // Master/Slave mode  reset: 0
      uint32_t RESERVED_629 : 24;
    };
  } SMCR;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x40014C0C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t CC2IE : 1; // Capture/Compare 2 interrupt enable  reset: 0
      uint32_t RESERVED_630 : 3;
      uint32_t TIE : 1; // Trigger interrupt enable  reset: 0
      uint32_t RESERVED_631 : 25;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40014C10 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t CC2IF : 1; // Capture/Compare 2 interrupt flag  reset: 0
      uint32_t RESERVED_632 : 3;
      uint32_t TIF : 1; // Trigger interrupt flag  reset: 0
      uint32_t RESERVED_633 : 2;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t CC2OF : 1; // Capture/compare 2 overcapture flag  reset: 0
      uint32_t RESERVED_634 : 21;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40014C14 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t CC2G : 1; // Capture/compare 2 generation  reset: 0
      uint32_t RESERVED_635 : 3;
      uint32_t TG : 1; // Trigger generation  reset: 0
      uint32_t RESERVED_636 : 25;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (output mode) @ 0x40014C18 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t OC1FE : 1; // Output Compare 1 fast enable  reset: 0
      uint32_t OC1PE : 1; // Output Compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output Compare 1 mode  reset: 0
      uint32_t RESERVED_637 : 1;
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t OC2FE : 1; // Output Compare 2 fast enable  reset: 0
      uint32_t OC2PE : 1; // Output Compare 2 preload enable  reset: 0
      uint32_t OC2M : 3; // Output Compare 2 mode  reset: 0
      uint32_t RESERVED_638 : 17;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register 1 (input mode) @ 0x40014C18 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t CC2S : 2; // Capture/Compare 2 selection  reset: 0
      uint32_t IC2PSC : 2; // Input capture 2 prescaler  reset: 0
      uint32_t IC2F : 4; // Input capture 2 filter  reset: 0
      uint32_t RESERVED_639 : 16;
    };
  } CCMR1_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40014C20 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_640 : 1;
      uint32_t CC1NP : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t CC2E : 1; // Capture/Compare 2 output enable  reset: 0
      uint32_t CC2P : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t RESERVED_641 : 1;
      uint32_t CC2NP : 1; // Capture/Compare 2 output Polarity  reset: 0
      uint32_t RESERVED_642 : 24;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40014C24 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_643 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40014C28 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_644 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x40014C2C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_645 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40014C34 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_646 : 16;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 2 @ 0x40014C38 read-write reset: 0x00000000
      uint32_t CCR2 : 16; // Capture/Compare 2 value  reset: 0
      uint32_t RESERVED_647 : 16;
    };
  } CCR2;
} TIM9_t;

// TIM10 register struct @ 0x40015000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40015000 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t RESERVED_648 : 4;
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_649 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40015004 read-write reset: 0x00000000
      uint32_t RESERVED_650 : 4;
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t RESERVED_651 : 25;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4001500C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t RESERVED_652 : 30;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40015010 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t RESERVED_653 : 7;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t RESERVED_654 : 22;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40015014 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t RESERVED_655 : 30;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (output mode) @ 0x40015018 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t RESERVED_656 : 1;
      uint32_t OC1PE : 1; // Output Compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output Compare 1 mode  reset: 0
      uint32_t RESERVED_657 : 25;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (input mode) @ 0x40015018 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t RESERVED_658 : 24;
    };
  } CCMR1_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40015020 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_659 : 1;
      uint32_t CC1NP : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_660 : 28;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40015024 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_661 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40015028 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_662 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4001502C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_663 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40015034 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_664 : 16;
    };
  } CCR1;
} TIM10_t;

// TIM11 register struct @ 0x40015400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // control register 1 @ 0x40015400 read-write reset: 0x00000000
      uint32_t CEN : 1; // Counter enable  reset: 0
      uint32_t UDIS : 1; // Update disable  reset: 0
      uint32_t URS : 1; // Update request source  reset: 0
      uint32_t RESERVED_665 : 4;
      uint32_t ARPE : 1; // Auto-reload preload enable  reset: 0
      uint32_t CKD : 2; // Clock division  reset: 0
      uint32_t RESERVED_666 : 22;
    };
  } CR1;
  union{
    volatile uint32_t w;
    struct { // control register 2 @ 0x40015404 read-write reset: 0x00000000
      uint32_t RESERVED_667 : 4;
      uint32_t MMS : 3; // Master mode selection  reset: 0
      uint32_t RESERVED_668 : 25;
    };
  } CR2;
  union{
    volatile uint32_t w;
    struct { // DMA/Interrupt enable register @ 0x4001540C read-write reset: 0x00000000
      uint32_t UIE : 1; // Update interrupt enable  reset: 0
      uint32_t CC1IE : 1; // Capture/Compare 1 interrupt enable  reset: 0
      uint32_t RESERVED_669 : 30;
    };
  } DIER;
  union{
    volatile uint32_t w;
    struct { // status register @ 0x40015410 read-write reset: 0x00000000
      uint32_t UIF : 1; // Update interrupt flag  reset: 0
      uint32_t CC1IF : 1; // Capture/compare 1 interrupt flag  reset: 0
      uint32_t RESERVED_670 : 7;
      uint32_t CC1OF : 1; // Capture/Compare 1 overcapture flag  reset: 0
      uint32_t RESERVED_671 : 22;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // event generation register @ 0x40015414 write-only reset: 0x00000000
      uint32_t UG : 1; // Update generation  reset: 0
      uint32_t CC1G : 1; // Capture/compare 1 generation  reset: 0
      uint32_t RESERVED_672 : 30;
    };
  } EGR;
  union{
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (output mode) @ 0x40015418 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t RESERVED_673 : 1;
      uint32_t OC1PE : 1; // Output Compare 1 preload enable  reset: 0
      uint32_t OC1M : 3; // Output Compare 1 mode  reset: 0
      uint32_t RESERVED_674 : 25;
    };
  } CCMR1_Output;
  union{
    volatile uint32_t w;
    struct { // capture/compare mode register (input mode) @ 0x40015418 read-write reset: 0x00000000
      uint32_t CC1S : 2; // Capture/Compare 1 selection  reset: 0
      uint32_t IC1PSC : 2; // Input capture 1 prescaler  reset: 0
      uint32_t IC1F : 4; // Input capture 1 filter  reset: 0
      uint32_t RESERVED_675 : 24;
    };
  } CCMR1_Input;
  };
  union{
    volatile uint32_t w;
    struct { // capture/compare enable register @ 0x40015420 read-write reset: 0x00000000
      uint32_t CC1E : 1; // Capture/Compare 1 output enable  reset: 0
      uint32_t CC1P : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_676 : 1;
      uint32_t CC1NP : 1; // Capture/Compare 1 output Polarity  reset: 0
      uint32_t RESERVED_677 : 28;
    };
  } CCER;
  union{
    volatile uint32_t w;
    struct { // counter @ 0x40015424 read-write reset: 0x00000000
      uint32_t CNT : 16; // counter value  reset: 0
      uint32_t RESERVED_678 : 16;
    };
  } CNT;
  union{
    volatile uint32_t w;
    struct { // prescaler @ 0x40015428 read-write reset: 0x00000000
      uint32_t PSC : 16; // Prescaler value  reset: 0
      uint32_t RESERVED_679 : 16;
    };
  } PSC;
  union{
    volatile uint32_t w;
    struct { // auto-reload register @ 0x4001542C read-write reset: 0x00000000
      uint32_t ARR : 16; // Auto-reload value  reset: 0
      uint32_t RESERVED_680 : 16;
    };
  } ARR;
  union{
    volatile uint32_t w;
    struct { // capture/compare register 1 @ 0x40015434 read-write reset: 0x00000000
      uint32_t CCR1 : 16; // Capture/Compare 1 value  reset: 0
      uint32_t RESERVED_681 : 16;
    };
  } CCR1;
} TIM11_t;

// SDIO register struct @ 0x40018000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Bits 1:0 = PWRCTRL: Power supply control bits @ 0x40018000 read-write reset: 0x00000000
      uint32_t PWRCTRL : 2; // PWRCTRL  reset: 0
      uint32_t RESERVED_682 : 30;
    };
  } POWER;
  union{
    volatile uint32_t w;
    struct { // SDI clock control register (SDIO_CLKCR) @ 0x40018004 read-write reset: 0x00000000
      uint32_t CLKDIV : 8; // Clock divide factor  reset: 0
      uint32_t CLKEN : 1; // Clock enable bit  reset: 0
      uint32_t PWRSAV : 1; // Power saving configuration bit  reset: 0
      uint32_t BYPASS : 1; // Clock divider bypass enable bit  reset: 0
      uint32_t WIDBUS : 2; // Wide bus mode enable bit  reset: 0
      uint32_t NEGEDGE : 1; // SDIO_CK dephasing selection bit  reset: 0
      uint32_t HWFC_EN : 1; // HW Flow Control enable  reset: 0
      uint32_t RESERVED_683 : 17;
    };
  } CLKCR;
  union{
    volatile uint32_t w;
    struct { // Bits 31:0 = : Command argument @ 0x40018008 read-write reset: 0x00000000
      uint32_t CMDARG : 32; // Command argument  reset: 0
    };
  } ARG;
  union{
    volatile uint32_t w;
    struct { // SDIO command register (SDIO_CMD) @ 0x4001800C read-write reset: 0x00000000
      uint32_t CMDINDEX : 6; // CMDINDEX  reset: 0
      uint32_t WAITRESP : 2; // WAITRESP  reset: 0
      uint32_t WAITINT : 1; // WAITINT  reset: 0
      uint32_t WAITPEND : 1; // WAITPEND  reset: 0
      uint32_t CPSMEN : 1; // CPSMEN  reset: 0
      uint32_t SDIOSuspend : 1; // SDIOSuspend  reset: 0
      uint32_t ENCMDcompl : 1; // ENCMDcompl  reset: 0
      uint32_t nIEN : 1; // nIEN  reset: 0
      uint32_t CE_ATACMD : 1; // CE_ATACMD  reset: 0
      uint32_t RESERVED_684 : 17;
    };
  } CMD;
  union{
    volatile uint32_t w;
    struct { // SDIO command register @ 0x40018010 read-only reset: 0x00000000
      uint32_t RESPCMD : 6; // RESPCMD  reset: 0
      uint32_t RESERVED_685 : 26;
    };
  } RESPCMD;
  union{
    volatile uint32_t w;
    struct { // Bits 31:0 = CARDSTATUS1 @ 0x40018014 read-only reset: 0x00000000
      uint32_t CARDSTATUS1 : 32; // CARDSTATUS1  reset: 0
    };
  } RESPI1;
  union{
    volatile uint32_t w;
    struct { // Bits 31:0 = CARDSTATUS2 @ 0x40018018 read-only reset: 0x00000000
      uint32_t CARDSTATUS2 : 32; // CARDSTATUS2  reset: 0
    };
  } RESP2;
  union{
    volatile uint32_t w;
    struct { // Bits 31:0 = CARDSTATUS3 @ 0x4001801C read-only reset: 0x00000000
      uint32_t CARDSTATUS3 : 32; // CARDSTATUS3  reset: 0
    };
  } RESP3;
  union{
    volatile uint32_t w;
    struct { // Bits 31:0 = CARDSTATUS4 @ 0x40018020 read-only reset: 0x00000000
      uint32_t CARDSTATUS4 : 32; // CARDSTATUS4  reset: 0
    };
  } RESP4;
  union{
    volatile uint32_t w;
    struct { // Bits 31:0 = DATATIME: Data timeout period @ 0x40018024 read-write reset: 0x00000000
      uint32_t DATATIME : 32; // Data timeout period  reset: 0
    };
  } DTIMER;
  union{
    volatile uint32_t w;
    struct { // Bits 24:0 = DATALENGTH: Data length value @ 0x40018028 read-write reset: 0x00000000
      uint32_t DATALENGTH : 25; // Data length value  reset: 0
      uint32_t RESERVED_686 : 7;
    };
  } DLEN;
  union{
    volatile uint32_t w;
    struct { // SDIO data control register (SDIO_DCTRL) @ 0x4001802C read-write reset: 0x00000000
      uint32_t DTEN : 1; // DTEN  reset: 0
      uint32_t DTDIR : 1; // DTDIR  reset: 0
      uint32_t DTMODE : 1; // DTMODE  reset: 0
      uint32_t DMAEN : 1; // DMAEN  reset: 0
      uint32_t DBLOCKSIZE : 4; // DBLOCKSIZE  reset: 0
      uint32_t PWSTART : 1; // PWSTART  reset: 0
      uint32_t PWSTOP : 1; // PWSTOP  reset: 0
      uint32_t RWMOD : 1; // RWMOD  reset: 0
      uint32_t SDIOEN : 1; // SDIOEN  reset: 0
      uint32_t RESERVED_687 : 20;
    };
  } DCTRL;
  union{
    volatile uint32_t w;
    struct { // Bits 24:0 = DATACOUNT: Data count value @ 0x40018030 read-only reset: 0x00000000
      uint32_t DATACOUNT : 25; // Data count value  reset: 0
      uint32_t RESERVED_688 : 7;
    };
  } DCOUNT;
  union{
    volatile uint32_t w;
    struct { // SDIO status register (SDIO_STA) @ 0x40018034 read-only reset: 0x00000000
      uint32_t CCRCFAIL : 1; // CCRCFAIL  reset: 0
      uint32_t DCRCFAIL : 1; // DCRCFAIL  reset: 0
      uint32_t CTIMEOUT : 1; // CTIMEOUT  reset: 0
      uint32_t DTIMEOUT : 1; // DTIMEOUT  reset: 0
      uint32_t TXUNDERR : 1; // TXUNDERR  reset: 0
      uint32_t RXOVERR : 1; // RXOVERR  reset: 0
      uint32_t CMDREND : 1; // CMDREND  reset: 0
      uint32_t CMDSENT : 1; // CMDSENT  reset: 0
      uint32_t DATAEND : 1; // DATAEND  reset: 0
      uint32_t STBITERR : 1; // STBITERR  reset: 0
      uint32_t DBCKEND : 1; // DBCKEND  reset: 0
      uint32_t CMDACT : 1; // CMDACT  reset: 0
      uint32_t TXACT : 1; // TXACT  reset: 0
      uint32_t RXACT : 1; // RXACT  reset: 0
      uint32_t TXFIFOHE : 1; // TXFIFOHE  reset: 0
      uint32_t RXFIFOHF : 1; // RXFIFOHF  reset: 0
      uint32_t TXFIFOF : 1; // TXFIFOF  reset: 0
      uint32_t RXFIFOF : 1; // RXFIFOF  reset: 0
      uint32_t TXFIFOE : 1; // TXFIFOE  reset: 0
      uint32_t RXFIFOE : 1; // RXFIFOE  reset: 0
      uint32_t TXDAVL : 1; // TXDAVL  reset: 0
      uint32_t RXDAVL : 1; // RXDAVL  reset: 0
      uint32_t SDIOIT : 1; // SDIOIT  reset: 0
      uint32_t CEATAEND : 1; // CEATAEND  reset: 0
      uint32_t RESERVED_689 : 8;
    };
  } STA;
  union{
    volatile uint32_t w;
    struct { // SDIO interrupt clear register (SDIO_ICR) @ 0x40018038 read-write reset: 0x00000000
      uint32_t CCRCFAILC : 1; // CCRCFAILC  reset: 0
      uint32_t DCRCFAILC : 1; // DCRCFAILC  reset: 0
      uint32_t CTIMEOUTC : 1; // CTIMEOUTC  reset: 0
      uint32_t DTIMEOUTC : 1; // DTIMEOUTC  reset: 0
      uint32_t TXUNDERRC : 1; // TXUNDERRC  reset: 0
      uint32_t RXOVERRC : 1; // RXOVERRC  reset: 0
      uint32_t CMDRENDC : 1; // CMDRENDC  reset: 0
      uint32_t CMDSENTC : 1; // CMDSENTC  reset: 0
      uint32_t DATAENDC : 1; // DATAENDC  reset: 0
      uint32_t STBITERRC : 1; // STBITERRC  reset: 0
      uint32_t DBCKENDC : 1; // DBCKENDC  reset: 0
      uint32_t RESERVED_690 : 11;
      uint32_t SDIOITC : 1; // SDIOITC  reset: 0
      uint32_t CEATAENDC : 1; // CEATAENDC  reset: 0
      uint32_t RESERVED_691 : 8;
    };
  } ICR;
  union{
    volatile uint32_t w;
    struct { // SDIO mask register (SDIO_MASK) @ 0x4001803C read-write reset: 0x00000000
      uint32_t CCRCFAILIE : 1; // CCRCFAILIE  reset: 0
      uint32_t DCRCFAILIE : 1; // DCRCFAILIE  reset: 0
      uint32_t CTIMEOUTIE : 1; // CTIMEOUTIE  reset: 0
      uint32_t DTIMEOUTIE : 1; // DTIMEOUTIE  reset: 0
      uint32_t TXUNDERRIE : 1; // TXUNDERRIE  reset: 0
      uint32_t RXOVERRIE : 1; // RXOVERRIE  reset: 0
      uint32_t CMDRENDIE : 1; // CMDRENDIE  reset: 0
      uint32_t CMDSENTIE : 1; // CMDSENTIE  reset: 0
      uint32_t DATAENDIE : 1; // DATAENDIE  reset: 0
      uint32_t STBITERRIE : 1; // STBITERRIE  reset: 0
      uint32_t DBACKENDIE : 1; // DBACKENDIE  reset: 0
      uint32_t CMDACTIE : 1; // CMDACTIE  reset: 0
      uint32_t TXACTIE : 1; // TXACTIE  reset: 0
      uint32_t RXACTIE : 1; // RXACTIE  reset: 0
      uint32_t TXFIFOHEIE : 1; // TXFIFOHEIE  reset: 0
      uint32_t RXFIFOHFIE : 1; // RXFIFOHFIE  reset: 0
      uint32_t TXFIFOFIE : 1; // TXFIFOFIE  reset: 0
      uint32_t RXFIFOFIE : 1; // RXFIFOFIE  reset: 0
      uint32_t TXFIFOEIE : 1; // TXFIFOEIE  reset: 0
      uint32_t RXFIFOEIE : 1; // RXFIFOEIE  reset: 0
      uint32_t TXDAVLIE : 1; // TXDAVLIE  reset: 0
      uint32_t RXDAVLIE : 1; // RXDAVLIE  reset: 0
      uint32_t SDIOITIE : 1; // SDIOITIE  reset: 0
      uint32_t CEATENDIE : 1; // CEATENDIE  reset: 0
      uint32_t RESERVED_692 : 8;
    };
  } MASK;
  union{
    volatile uint32_t w;
    struct { // Bits 23:0 = FIFOCOUNT: Remaining number of words to be written to or read from the FIFO @ 0x40018048 read-only reset: 0x00000000
      uint32_t FIF0COUNT : 24; // FIF0COUNT  reset: 0
      uint32_t RESERVED_693 : 8;
    };
  } FIFOCNT;
  const uint8_t RESERVED_694[24];
  union{
    volatile uint32_t w;
    struct { // bits 31:0 = FIFOData: Receive and transmit FIFO data @ 0x40018080 read-write reset: 0x00000000
      uint32_t FIFOData : 32; // FIFOData  reset: 0
    };
  } FIFO;
} SDIO_t;

// DMA1 register struct @ 0x40020000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // DMA interrupt status register (DMA_ISR) @ 0x40020000 read-only reset: 0x00000000
      uint32_t GIF1 : 1; // Channel 1 Global interrupt flag  reset: 0
      uint32_t TCIF1 : 1; // Channel 1 Transfer Complete flag  reset: 0
      uint32_t HTIF1 : 1; // Channel 1 Half Transfer Complete flag  reset: 0
      uint32_t TEIF1 : 1; // Channel 1 Transfer Error flag  reset: 0
      uint32_t GIF2 : 1; // Channel 2 Global interrupt flag  reset: 0
      uint32_t TCIF2 : 1; // Channel 2 Transfer Complete flag  reset: 0
      uint32_t HTIF2 : 1; // Channel 2 Half Transfer Complete flag  reset: 0
      uint32_t TEIF2 : 1; // Channel 2 Transfer Error flag  reset: 0
      uint32_t GIF3 : 1; // Channel 3 Global interrupt flag  reset: 0
      uint32_t TCIF3 : 1; // Channel 3 Transfer Complete flag  reset: 0
      uint32_t HTIF3 : 1; // Channel 3 Half Transfer Complete flag  reset: 0
      uint32_t TEIF3 : 1; // Channel 3 Transfer Error flag  reset: 0
      uint32_t GIF4 : 1; // Channel 4 Global interrupt flag  reset: 0
      uint32_t TCIF4 : 1; // Channel 4 Transfer Complete flag  reset: 0
      uint32_t HTIF4 : 1; // Channel 4 Half Transfer Complete flag  reset: 0
      uint32_t TEIF4 : 1; // Channel 4 Transfer Error flag  reset: 0
      uint32_t GIF5 : 1; // Channel 5 Global interrupt flag  reset: 0
      uint32_t TCIF5 : 1; // Channel 5 Transfer Complete flag  reset: 0
      uint32_t HTIF5 : 1; // Channel 5 Half Transfer Complete flag  reset: 0
      uint32_t TEIF5 : 1; // Channel 5 Transfer Error flag  reset: 0
      uint32_t GIF6 : 1; // Channel 6 Global interrupt flag  reset: 0
      uint32_t TCIF6 : 1; // Channel 6 Transfer Complete flag  reset: 0
      uint32_t HTIF6 : 1; // Channel 6 Half Transfer Complete flag  reset: 0
      uint32_t TEIF6 : 1; // Channel 6 Transfer Error flag  reset: 0
      uint32_t GIF7 : 1; // Channel 7 Global interrupt flag  reset: 0
      uint32_t TCIF7 : 1; // Channel 7 Transfer Complete flag  reset: 0
      uint32_t HTIF7 : 1; // Channel 7 Half Transfer Complete flag  reset: 0
      uint32_t TEIF7 : 1; // Channel 7 Transfer Error flag  reset: 0
      uint32_t RESERVED_695 : 4;
    };
  } ISR;
  union{
    volatile uint32_t w;
    struct { // DMA interrupt flag clear register (DMA_IFCR) @ 0x40020004 write-only reset: 0x00000000
      uint32_t CGIF1 : 1; // Channel 1 Global interrupt clear  reset: 0
      uint32_t CTCIF1 : 1; // Channel 1 Transfer Complete clear  reset: 0
      uint32_t CHTIF1 : 1; // Channel 1 Half Transfer clear  reset: 0
      uint32_t CTEIF1 : 1; // Channel 1 Transfer Error clear  reset: 0
      uint32_t CGIF2 : 1; // Channel 2 Global interrupt clear  reset: 0
      uint32_t CTCIF2 : 1; // Channel 2 Transfer Complete clear  reset: 0
      uint32_t CHTIF2 : 1; // Channel 2 Half Transfer clear  reset: 0
      uint32_t CTEIF2 : 1; // Channel 2 Transfer Error clear  reset: 0
      uint32_t CGIF3 : 1; // Channel 3 Global interrupt clear  reset: 0
      uint32_t CTCIF3 : 1; // Channel 3 Transfer Complete clear  reset: 0
      uint32_t CHTIF3 : 1; // Channel 3 Half Transfer clear  reset: 0
      uint32_t CTEIF3 : 1; // Channel 3 Transfer Error clear  reset: 0
      uint32_t CGIF4 : 1; // Channel 4 Global interrupt clear  reset: 0
      uint32_t CTCIF4 : 1; // Channel 4 Transfer Complete clear  reset: 0
      uint32_t CHTIF4 : 1; // Channel 4 Half Transfer clear  reset: 0
      uint32_t CTEIF4 : 1; // Channel 4 Transfer Error clear  reset: 0
      uint32_t CGIF5 : 1; // Channel 5 Global interrupt clear  reset: 0
      uint32_t CTCIF5 : 1; // Channel 5 Transfer Complete clear  reset: 0
      uint32_t CHTIF5 : 1; // Channel 5 Half Transfer clear  reset: 0
      uint32_t CTEIF5 : 1; // Channel 5 Transfer Error clear  reset: 0
      uint32_t CGIF6 : 1; // Channel 6 Global interrupt clear  reset: 0
      uint32_t CTCIF6 : 1; // Channel 6 Transfer Complete clear  reset: 0
      uint32_t CHTIF6 : 1; // Channel 6 Half Transfer clear  reset: 0
      uint32_t CTEIF6 : 1; // Channel 6 Transfer Error clear  reset: 0
      uint32_t CGIF7 : 1; // Channel 7 Global interrupt clear  reset: 0
      uint32_t CTCIF7 : 1; // Channel 7 Transfer Complete clear  reset: 0
      uint32_t CHTIF7 : 1; // Channel 7 Half Transfer clear  reset: 0
      uint32_t CTEIF7 : 1; // Channel 7 Transfer Error clear  reset: 0
      uint32_t RESERVED_696 : 4;
    };
  } IFCR;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020008 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_697 : 17;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // DMA channel 1 number of data register @ 0x4002000C read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_698 : 16;
    };
  } CNDTR1;
  union{
    volatile uint32_t w;
    struct { // DMA channel 1 peripheral address register @ 0x40020010 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR1;
  union{
    volatile uint32_t w;
    struct { // DMA channel 1 memory address register @ 0x40020014 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR1;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x4002001C read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_699 : 17;
    };
  } CCR2;
  union{
    volatile uint32_t w;
    struct { // DMA channel 2 number of data register @ 0x40020020 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_700 : 16;
    };
  } CNDTR2;
  union{
    volatile uint32_t w;
    struct { // DMA channel 2 peripheral address register @ 0x40020024 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR2;
  union{
    volatile uint32_t w;
    struct { // DMA channel 2 memory address register @ 0x40020028 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR2;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020030 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_701 : 17;
    };
  } CCR3;
  union{
    volatile uint32_t w;
    struct { // DMA channel 3 number of data register @ 0x40020034 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_702 : 16;
    };
  } CNDTR3;
  union{
    volatile uint32_t w;
    struct { // DMA channel 3 peripheral address register @ 0x40020038 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR3;
  union{
    volatile uint32_t w;
    struct { // DMA channel 3 memory address register @ 0x4002003C read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR3;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020044 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_703 : 17;
    };
  } CCR4;
  union{
    volatile uint32_t w;
    struct { // DMA channel 4 number of data register @ 0x40020048 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_704 : 16;
    };
  } CNDTR4;
  union{
    volatile uint32_t w;
    struct { // DMA channel 4 peripheral address register @ 0x4002004C read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR4;
  union{
    volatile uint32_t w;
    struct { // DMA channel 4 memory address register @ 0x40020050 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR4;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020058 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_705 : 17;
    };
  } CCR5;
  union{
    volatile uint32_t w;
    struct { // DMA channel 5 number of data register @ 0x4002005C read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_706 : 16;
    };
  } CNDTR5;
  union{
    volatile uint32_t w;
    struct { // DMA channel 5 peripheral address register @ 0x40020060 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR5;
  union{
    volatile uint32_t w;
    struct { // DMA channel 5 memory address register @ 0x40020064 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR5;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x4002006C read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_707 : 17;
    };
  } CCR6;
  union{
    volatile uint32_t w;
    struct { // DMA channel 6 number of data register @ 0x40020070 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_708 : 16;
    };
  } CNDTR6;
  union{
    volatile uint32_t w;
    struct { // DMA channel 6 peripheral address register @ 0x40020074 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR6;
  union{
    volatile uint32_t w;
    struct { // DMA channel 6 memory address register @ 0x40020078 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR6;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020080 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_709 : 17;
    };
  } CCR7;
  union{
    volatile uint32_t w;
    struct { // DMA channel 7 number of data register @ 0x40020084 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_710 : 16;
    };
  } CNDTR7;
  union{
    volatile uint32_t w;
    struct { // DMA channel 7 peripheral address register @ 0x40020088 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR7;
  union{
    volatile uint32_t w;
    struct { // DMA channel 7 memory address register @ 0x4002008C read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR7;
} DMA1_t;

// DMA2 register struct @ 0x40020400
typedef struct{
  union{
    volatile uint32_t w;
    struct { // DMA interrupt status register (DMA_ISR) @ 0x40020400 read-only reset: 0x00000000
      uint32_t GIF1 : 1; // Channel 1 Global interrupt flag  reset: 0
      uint32_t TCIF1 : 1; // Channel 1 Transfer Complete flag  reset: 0
      uint32_t HTIF1 : 1; // Channel 1 Half Transfer Complete flag  reset: 0
      uint32_t TEIF1 : 1; // Channel 1 Transfer Error flag  reset: 0
      uint32_t GIF2 : 1; // Channel 2 Global interrupt flag  reset: 0
      uint32_t TCIF2 : 1; // Channel 2 Transfer Complete flag  reset: 0
      uint32_t HTIF2 : 1; // Channel 2 Half Transfer Complete flag  reset: 0
      uint32_t TEIF2 : 1; // Channel 2 Transfer Error flag  reset: 0
      uint32_t GIF3 : 1; // Channel 3 Global interrupt flag  reset: 0
      uint32_t TCIF3 : 1; // Channel 3 Transfer Complete flag  reset: 0
      uint32_t HTIF3 : 1; // Channel 3 Half Transfer Complete flag  reset: 0
      uint32_t TEIF3 : 1; // Channel 3 Transfer Error flag  reset: 0
      uint32_t GIF4 : 1; // Channel 4 Global interrupt flag  reset: 0
      uint32_t TCIF4 : 1; // Channel 4 Transfer Complete flag  reset: 0
      uint32_t HTIF4 : 1; // Channel 4 Half Transfer Complete flag  reset: 0
      uint32_t TEIF4 : 1; // Channel 4 Transfer Error flag  reset: 0
      uint32_t GIF5 : 1; // Channel 5 Global interrupt flag  reset: 0
      uint32_t TCIF5 : 1; // Channel 5 Transfer Complete flag  reset: 0
      uint32_t HTIF5 : 1; // Channel 5 Half Transfer Complete flag  reset: 0
      uint32_t TEIF5 : 1; // Channel 5 Transfer Error flag  reset: 0
      uint32_t GIF6 : 1; // Channel 6 Global interrupt flag  reset: 0
      uint32_t TCIF6 : 1; // Channel 6 Transfer Complete flag  reset: 0
      uint32_t HTIF6 : 1; // Channel 6 Half Transfer Complete flag  reset: 0
      uint32_t TEIF6 : 1; // Channel 6 Transfer Error flag  reset: 0
      uint32_t GIF7 : 1; // Channel 7 Global interrupt flag  reset: 0
      uint32_t TCIF7 : 1; // Channel 7 Transfer Complete flag  reset: 0
      uint32_t HTIF7 : 1; // Channel 7 Half Transfer Complete flag  reset: 0
      uint32_t TEIF7 : 1; // Channel 7 Transfer Error flag  reset: 0
      uint32_t RESERVED_711 : 4;
    };
  } ISR;
  union{
    volatile uint32_t w;
    struct { // DMA interrupt flag clear register (DMA_IFCR) @ 0x40020404 write-only reset: 0x00000000
      uint32_t CGIF1 : 1; // Channel 1 Global interrupt clear  reset: 0
      uint32_t CTCIF1 : 1; // Channel 1 Transfer Complete clear  reset: 0
      uint32_t CHTIF1 : 1; // Channel 1 Half Transfer clear  reset: 0
      uint32_t CTEIF1 : 1; // Channel 1 Transfer Error clear  reset: 0
      uint32_t CGIF2 : 1; // Channel 2 Global interrupt clear  reset: 0
      uint32_t CTCIF2 : 1; // Channel 2 Transfer Complete clear  reset: 0
      uint32_t CHTIF2 : 1; // Channel 2 Half Transfer clear  reset: 0
      uint32_t CTEIF2 : 1; // Channel 2 Transfer Error clear  reset: 0
      uint32_t CGIF3 : 1; // Channel 3 Global interrupt clear  reset: 0
      uint32_t CTCIF3 : 1; // Channel 3 Transfer Complete clear  reset: 0
      uint32_t CHTIF3 : 1; // Channel 3 Half Transfer clear  reset: 0
      uint32_t CTEIF3 : 1; // Channel 3 Transfer Error clear  reset: 0
      uint32_t CGIF4 : 1; // Channel 4 Global interrupt clear  reset: 0
      uint32_t CTCIF4 : 1; // Channel 4 Transfer Complete clear  reset: 0
      uint32_t CHTIF4 : 1; // Channel 4 Half Transfer clear  reset: 0
      uint32_t CTEIF4 : 1; // Channel 4 Transfer Error clear  reset: 0
      uint32_t CGIF5 : 1; // Channel 5 Global interrupt clear  reset: 0
      uint32_t CTCIF5 : 1; // Channel 5 Transfer Complete clear  reset: 0
      uint32_t CHTIF5 : 1; // Channel 5 Half Transfer clear  reset: 0
      uint32_t CTEIF5 : 1; // Channel 5 Transfer Error clear  reset: 0
      uint32_t CGIF6 : 1; // Channel 6 Global interrupt clear  reset: 0
      uint32_t CTCIF6 : 1; // Channel 6 Transfer Complete clear  reset: 0
      uint32_t CHTIF6 : 1; // Channel 6 Half Transfer clear  reset: 0
      uint32_t CTEIF6 : 1; // Channel 6 Transfer Error clear  reset: 0
      uint32_t CGIF7 : 1; // Channel 7 Global interrupt clear  reset: 0
      uint32_t CTCIF7 : 1; // Channel 7 Transfer Complete clear  reset: 0
      uint32_t CHTIF7 : 1; // Channel 7 Half Transfer clear  reset: 0
      uint32_t CTEIF7 : 1; // Channel 7 Transfer Error clear  reset: 0
      uint32_t RESERVED_712 : 4;
    };
  } IFCR;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020408 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_713 : 17;
    };
  } CCR1;
  union{
    volatile uint32_t w;
    struct { // DMA channel 1 number of data register @ 0x4002040C read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_714 : 16;
    };
  } CNDTR1;
  union{
    volatile uint32_t w;
    struct { // DMA channel 1 peripheral address register @ 0x40020410 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR1;
  union{
    volatile uint32_t w;
    struct { // DMA channel 1 memory address register @ 0x40020414 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR1;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x4002041C read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_715 : 17;
    };
  } CCR2;
  union{
    volatile uint32_t w;
    struct { // DMA channel 2 number of data register @ 0x40020420 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_716 : 16;
    };
  } CNDTR2;
  union{
    volatile uint32_t w;
    struct { // DMA channel 2 peripheral address register @ 0x40020424 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR2;
  union{
    volatile uint32_t w;
    struct { // DMA channel 2 memory address register @ 0x40020428 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR2;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020430 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_717 : 17;
    };
  } CCR3;
  union{
    volatile uint32_t w;
    struct { // DMA channel 3 number of data register @ 0x40020434 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_718 : 16;
    };
  } CNDTR3;
  union{
    volatile uint32_t w;
    struct { // DMA channel 3 peripheral address register @ 0x40020438 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR3;
  union{
    volatile uint32_t w;
    struct { // DMA channel 3 memory address register @ 0x4002043C read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR3;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020444 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_719 : 17;
    };
  } CCR4;
  union{
    volatile uint32_t w;
    struct { // DMA channel 4 number of data register @ 0x40020448 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_720 : 16;
    };
  } CNDTR4;
  union{
    volatile uint32_t w;
    struct { // DMA channel 4 peripheral address register @ 0x4002044C read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR4;
  union{
    volatile uint32_t w;
    struct { // DMA channel 4 memory address register @ 0x40020450 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR4;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020458 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_721 : 17;
    };
  } CCR5;
  union{
    volatile uint32_t w;
    struct { // DMA channel 5 number of data register @ 0x4002045C read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_722 : 16;
    };
  } CNDTR5;
  union{
    volatile uint32_t w;
    struct { // DMA channel 5 peripheral address register @ 0x40020460 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR5;
  union{
    volatile uint32_t w;
    struct { // DMA channel 5 memory address register @ 0x40020464 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR5;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x4002046C read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_723 : 17;
    };
  } CCR6;
  union{
    volatile uint32_t w;
    struct { // DMA channel 6 number of data register @ 0x40020470 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_724 : 16;
    };
  } CNDTR6;
  union{
    volatile uint32_t w;
    struct { // DMA channel 6 peripheral address register @ 0x40020474 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR6;
  union{
    volatile uint32_t w;
    struct { // DMA channel 6 memory address register @ 0x40020478 read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR6;
  union{
    volatile uint32_t w;
    struct { // DMA channel configuration register (DMA_CCR) @ 0x40020480 read-write reset: 0x00000000
      uint32_t EN : 1; // Channel enable  reset: 0
      uint32_t TCIE : 1; // Transfer complete interrupt enable  reset: 0
      uint32_t HTIE : 1; // Half Transfer interrupt enable  reset: 0
      uint32_t TEIE : 1; // Transfer error interrupt enable  reset: 0
      uint32_t DIR : 1; // Data transfer direction  reset: 0
      uint32_t CIRC : 1; // Circular mode  reset: 0
      uint32_t PINC : 1; // Peripheral increment mode  reset: 0
      uint32_t MINC : 1; // Memory increment mode  reset: 0
      uint32_t PSIZE : 2; // Peripheral size  reset: 0
      uint32_t MSIZE : 2; // Memory size  reset: 0
      uint32_t PL : 2; // Channel Priority level  reset: 0
      uint32_t MEM2MEM : 1; // Memory to memory mode  reset: 0
      uint32_t RESERVED_725 : 17;
    };
  } CCR7;
  union{
    volatile uint32_t w;
    struct { // DMA channel 7 number of data register @ 0x40020484 read-write reset: 0x00000000
      uint32_t NDT : 16; // Number of data to transfer  reset: 0
      uint32_t RESERVED_726 : 16;
    };
  } CNDTR7;
  union{
    volatile uint32_t w;
    struct { // DMA channel 7 peripheral address register @ 0x40020488 read-write reset: 0x00000000
      uint32_t PA : 32; // Peripheral address  reset: 0
    };
  } CPAR7;
  union{
    volatile uint32_t w;
    struct { // DMA channel 7 memory address register @ 0x4002048C read-write reset: 0x00000000
      uint32_t MA : 32; // Memory address  reset: 0
    };
  } CMAR7;
} DMA2_t;

// RCC register struct @ 0x40021000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Clock control register @ 0x40021000  reset: 0x00000083
      uint32_t HSION : 1; // Internal High Speed clock enable read-write reset: 1
      uint32_t HSIRDY : 1; // Internal High Speed clock ready flag read-only reset: 1
      uint32_t RESERVED_727 : 1;
      uint32_t HSITRIM : 5; // Internal High Speed clock trimming read-write reset: 16
      uint32_t HSICAL : 8; // Internal High Speed clock Calibration read-only reset: 0
      uint32_t HSEON : 1; // External High Speed clock enable read-write reset: 0
      uint32_t HSERDY : 1; // External High Speed clock ready flag read-only reset: 0
      uint32_t HSEBYP : 1; // External High Speed clock Bypass read-write reset: 0
      uint32_t CSSON : 1; // Clock Security System enable read-write reset: 0
      uint32_t RESERVED_728 : 4;
      uint32_t PLLON : 1; // PLL enable read-write reset: 0
      uint32_t PLLRDY : 1; // PLL clock ready flag read-only reset: 0
      uint32_t RESERVED_729 : 6;
    };
  } CR;
  union{
    volatile uint32_t w;
    struct { // Clock configuration register (RCC_CFGR) @ 0x40021004  reset: 0x00000000
      uint32_t SW : 2; // System clock Switch read-write reset: 0
      uint32_t SWS : 2; // System Clock Switch Status read-only reset: 0
      uint32_t HPRE : 4; // AHB prescaler read-write reset: 0
      uint32_t PPRE1 : 3; // APB Low speed prescaler (APB1) read-write reset: 0
      uint32_t PPRE2 : 3; // APB High speed prescaler (APB2) read-write reset: 0
      uint32_t ADCPRE : 2; // ADC prescaler read-write reset: 0
      uint32_t PLLSRC : 1; // PLL entry clock source read-write reset: 0
      uint32_t PLLXTPRE : 1; // HSE divider for PLL entry read-write reset: 0
      uint32_t PLLMUL : 4; // PLL Multiplication Factor read-write reset: 0
      uint32_t OTGFSPRE : 1; // USB OTG FS prescaler read-write reset: 0
      uint32_t RESERVED_730 : 1;
      uint32_t MCO : 3; // Microcontroller clock output read-write reset: 0
      uint32_t RESERVED_731 : 5;
    };
  } CFGR;
  union{
    volatile uint32_t w;
    struct { // Clock interrupt register (RCC_CIR) @ 0x40021008  reset: 0x00000000
      uint32_t LSIRDYF : 1; // LSI Ready Interrupt flag read-only reset: 0
      uint32_t LSERDYF : 1; // LSE Ready Interrupt flag read-only reset: 0
      uint32_t HSIRDYF : 1; // HSI Ready Interrupt flag read-only reset: 0
      uint32_t HSERDYF : 1; // HSE Ready Interrupt flag read-only reset: 0
      uint32_t PLLRDYF : 1; // PLL Ready Interrupt flag read-only reset: 0
      uint32_t RESERVED_732 : 2;
      uint32_t CSSF : 1; // Clock Security System Interrupt flag read-only reset: 0
      uint32_t LSIRDYIE : 1; // LSI Ready Interrupt Enable read-write reset: 0
      uint32_t LSERDYIE : 1; // LSE Ready Interrupt Enable read-write reset: 0
      uint32_t HSIRDYIE : 1; // HSI Ready Interrupt Enable read-write reset: 0
      uint32_t HSERDYIE : 1; // HSE Ready Interrupt Enable read-write reset: 0
      uint32_t PLLRDYIE : 1; // PLL Ready Interrupt Enable read-write reset: 0
      uint32_t RESERVED_733 : 3;
      uint32_t LSIRDYC : 1; // LSI Ready Interrupt Clear write-only reset: 0
      uint32_t LSERDYC : 1; // LSE Ready Interrupt Clear write-only reset: 0
      uint32_t HSIRDYC : 1; // HSI Ready Interrupt Clear write-only reset: 0
      uint32_t HSERDYC : 1; // HSE Ready Interrupt Clear write-only reset: 0
      uint32_t PLLRDYC : 1; // PLL Ready Interrupt Clear write-only reset: 0
      uint32_t RESERVED_734 : 2;
      uint32_t CSSC : 1; // Clock security system interrupt clear write-only reset: 0
      uint32_t RESERVED_735 : 8;
    };
  } CIR;
  union{
    volatile uint32_t w;
    struct { // APB2 peripheral reset register (RCC_APB2RSTR) @ 0x4002100C read-write reset: 0x00000000
      uint32_t AFIORST : 1; // Alternate function I/O reset  reset: 0
      uint32_t RESERVED_736 : 1;
      uint32_t IOPARST : 1; // IO port A reset  reset: 0
      uint32_t IOPBRST : 1; // IO port B reset  reset: 0
      uint32_t IOPCRST : 1; // IO port C reset  reset: 0
      uint32_t IOPDRST : 1; // IO port D reset  reset: 0
      uint32_t IOPERST : 1; // IO port E reset  reset: 0
      uint32_t IOPFRST : 1; // IO port F reset  reset: 0
      uint32_t IOPGRST : 1; // IO port G reset  reset: 0
      uint32_t ADC1RST : 1; // ADC 1 interface reset  reset: 0
      uint32_t ADC2RST : 1; // ADC 2 interface reset  reset: 0
      uint32_t TIM1RST : 1; // TIM1 timer reset  reset: 0
      uint32_t SPI1RST : 1; // SPI 1 reset  reset: 0
      uint32_t TIM8RST : 1; // TIM8 timer reset  reset: 0
      uint32_t USART1RST : 1; // USART1 reset  reset: 0
      uint32_t ADC3RST : 1; // ADC 3 interface reset  reset: 0
      uint32_t RESERVED_737 : 3;
      uint32_t TIM9RST : 1; // TIM9 timer reset  reset: 0
      uint32_t TIM10RST : 1; // TIM10 timer reset  reset: 0
      uint32_t TIM11RST : 1; // TIM11 timer reset  reset: 0
      uint32_t RESERVED_738 : 10;
    };
  } APB2RSTR;
  union{
    volatile uint32_t w;
    struct { // APB1 peripheral reset register (RCC_APB1RSTR) @ 0x40021010 read-write reset: 0x00000000
      uint32_t TIM2RST : 1; // Timer 2 reset  reset: 0
      uint32_t TIM3RST : 1; // Timer 3 reset  reset: 0
      uint32_t TIM4RST : 1; // Timer 4 reset  reset: 0
      uint32_t TIM5RST : 1; // Timer 5 reset  reset: 0
      uint32_t TIM6RST : 1; // Timer 6 reset  reset: 0
      uint32_t TIM7RST : 1; // Timer 7 reset  reset: 0
      uint32_t TIM12RST : 1; // Timer 12 reset  reset: 0
      uint32_t TIM13RST : 1; // Timer 13 reset  reset: 0
      uint32_t TIM14RST : 1; // Timer 14 reset  reset: 0
      uint32_t RESERVED_739 : 2;
      uint32_t WWDGRST : 1; // Window watchdog reset  reset: 0
      uint32_t RESERVED_740 : 2;
      uint32_t SPI2RST : 1; // SPI2 reset  reset: 0
      uint32_t SPI3RST : 1; // SPI3 reset  reset: 0
      uint32_t RESERVED_741 : 1;
      uint32_t USART2RST : 1; // USART 2 reset  reset: 0
      uint32_t USART3RST : 1; // USART 3 reset  reset: 0
      uint32_t UART4RST : 1; // UART 4 reset  reset: 0
      uint32_t UART5RST : 1; // UART 5 reset  reset: 0
      uint32_t I2C1RST : 1; // I2C1 reset  reset: 0
      uint32_t I2C2RST : 1; // I2C2 reset  reset: 0
      uint32_t USBRST : 1; // USB reset  reset: 0
      uint32_t RESERVED_742 : 1;
      uint32_t CANRST : 1; // CAN reset  reset: 0
      uint32_t RESERVED_743 : 1;
      uint32_t BKPRST : 1; // Backup interface reset  reset: 0
      uint32_t PWRRST : 1; // Power interface reset  reset: 0
      uint32_t DACRST : 1; // DAC interface reset  reset: 0
      uint32_t RESERVED_744 : 2;
    };
  } APB1RSTR;
  union{
    volatile uint32_t w;
    struct { // AHB Peripheral Clock enable register (RCC_AHBENR) @ 0x40021014 read-write reset: 0x00000014
      uint32_t DMA1EN : 1; // DMA1 clock enable  reset: 0
      uint32_t DMA2EN : 1; // DMA2 clock enable  reset: 0
      uint32_t SRAMEN : 1; // SRAM interface clock enable  reset: 1
      uint32_t RESERVED_745 : 1;
      uint32_t FLITFEN : 1; // FLITF clock enable  reset: 1
      uint32_t RESERVED_746 : 1;
      uint32_t CRCEN : 1; // CRC clock enable  reset: 0
      uint32_t RESERVED_747 : 1;
      uint32_t FSMCEN : 1; // FSMC clock enable  reset: 0
      uint32_t RESERVED_748 : 1;
      uint32_t SDIOEN : 1; // SDIO clock enable  reset: 0
      uint32_t RESERVED_749 : 21;
    };
  } AHBENR;
  union{
    volatile uint32_t w;
    struct { // APB2 peripheral clock enable register (RCC_APB2ENR) @ 0x40021018 read-write reset: 0x00000000
      uint32_t AFIOEN : 1; // Alternate function I/O clock enable  reset: 0
      uint32_t RESERVED_750 : 1;
      uint32_t IOPAEN : 1; // I/O port A clock enable  reset: 0
      uint32_t IOPBEN : 1; // I/O port B clock enable  reset: 0
      uint32_t IOPCEN : 1; // I/O port C clock enable  reset: 0
      uint32_t IOPDEN : 1; // I/O port D clock enable  reset: 0
      uint32_t IOPEEN : 1; // I/O port E clock enable  reset: 0
      uint32_t IOPFEN : 1; // I/O port F clock enable  reset: 0
      uint32_t IOPGEN : 1; // I/O port G clock enable  reset: 0
      uint32_t ADC1EN : 1; // ADC 1 interface clock enable  reset: 0
      uint32_t ADC2EN : 1; // ADC 2 interface clock enable  reset: 0
      uint32_t TIM1EN : 1; // TIM1 Timer clock enable  reset: 0
      uint32_t SPI1EN : 1; // SPI 1 clock enable  reset: 0
      uint32_t TIM8EN : 1; // TIM8 Timer clock enable  reset: 0
      uint32_t USART1EN : 1; // USART1 clock enable  reset: 0
      uint32_t ADC3EN : 1; // ADC3 interface clock enable  reset: 0
      uint32_t RESERVED_751 : 3;
      uint32_t TIM9EN : 1; // TIM9 Timer clock enable  reset: 0
      uint32_t TIM10EN : 1; // TIM10 Timer clock enable  reset: 0
      uint32_t TIM11EN : 1; // TIM11 Timer clock enable  reset: 0
      uint32_t RESERVED_752 : 10;
    };
  } APB2ENR;
  union{
    volatile uint32_t w;
    struct { // APB1 peripheral clock enable register (RCC_APB1ENR) @ 0x4002101C read-write reset: 0x00000000
      uint32_t TIM2EN : 1; // Timer 2 clock enable  reset: 0
      uint32_t TIM3EN : 1; // Timer 3 clock enable  reset: 0
      uint32_t TIM4EN : 1; // Timer 4 clock enable  reset: 0
      uint32_t TIM5EN : 1; // Timer 5 clock enable  reset: 0
      uint32_t TIM6EN : 1; // Timer 6 clock enable  reset: 0
      uint32_t TIM7EN : 1; // Timer 7 clock enable  reset: 0
      uint32_t TIM12EN : 1; // Timer 12 clock enable  reset: 0
      uint32_t TIM13EN : 1; // Timer 13 clock enable  reset: 0
      uint32_t TIM14EN : 1; // Timer 14 clock enable  reset: 0
      uint32_t RESERVED_753 : 2;
      uint32_t WWDGEN : 1; // Window watchdog clock enable  reset: 0
      uint32_t RESERVED_754 : 2;
      uint32_t SPI2EN : 1; // SPI 2 clock enable  reset: 0
      uint32_t SPI3EN : 1; // SPI 3 clock enable  reset: 0
      uint32_t RESERVED_755 : 1;
      uint32_t USART2EN : 1; // USART 2 clock enable  reset: 0
      uint32_t USART3EN : 1; // USART 3 clock enable  reset: 0
      uint32_t UART4EN : 1; // UART 4 clock enable  reset: 0
      uint32_t UART5EN : 1; // UART 5 clock enable  reset: 0
      uint32_t I2C1EN : 1; // I2C 1 clock enable  reset: 0
      uint32_t I2C2EN : 1; // I2C 2 clock enable  reset: 0
      uint32_t USBEN : 1; // USB clock enable  reset: 0
      uint32_t RESERVED_756 : 1;
      uint32_t CANEN : 1; // CAN clock enable  reset: 0
      uint32_t RESERVED_757 : 1;
      uint32_t BKPEN : 1; // Backup interface clock enable  reset: 0
      uint32_t PWREN : 1; // Power interface clock enable  reset: 0
      uint32_t DACEN : 1; // DAC interface clock enable  reset: 0
      uint32_t RESERVED_758 : 2;
    };
  } APB1ENR;
  union{
    volatile uint32_t w;
    struct { // Backup domain control register (RCC_BDCR) @ 0x40021020  reset: 0x00000000
      uint32_t LSEON : 1; // External Low Speed oscillator enable read-write reset: 0
      uint32_t LSERDY : 1; // External Low Speed oscillator ready read-only reset: 0
      uint32_t LSEBYP : 1; // External Low Speed oscillator bypass read-write reset: 0
      uint32_t RESERVED_759 : 5;
      uint32_t RTCSEL : 2; // RTC clock source selection read-write reset: 0
      uint32_t RESERVED_760 : 5;
      uint32_t RTCEN : 1; // RTC clock enable read-write reset: 0
      uint32_t BDRST : 1; // Backup domain software reset read-write reset: 0
      uint32_t RESERVED_761 : 15;
    };
  } BDCR;
  union{
    volatile uint32_t w;
    struct { // Control/status register (RCC_CSR) @ 0x40021024  reset: 0x0C000000
      uint32_t LSION : 1; // Internal low speed oscillator enable read-write reset: 0
      uint32_t LSIRDY : 1; // Internal low speed oscillator ready read-only reset: 0
      uint32_t RESERVED_762 : 22;
      uint32_t RMVF : 1; // Remove reset flag read-write reset: 0
      uint32_t RESERVED_763 : 1;
      uint32_t PINRSTF : 1; // PIN reset flag read-write reset: 1
      uint32_t PORRSTF : 1; // POR/PDR reset flag read-write reset: 1
      uint32_t SFTRSTF : 1; // Software reset flag read-write reset: 0
      uint32_t IWDGRSTF : 1; // Independent watchdog reset flag read-write reset: 0
      uint32_t WWDGRSTF : 1; // Window watchdog reset flag read-write reset: 0
      uint32_t LPWRRSTF : 1; // Low-power reset flag read-write reset: 0
    };
  } CSR;
} RCC_t;

// FLASH register struct @ 0x40022000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Flash access control register @ 0x40022000  reset: 0x00000030
      uint32_t LATENCY : 3; // Latency read-write reset: 0
      uint32_t HLFCYA : 1; // Flash half cycle access enable read-write reset: 0
      uint32_t PRFTBE : 1; // Prefetch buffer enable read-write reset: 1
      uint32_t PRFTBS : 1; // Prefetch buffer status read-only reset: 1
      uint32_t RESERVED_764 : 26;
    };
  } ACR;
  union{
    volatile uint32_t w;
    struct { // Flash key register @ 0x40022004 write-only reset: 0x00000000
      uint32_t KEY : 32; // FPEC key  reset: 0
    };
  } KEYR;
  union{
    volatile uint32_t w;
    struct { // Flash option key register @ 0x40022008 write-only reset: 0x00000000
      uint32_t OPTKEY : 32; // Option byte key  reset: 0
    };
  } OPTKEYR;
  union{
    volatile uint32_t w;
    struct { // Status register @ 0x4002200C  reset: 0x00000000
      uint32_t BSY : 1; // Busy read-only reset: 0
      uint32_t RESERVED_765 : 1;
      uint32_t PGERR : 1; // Programming error read-write reset: 0
      uint32_t RESERVED_766 : 1;
      uint32_t WRPRTERR : 1; // Write protection error read-write reset: 0
      uint32_t EOP : 1; // End of operation read-write reset: 0
      uint32_t RESERVED_767 : 26;
    };
  } SR;
  union{
    volatile uint32_t w;
    struct { // Control register @ 0x40022010 read-write reset: 0x00000080
      uint32_t PG : 1; // Programming  reset: 0
      uint32_t PER : 1; // Page Erase  reset: 0
      uint32_t MER : 1; // Mass Erase  reset: 0
      uint32_t RESERVED_768 : 1;
      uint32_t OPTPG : 1; // Option byte programming  reset: 0
      uint32_t OPTER : 1; // Option byte erase  reset: 0
      uint32_t STRT : 1; // Start  reset: 0
      uint32_t LOCK : 1; // Lock  reset: 1
      uint32_t RESERVED_769 : 1;
      uint32_t OPTWRE : 1; // Option bytes write enable  reset: 0
      uint32_t ERRIE : 1; // Error interrupt enable  reset: 0
      uint32_t RESERVED_770 : 1;
      uint32_t EOPIE : 1; // End of operation interrupt enable  reset: 0
      uint32_t RESERVED_771 : 19;
    };
  } CR;
  union{
    volatile uint32_t w;
    struct { // Flash address register @ 0x40022014 write-only reset: 0x00000000
      uint32_t FAR : 32; // Flash Address  reset: 0
    };
  } AR;
  union{
    volatile uint32_t w;
    struct { // Option byte register @ 0x4002201C read-only reset: 0x03FFFFFC
      uint32_t OPTERR : 1; // Option byte error  reset: 0
      uint32_t RDPRT : 1; // Read protection  reset: 0
      uint32_t WDG_SW : 1; // WDG_SW  reset: 1
      uint32_t nRST_STOP : 1; // nRST_STOP  reset: 1
      uint32_t nRST_STDBY : 1; // nRST_STDBY  reset: 1
      uint32_t RESERVED_772 : 5;
      uint32_t Data0 : 8; // Data0  reset: 255
      uint32_t Data1 : 8; // Data1  reset: 255
      uint32_t RESERVED_773 : 6;
    };
  } OBR;
  union{
    volatile uint32_t w;
    struct { // Write protection register @ 0x40022020 read-only reset: 0xFFFFFFFF
      uint32_t WRP : 32; // Write protect  reset: 4294967295
    };
  } WRPR;
} FLASH_t;

// CRC register struct @ 0x40023000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // Data register @ 0x40023000 read-write reset: 0xFFFFFFFF
      uint32_t DR : 32; // Data Register  reset: 4294967295
    };
  } DR;
  union{
    volatile uint32_t w;
    struct { // Independent Data register @ 0x40023004 read-write reset: 0x00000000
      uint32_t IDR : 8; // Independent Data register  reset: 0
      uint32_t RESERVED_774 : 24;
    };
  } IDR;
  union{
    volatile uint32_t w;
    struct { // Control register @ 0x40023008 write-only reset: 0x00000000
      uint32_t RESET : 1; // Reset bit  reset: 0
      uint32_t RESERVED_775 : 31;
    };
  } CR;
} CRC_t;

// FSMC register struct @ 0xA0000000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash chip-select control register 1 @ 0xA0000000 read-write reset: 0x000030D0
      uint32_t MBKEN : 1; // MBKEN  reset: 0
      uint32_t MUXEN : 1; // MUXEN  reset: 0
      uint32_t MTYP : 2; // MTYP  reset: 0
      uint32_t MWID : 2; // MWID  reset: 1
      uint32_t FACCEN : 1; // FACCEN  reset: 1
      uint32_t RESERVED_776 : 1;
      uint32_t BURSTEN : 1; // BURSTEN  reset: 0
      uint32_t WAITPOL : 1; // WAITPOL  reset: 0
      uint32_t RESERVED_777 : 1;
      uint32_t WAITCFG : 1; // WAITCFG  reset: 0
      uint32_t WREN : 1; // WREN  reset: 1
      uint32_t WAITEN : 1; // WAITEN  reset: 1
      uint32_t EXTMOD : 1; // EXTMOD  reset: 0
      uint32_t ASYNCWAIT : 1; // ASYNCWAIT  reset: 0
      uint32_t RESERVED_778 : 3;
      uint32_t CBURSTRW : 1; // CBURSTRW  reset: 0
      uint32_t RESERVED_779 : 12;
    };
  } BCR1;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash chip-select timing register 1 @ 0xA0000004 read-write reset: 0xFFFFFFFF
      uint32_t ADDSET : 4; // ADDSET  reset: 15
      uint32_t ADDHLD : 4; // ADDHLD  reset: 15
      uint32_t DATAST : 8; // DATAST  reset: 255
      uint32_t BUSTURN : 4; // BUSTURN  reset: 15
      uint32_t CLKDIV : 4; // CLKDIV  reset: 15
      uint32_t DATLAT : 4; // DATLAT  reset: 15
      uint32_t ACCMOD : 2; // ACCMOD  reset: 3
      uint32_t RESERVED_780 : 2;
    };
  } BTR1;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash chip-select control register 2 @ 0xA0000008 read-write reset: 0x000030D0
      uint32_t MBKEN : 1; // MBKEN  reset: 0
      uint32_t MUXEN : 1; // MUXEN  reset: 0
      uint32_t MTYP : 2; // MTYP  reset: 0
      uint32_t MWID : 2; // MWID  reset: 1
      uint32_t FACCEN : 1; // FACCEN  reset: 1
      uint32_t RESERVED_781 : 1;
      uint32_t BURSTEN : 1; // BURSTEN  reset: 0
      uint32_t WAITPOL : 1; // WAITPOL  reset: 0
      uint32_t WRAPMOD : 1; // WRAPMOD  reset: 0
      uint32_t WAITCFG : 1; // WAITCFG  reset: 0
      uint32_t WREN : 1; // WREN  reset: 1
      uint32_t WAITEN : 1; // WAITEN  reset: 1
      uint32_t EXTMOD : 1; // EXTMOD  reset: 0
      uint32_t ASYNCWAIT : 1; // ASYNCWAIT  reset: 0
      uint32_t RESERVED_782 : 3;
      uint32_t CBURSTRW : 1; // CBURSTRW  reset: 0
      uint32_t RESERVED_783 : 12;
    };
  } BCR2;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash chip-select timing register 2 @ 0xA000000C read-write reset: 0xFFFFFFFF
      uint32_t ADDSET : 4; // ADDSET  reset: 15
      uint32_t ADDHLD : 4; // ADDHLD  reset: 15
      uint32_t DATAST : 8; // DATAST  reset: 255
      uint32_t BUSTURN : 4; // BUSTURN  reset: 15
      uint32_t CLKDIV : 4; // CLKDIV  reset: 15
      uint32_t DATLAT : 4; // DATLAT  reset: 15
      uint32_t ACCMOD : 2; // ACCMOD  reset: 3
      uint32_t RESERVED_784 : 2;
    };
  } BTR2;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash chip-select control register 3 @ 0xA0000010 read-write reset: 0x000030D0
      uint32_t MBKEN : 1; // MBKEN  reset: 0
      uint32_t MUXEN : 1; // MUXEN  reset: 0
      uint32_t MTYP : 2; // MTYP  reset: 0
      uint32_t MWID : 2; // MWID  reset: 1
      uint32_t FACCEN : 1; // FACCEN  reset: 1
      uint32_t RESERVED_785 : 1;
      uint32_t BURSTEN : 1; // BURSTEN  reset: 0
      uint32_t WAITPOL : 1; // WAITPOL  reset: 0
      uint32_t WRAPMOD : 1; // WRAPMOD  reset: 0
      uint32_t WAITCFG : 1; // WAITCFG  reset: 0
      uint32_t WREN : 1; // WREN  reset: 1
      uint32_t WAITEN : 1; // WAITEN  reset: 1
      uint32_t EXTMOD : 1; // EXTMOD  reset: 0
      uint32_t ASYNCWAIT : 1; // ASYNCWAIT  reset: 0
      uint32_t RESERVED_786 : 3;
      uint32_t CBURSTRW : 1; // CBURSTRW  reset: 0
      uint32_t RESERVED_787 : 12;
    };
  } BCR3;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash chip-select timing register 3 @ 0xA0000014 read-write reset: 0xFFFFFFFF
      uint32_t ADDSET : 4; // ADDSET  reset: 15
      uint32_t ADDHLD : 4; // ADDHLD  reset: 15
      uint32_t DATAST : 8; // DATAST  reset: 255
      uint32_t BUSTURN : 4; // BUSTURN  reset: 15
      uint32_t CLKDIV : 4; // CLKDIV  reset: 15
      uint32_t DATLAT : 4; // DATLAT  reset: 15
      uint32_t ACCMOD : 2; // ACCMOD  reset: 3
      uint32_t RESERVED_788 : 2;
    };
  } BTR3;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash chip-select control register 4 @ 0xA0000018 read-write reset: 0x000030D0
      uint32_t MBKEN : 1; // MBKEN  reset: 0
      uint32_t MUXEN : 1; // MUXEN  reset: 0
      uint32_t MTYP : 2; // MTYP  reset: 0
      uint32_t MWID : 2; // MWID  reset: 1
      uint32_t FACCEN : 1; // FACCEN  reset: 1
      uint32_t RESERVED_789 : 1;
      uint32_t BURSTEN : 1; // BURSTEN  reset: 0
      uint32_t WAITPOL : 1; // WAITPOL  reset: 0
      uint32_t WRAPMOD : 1; // WRAPMOD  reset: 0
      uint32_t WAITCFG : 1; // WAITCFG  reset: 0
      uint32_t WREN : 1; // WREN  reset: 1
      uint32_t WAITEN : 1; // WAITEN  reset: 1
      uint32_t EXTMOD : 1; // EXTMOD  reset: 0
      uint32_t ASYNCWAIT : 1; // ASYNCWAIT  reset: 0
      uint32_t RESERVED_790 : 3;
      uint32_t CBURSTRW : 1; // CBURSTRW  reset: 0
      uint32_t RESERVED_791 : 12;
    };
  } BCR4;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash chip-select timing register 4 @ 0xA000001C read-write reset: 0xFFFFFFFF
      uint32_t ADDSET : 4; // ADDSET  reset: 15
      uint32_t ADDHLD : 4; // ADDHLD  reset: 15
      uint32_t DATAST : 8; // DATAST  reset: 255
      uint32_t BUSTURN : 4; // BUSTURN  reset: 15
      uint32_t CLKDIV : 4; // CLKDIV  reset: 15
      uint32_t DATLAT : 4; // DATLAT  reset: 15
      uint32_t ACCMOD : 2; // ACCMOD  reset: 3
      uint32_t RESERVED_792 : 2;
    };
  } BTR4;
  const uint8_t RESERVED_793[36];
  union{
    volatile uint32_t w;
    struct { // PC Card/NAND Flash control register 2 @ 0xA0000060 read-write reset: 0x00000018
      uint32_t RESERVED_794 : 1;
      uint32_t PWAITEN : 1; // PWAITEN  reset: 0
      uint32_t PBKEN : 1; // PBKEN  reset: 0
      uint32_t PTYP : 1; // PTYP  reset: 1
      uint32_t PWID : 2; // PWID  reset: 1
      uint32_t ECCEN : 1; // ECCEN  reset: 0
      uint32_t RESERVED_795 : 2;
      uint32_t TCLR : 4; // TCLR  reset: 0
      uint32_t TAR : 4; // TAR  reset: 0
      uint32_t ECCPS : 3; // ECCPS  reset: 0
      uint32_t RESERVED_796 : 12;
    };
  } PCR2;
  union{
    volatile uint32_t w;
    struct { // FIFO status and interrupt register 2 @ 0xA0000064  reset: 0x00000040
      uint32_t IRS : 1; // IRS read-write reset: 0
      uint32_t ILS : 1; // ILS read-write reset: 0
      uint32_t IFS : 1; // IFS read-write reset: 0
      uint32_t IREN : 1; // IREN read-write reset: 0
      uint32_t ILEN : 1; // ILEN read-write reset: 0
      uint32_t IFEN : 1; // IFEN read-write reset: 0
      uint32_t FEMPT : 1; // FEMPT read-only reset: 1
      uint32_t RESERVED_797 : 25;
    };
  } SR2;
  union{
    volatile uint32_t w;
    struct { // Common memory space timing register 2 @ 0xA0000068 read-write reset: 0xFCFCFCFC
      uint32_t MEMSETx : 8; // MEMSETx  reset: 252
      uint32_t MEMWAITx : 8; // MEMWAITx  reset: 252
      uint32_t MEMHOLDx : 8; // MEMHOLDx  reset: 252
      uint32_t MEMHIZx : 8; // MEMHIZx  reset: 252
    };
  } PMEM2;
  union{
    volatile uint32_t w;
    struct { // Attribute memory space timing register 2 @ 0xA000006C read-write reset: 0xFCFCFCFC
      uint32_t ATTSETx : 8; // Attribute memory x setup time  reset: 252
      uint32_t ATTWAITx : 8; // Attribute memory x wait time  reset: 252
      uint32_t ATTHOLDx : 8; // Attribute memory x hold time  reset: 252
      uint32_t ATTHIZx : 8; // Attribute memory x databus HiZ time  reset: 252
    };
  } PATT2;
  union{
    volatile uint32_t w;
    struct { // ECC result register 2 @ 0xA0000074 read-only reset: 0x00000000
      uint32_t ECCx : 32; // ECC result  reset: 0
    };
  } ECCR2;
  union{
    volatile uint32_t w;
    struct { // PC Card/NAND Flash control register 3 @ 0xA0000080 read-write reset: 0x00000018
      uint32_t RESERVED_798 : 1;
      uint32_t PWAITEN : 1; // PWAITEN  reset: 0
      uint32_t PBKEN : 1; // PBKEN  reset: 0
      uint32_t PTYP : 1; // PTYP  reset: 1
      uint32_t PWID : 2; // PWID  reset: 1
      uint32_t ECCEN : 1; // ECCEN  reset: 0
      uint32_t RESERVED_799 : 2;
      uint32_t TCLR : 4; // TCLR  reset: 0
      uint32_t TAR : 4; // TAR  reset: 0
      uint32_t ECCPS : 3; // ECCPS  reset: 0
      uint32_t RESERVED_800 : 12;
    };
  } PCR3;
  union{
    volatile uint32_t w;
    struct { // FIFO status and interrupt register 3 @ 0xA0000084  reset: 0x00000040
      uint32_t IRS : 1; // IRS read-write reset: 0
      uint32_t ILS : 1; // ILS read-write reset: 0
      uint32_t IFS : 1; // IFS read-write reset: 0
      uint32_t IREN : 1; // IREN read-write reset: 0
      uint32_t ILEN : 1; // ILEN read-write reset: 0
      uint32_t IFEN : 1; // IFEN read-write reset: 0
      uint32_t FEMPT : 1; // FEMPT read-only reset: 1
      uint32_t RESERVED_801 : 25;
    };
  } SR3;
  union{
    volatile uint32_t w;
    struct { // Common memory space timing register 3 @ 0xA0000088 read-write reset: 0xFCFCFCFC
      uint32_t MEMSETx : 8; // MEMSETx  reset: 252
      uint32_t MEMWAITx : 8; // MEMWAITx  reset: 252
      uint32_t MEMHOLDx : 8; // MEMHOLDx  reset: 252
      uint32_t MEMHIZx : 8; // MEMHIZx  reset: 252
    };
  } PMEM3;
  union{
    volatile uint32_t w;
    struct { // Attribute memory space timing register 3 @ 0xA000008C read-write reset: 0xFCFCFCFC
      uint32_t ATTSETx : 8; // ATTSETx  reset: 252
      uint32_t ATTWAITx : 8; // ATTWAITx  reset: 252
      uint32_t ATTHOLDx : 8; // ATTHOLDx  reset: 252
      uint32_t ATTHIZx : 8; // ATTHIZx  reset: 252
    };
  } PATT3;
  union{
    volatile uint32_t w;
    struct { // ECC result register 3 @ 0xA0000094 read-only reset: 0x00000000
      uint32_t ECCx : 32; // ECCx  reset: 0
    };
  } ECCR3;
  union{
    volatile uint32_t w;
    struct { // PC Card/NAND Flash control register 4 @ 0xA00000A0 read-write reset: 0x00000018
      uint32_t RESERVED_802 : 1;
      uint32_t PWAITEN : 1; // PWAITEN  reset: 0
      uint32_t PBKEN : 1; // PBKEN  reset: 0
      uint32_t PTYP : 1; // PTYP  reset: 1
      uint32_t PWID : 2; // PWID  reset: 1
      uint32_t ECCEN : 1; // ECCEN  reset: 0
      uint32_t RESERVED_803 : 2;
      uint32_t TCLR : 4; // TCLR  reset: 0
      uint32_t TAR : 4; // TAR  reset: 0
      uint32_t ECCPS : 3; // ECCPS  reset: 0
      uint32_t RESERVED_804 : 12;
    };
  } PCR4;
  union{
    volatile uint32_t w;
    struct { // FIFO status and interrupt register 4 @ 0xA00000A4  reset: 0x00000040
      uint32_t IRS : 1; // IRS read-write reset: 0
      uint32_t ILS : 1; // ILS read-write reset: 0
      uint32_t IFS : 1; // IFS read-write reset: 0
      uint32_t IREN : 1; // IREN read-write reset: 0
      uint32_t ILEN : 1; // ILEN read-write reset: 0
      uint32_t IFEN : 1; // IFEN read-write reset: 0
      uint32_t FEMPT : 1; // FEMPT read-only reset: 1
      uint32_t RESERVED_805 : 25;
    };
  } SR4;
  union{
    volatile uint32_t w;
    struct { // Common memory space timing register 4 @ 0xA00000A8 read-write reset: 0xFCFCFCFC
      uint32_t MEMSETx : 8; // MEMSETx  reset: 252
      uint32_t MEMWAITx : 8; // MEMWAITx  reset: 252
      uint32_t MEMHOLDx : 8; // MEMHOLDx  reset: 252
      uint32_t MEMHIZx : 8; // MEMHIZx  reset: 252
    };
  } PMEM4;
  union{
    volatile uint32_t w;
    struct { // Attribute memory space timing register 4 @ 0xA00000AC read-write reset: 0xFCFCFCFC
      uint32_t ATTSETx : 8; // ATTSETx  reset: 252
      uint32_t ATTWAITx : 8; // ATTWAITx  reset: 252
      uint32_t ATTHOLDx : 8; // ATTHOLDx  reset: 252
      uint32_t ATTHIZx : 8; // ATTHIZx  reset: 252
    };
  } PATT4;
  union{
    volatile uint32_t w;
    struct { // I/O space timing register 4 @ 0xA00000B0 read-write reset: 0xFCFCFCFC
      uint32_t IOSETx : 8; // IOSETx  reset: 252
      uint32_t IOWAITx : 8; // IOWAITx  reset: 252
      uint32_t IOHOLDx : 8; // IOHOLDx  reset: 252
      uint32_t IOHIZx : 8; // IOHIZx  reset: 252
    };
  } PIO4;
  const uint8_t RESERVED_806[52];
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash write timing registers 1 @ 0xA0000104 read-write reset: 0x0FFFFFFF
      uint32_t ADDSET : 4; // ADDSET  reset: 15
      uint32_t ADDHLD : 4; // ADDHLD  reset: 15
      uint32_t DATAST : 8; // DATAST  reset: 255
      uint32_t RESERVED_807 : 4;
      uint32_t CLKDIV : 4; // CLKDIV  reset: 15
      uint32_t DATLAT : 4; // DATLAT  reset: 15
      uint32_t ACCMOD : 2; // ACCMOD  reset: 0
      uint32_t RESERVED_808 : 2;
    };
  } BWTR1;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash write timing registers 2 @ 0xA000010C read-write reset: 0x0FFFFFFF
      uint32_t ADDSET : 4; // ADDSET  reset: 15
      uint32_t ADDHLD : 4; // ADDHLD  reset: 15
      uint32_t DATAST : 8; // DATAST  reset: 255
      uint32_t RESERVED_809 : 4;
      uint32_t CLKDIV : 4; // CLKDIV  reset: 15
      uint32_t DATLAT : 4; // DATLAT  reset: 15
      uint32_t ACCMOD : 2; // ACCMOD  reset: 0
      uint32_t RESERVED_810 : 2;
    };
  } BWTR2;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash write timing registers 3 @ 0xA0000114 read-write reset: 0x0FFFFFFF
      uint32_t ADDSET : 4; // ADDSET  reset: 15
      uint32_t ADDHLD : 4; // ADDHLD  reset: 15
      uint32_t DATAST : 8; // DATAST  reset: 255
      uint32_t RESERVED_811 : 4;
      uint32_t CLKDIV : 4; // CLKDIV  reset: 15
      uint32_t DATLAT : 4; // DATLAT  reset: 15
      uint32_t ACCMOD : 2; // ACCMOD  reset: 0
      uint32_t RESERVED_812 : 2;
    };
  } BWTR3;
  union{
    volatile uint32_t w;
    struct { // SRAM/NOR-Flash write timing registers 4 @ 0xA000011C read-write reset: 0x0FFFFFFF
      uint32_t ADDSET : 4; // ADDSET  reset: 15
      uint32_t ADDHLD : 4; // ADDHLD  reset: 15
      uint32_t DATAST : 8; // DATAST  reset: 255
      uint32_t RESERVED_813 : 4;
      uint32_t CLKDIV : 4; // CLKDIV  reset: 15
      uint32_t DATLAT : 4; // DATLAT  reset: 15
      uint32_t ACCMOD : 2; // ACCMOD  reset: 0
      uint32_t RESERVED_814 : 2;
    };
  } BWTR4;
} FSMC_t;

// NVIC register struct @ 0xE000E000
typedef struct{
  const uint8_t RESERVED_815[4];
  union{
    volatile uint32_t w;
    struct { // Interrupt Controller Type Register @ 0xE000E004 read-only reset: 0x00000000
      uint32_t INTLINESNUM : 4; // Total number of interrupt lines in groups  reset: 0
      uint32_t RESERVED_816 : 28;
    };
  } ICTR;
  const uint8_t RESERVED_817[220];
  union{
    volatile uint32_t w;
    struct { // Interrupt Set-Enable Register @ 0xE000E100 read-write reset: 0x00000000
      uint32_t SETENA : 32; // SETENA  reset: 0
    };
  } ISER0;
  union{
    volatile uint32_t w;
    struct { // Interrupt Set-Enable Register @ 0xE000E104 read-write reset: 0x00000000
      uint32_t SETENA : 32; // SETENA  reset: 0
    };
  } ISER1;
  const uint8_t RESERVED_818[92];
  union{
    volatile uint32_t w;
    struct { // Interrupt Clear-Enable Register @ 0xE000E180 read-write reset: 0x00000000
      uint32_t CLRENA : 32; // CLRENA  reset: 0
    };
  } ICER0;
  union{
    volatile uint32_t w;
    struct { // Interrupt Clear-Enable Register @ 0xE000E184 read-write reset: 0x00000000
      uint32_t CLRENA : 32; // CLRENA  reset: 0
    };
  } ICER1;
  const uint8_t RESERVED_819[92];
  union{
    volatile uint32_t w;
    struct { // Interrupt Set-Pending Register @ 0xE000E200 read-write reset: 0x00000000
      uint32_t SETPEND : 32; // SETPEND  reset: 0
    };
  } ISPR0;
  union{
    volatile uint32_t w;
    struct { // Interrupt Set-Pending Register @ 0xE000E204 read-write reset: 0x00000000
      uint32_t SETPEND : 32; // SETPEND  reset: 0
    };
  } ISPR1;
  const uint8_t RESERVED_820[92];
  union{
    volatile uint32_t w;
    struct { // Interrupt Clear-Pending Register @ 0xE000E280 read-write reset: 0x00000000
      uint32_t CLRPEND : 32; // CLRPEND  reset: 0
    };
  } ICPR0;
  union{
    volatile uint32_t w;
    struct { // Interrupt Clear-Pending Register @ 0xE000E284 read-write reset: 0x00000000
      uint32_t CLRPEND : 32; // CLRPEND  reset: 0
    };
  } ICPR1;
  const uint8_t RESERVED_821[92];
  union{
    volatile uint32_t w;
    struct { // Interrupt Active Bit Register @ 0xE000E300 read-only reset: 0x00000000
      uint32_t ACTIVE : 32; // ACTIVE  reset: 0
    };
  } IABR0;
  union{
    volatile uint32_t w;
    struct { // Interrupt Active Bit Register @ 0xE000E304 read-only reset: 0x00000000
      uint32_t ACTIVE : 32; // ACTIVE  reset: 0
    };
  } IABR1;
  const uint8_t RESERVED_822[220];
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E400 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR0;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E404 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR1;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E408 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR2;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E40C read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR3;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E410 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR4;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E414 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR5;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E418 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR6;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E41C read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR7;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E420 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR8;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E424 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR9;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E428 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR10;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E42C read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR11;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E430 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR12;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E434 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR13;
  union{
    volatile uint32_t w;
    struct { // Interrupt Priority Register @ 0xE000E438 read-write reset: 0x00000000
      uint32_t IPR_N0 : 8; // IPR_N0  reset: 0
      uint32_t IPR_N1 : 8; // IPR_N1  reset: 0
      uint32_t IPR_N2 : 8; // IPR_N2  reset: 0
      uint32_t IPR_N3 : 8; // IPR_N3  reset: 0
    };
  } IPR14;
  const uint8_t RESERVED_823[2728];
  union{
    volatile uint32_t w;
    struct { // Software Triggered Interrupt Register @ 0xE000EF00 write-only reset: 0x00000000
      uint32_t INTID : 9; // interrupt to be triggered  reset: 0
      uint32_t RESERVED_824 : 23;
    };
  } STIR;
} NVIC_t;

// DBG register struct @ 0xE0042000
typedef struct{
  union{
    volatile uint32_t w;
    struct { // DBGMCU_IDCODE @ 0xE0042000 read-only reset: 0x00000000
      uint32_t DEV_ID : 12; // DEV_ID  reset: 0
      uint32_t RESERVED_825 : 4;
      uint32_t REV_ID : 16; // REV_ID  reset: 0
    };
  } IDCODE;
  union{
    volatile uint32_t w;
    struct { // DBGMCU_CR @ 0xE0042004 read-write reset: 0x00000000
      uint32_t DBG_SLEEP : 1; // DBG_SLEEP  reset: 0
      uint32_t DBG_STOP : 1; // DBG_STOP  reset: 0
      uint32_t DBG_STANDBY : 1; // DBG_STANDBY  reset: 0
      uint32_t RESERVED_826 : 2;
      uint32_t TRACE_IOEN : 1; // TRACE_IOEN  reset: 0
      uint32_t TRACE_MODE : 2; // TRACE_MODE  reset: 0
      uint32_t DBG_IWDG_STOP : 1; // DBG_IWDG_STOP  reset: 0
      uint32_t DBG_WWDG_STOP : 1; // DBG_WWDG_STOP  reset: 0
      uint32_t DBG_TIM1_STOP : 1; // DBG_TIM1_STOP  reset: 0
      uint32_t DBG_TIM2_STOP : 1; // DBG_TIM2_STOP  reset: 0
      uint32_t DBG_TIM3_STOP : 1; // DBG_TIM3_STOP  reset: 0
      uint32_t DBG_TIM4_STOP : 1; // DBG_TIM4_STOP  reset: 0
      uint32_t DBG_CAN1_STOP : 1; // DBG_CAN1_STOP  reset: 0
      uint32_t DBG_I2C1_SMBUS_TIMEOUT : 1; // DBG_I2C1_SMBUS_TIMEOUT  reset: 0
      uint32_t DBG_I2C2_SMBUS_TIMEOUT : 1; // DBG_I2C2_SMBUS_TIMEOUT  reset: 0
      uint32_t DBG_TIM8_STOP : 1; // DBG_TIM8_STOP  reset: 0
      uint32_t DBG_TIM5_STOP : 1; // DBG_TIM5_STOP  reset: 0
      uint32_t DBG_TIM6_STOP : 1; // DBG_TIM6_STOP  reset: 0
      uint32_t DBG_TIM7_STOP : 1; // DBG_TIM7_STOP  reset: 0
      uint32_t DBG_CAN2_STOP : 1; // DBG_CAN2_STOP  reset: 0
      uint32_t RESERVED_827 : 10;
    };
  } CR;
} DBG_t;

#define TIM2 ((volatile TIM2_t *) 0x40000000)
#define TIM3 ((volatile TIM3_t *) 0x40000400)
#define TIM4 ((volatile TIM4_t *) 0x40000800)
#define TIM5 ((volatile TIM5_t *) 0x40000C00)
#define TIM6 ((volatile TIM6_t *) 0x40001000)
#define TIM7 ((volatile TIM7_t *) 0x40001400)
#define TIM12 ((volatile TIM12_t *) 0x40001800)
#define TIM13 ((volatile TIM13_t *) 0x40001C00)
#define TIM14 ((volatile TIM14_t *) 0x40002000)
#define RTC ((volatile RTC_t *) 0x40002800)
#define WWDG ((volatile WWDG_t *) 0x40002C00)
#define IWDG ((volatile IWDG_t *) 0x40003000)
#define SPI2 ((volatile SPI2_t *) 0x40003800)
#define SPI3 ((volatile SPI3_t *) 0x40003C00)
#define USART2 ((volatile USART2_t *) 0x40004400)
#define USART3 ((volatile USART3_t *) 0x40004800)
#define UART4 ((volatile UART4_t *) 0x40004C00)
#define UART5 ((volatile UART5_t *) 0x40005000)
#define I2C1 ((volatile I2C1_t *) 0x40005400)
#define I2C2 ((volatile I2C2_t *) 0x40005800)
#define USB ((volatile USB_t *) 0x40005C00)
#define CAN ((volatile CAN_t *) 0x40006400)
#define BKP ((volatile BKP_t *) 0x40006C04)
#define PWR ((volatile PWR_t *) 0x40007000)
#define DAC ((volatile DAC_t *) 0x40007400)
#define AFIO ((volatile AFIO_t *) 0x40010000)
#define EXTI ((volatile EXTI_t *) 0x40010400)
#define GPIOA ((volatile GPIOA_t *) 0x40010800)
#define GPIOB ((volatile GPIOB_t *) 0x40010C00)
#define GPIOC ((volatile GPIOC_t *) 0x40011000)
#define GPIOD ((volatile GPIOD_t *) 0x40011400)
#define GPIOE ((volatile GPIOE_t *) 0x40011800)
#define GPIOF ((volatile GPIOF_t *) 0x40011C00)
#define GPIOG ((volatile GPIOG_t *) 0x40012000)
#define ADC1 ((volatile ADC1_t *) 0x40012400)
#define ADC2 ((volatile ADC2_t *) 0x40012800)
#define TIM1 ((volatile TIM1_t *) 0x40012C00)
#define SPI1 ((volatile SPI1_t *) 0x40013000)
#define TIM8 ((volatile TIM8_t *) 0x40013400)
#define USART1 ((volatile USART1_t *) 0x40013800)
#define ADC3 ((volatile ADC3_t *) 0x40013C00)
#define TIM9 ((volatile TIM9_t *) 0x40014C00)
#define TIM10 ((volatile TIM10_t *) 0x40015000)
#define TIM11 ((volatile TIM11_t *) 0x40015400)
#define SDIO ((volatile SDIO_t *) 0x40018000)
#define DMA1 ((volatile DMA1_t *) 0x40020000)
#define DMA2 ((volatile DMA2_t *) 0x40020400)
#define RCC ((volatile RCC_t *) 0x40021000)
#define FLASH ((volatile FLASH_t *) 0x40022000)
#define CRC ((volatile CRC_t *) 0x40023000)
#define FSMC ((volatile FSMC_t *) 0xA0000000)
#define NVIC ((volatile NVIC_t *) 0xE000E000)
#define DBG ((volatile DBG_t *) 0xE0042000)


typedef enum{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
/******  STM32F103xx specific Interrupt Numbers **********************************************************************/
  WWDG_IRQn = 0, // Window Watchdog interrupt
  PVD_IRQn = 1, // PVD through EXTI line detection interrupt
  TAMPER_IRQn = 2, // Tamper interrupt
  RTC_IRQn = 3, // RTC global interrupt
  FLASH_IRQn = 4, // Flash global interrupt
  RCC_IRQn = 5, // RCC global interrupt
  EXTI0_IRQn = 6, // EXTI Line0 interrupt
  EXTI1_IRQn = 7, // EXTI Line1 interrupt
  EXTI2_IRQn = 8, // EXTI Line2 interrupt
  EXTI3_IRQn = 9, // EXTI Line3 interrupt
  EXTI4_IRQn = 10, // EXTI Line4 interrupt
  DMA1_Channel1_IRQn = 11, // DMA1 Channel1 global interrupt
  DMA1_Channel2_IRQn = 12, // DMA1 Channel2 global interrupt
  DMA1_Channel3_IRQn = 13, // DMA1 Channel3 global interrupt
  DMA1_Channel4_IRQn = 14, // DMA1 Channel4 global interrupt
  DMA1_Channel5_IRQn = 15, // DMA1 Channel5 global interrupt
  DMA1_Channel6_IRQn = 16, // DMA1 Channel6 global interrupt
  DMA1_Channel7_IRQn = 17, // DMA1 Channel7 global interrupt
  ADC_IRQn = 18, // ADC1 global interrupt
  CAN1_TX_IRQn = 19, // CAN1 TX interrupts
  CAN1_RX0_IRQn = 20, // CAN1 RX0 interrupts
  CAN1_RX1_IRQn = 21, // CAN1 RX1 interrupt
  CAN1_SCE_IRQn = 22, // CAN1 SCE interrupt
  EXTI9_5_IRQn = 23, // EXTI Line[9:5] interrupts
  TIM1_BRK_TIM9_IRQn = 24, // TIM1 Break interrupt and TIM9 global interrupt
  TIM1_UP_TIM10_IRQn = 25, // TIM1 Update interrupt and TIM10 global interrupt
  TIM1_TRG_COM_TIM11_IRQn = 26, // TIM1 Trigger and Commutation interrupts and TIM11 global interrupt
  TIM1_CC_IRQn = 27, // TIM1 Capture Compare interrupt
  TIM2_IRQn = 28, // TIM2 global interrupt
  TIM3_IRQn = 29, // TIM3 global interrupt
  TIM4_IRQn = 30, // TIM4 global interrupt
  I2C1_EV_IRQn = 31, // I2C1 event interrupt
  I2C1_ER_IRQn = 32, // I2C1 error interrupt
  I2C2_EV_IRQn = 33, // I2C2 event interrupt
  I2C2_ER_IRQn = 34, // I2C2 error interrupt
  SPI1_IRQn = 35, // SPI1 global interrupt
  SPI2_IRQn = 36, // SPI2 global interrupt
  USART1_IRQn = 37, // USART1 global interrupt
  USART2_IRQn = 38, // USART2 global interrupt
  USART3_IRQn = 39, // USART3 global interrupt
  EXTI15_10_IRQn = 40, // EXTI Line[15:10] interrupts
  RTCAlarm_IRQn = 41, // RTC Alarms through EXTI line interrupt
  USB_FS_WKUP_IRQn = 42, // USB Device FS Wakeup through EXTI line interrupt
  TIM8_BRK_TIM12_IRQn = 43, // TIM8 Break interrupt and TIM12 global interrupt
  TIM8_UP_TIM13_IRQn = 44, // TIM8 Update interrupt and TIM13 global interrupt
  TIM8_TRG_COM_TIM14_IRQn = 45, // TIM8 Trigger and Commutation interrupts and TIM14 global interrupt
  TIM8_CC_IRQn = 46, // TIM8 Capture Compare interrupt
  ADC3_IRQn = 47, // ADC3 global interrupt
  FSMC_IRQn = 48, // FSMC global interrupt
  SDIO_IRQn = 49, // SDIO global interrupt
  TIM5_IRQn = 50, // TIM5 global interrupt
  SPI3_IRQn = 51, // SPI3 global interrupt
  UART4_IRQn = 52, // UART4 global interrupt
  UART5_IRQn = 53, // UART5 global interrupt
  TIM6_IRQn = 54, // TIM6 global interrupt
  TIM7_IRQn = 55, // TIM7 global interrupt
  DMA2_Channel1_IRQn = 56, // DMA2 Channel1 global interrupt
  DMA2_Channel2_IRQn = 57, // DMA2 Channel2 global interrupt
  DMA2_Channel3_IRQn = 58, // DMA2 Channel3 global interrupt
  DMA2_Channel4_5_IRQn = 59, // DMA2 Channel4 and DMA2 Channel5 global interrupt
} IRQn_Type;
