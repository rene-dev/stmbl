//--------------------------------------------------------------
// File     : stm32_ub_dac_dma.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_DAC_DMA_H
#define __STM32F4_UB_DAC_DMA_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"



typedef enum {
  SINGLE_DAC1_DMA = 0, // nur DAC1 an PA4 benutzen
  SINGLE_DAC2_DMA,     // nur DAC2 an PA5 benutzen
  DUAL_DAC_DMA         // DAC1 (PA4) und DAC2 (PA5) benutzen
}DAC_DMA_MODE_t;



//--------------------------------------------------------------
// Liste aller DAC-Kurvenformen
// (keine Nummer doppelt und von 0 beginnend)
//--------------------------------------------------------------
typedef enum {
  DAC_WAVE_OFF          = 0, // Dauer Lo-Pegel       [Len=1]
  DAC_WAVE1_SINUS       = 1, // Waveform1 (Sinus)    [Len=32]
  DAC_WAVE2_SAEGEZAHN   = 2, // Waveform2 (Sägezahn) [Len=17]
  DAC_WAVE3_DREIECK     = 3, // Waveform3 (Dreieck)  [Len=32]
  DAC_WAVE4_RECHTECK    = 4  // Waveform4 (Rechteck) [Len=4]
}DAC_DMA_WAVE_NAME_t;


//--------------------------------------------------------------
// Timer Grundeinstellungen
//--------------------------------------------------------------
#define  DAC_DMA_DEF_PERIODE   255  // default periode
#define  DAC_DMA_DEF_PRESCALE   10  // default prescaler


//--------------------------------------------------------------
// Struktur einer DAC-Kurvenform
//--------------------------------------------------------------
typedef struct {
  DAC_DMA_WAVE_NAME_t name;
  const uint16_t *ptr;
  uint16_t wave_len;
}DAC_WAVE_t;


//--------------------------------------------------------------
// Timer Einstellung
// Moegliche Timer fuer den DAC1 Trigger :
//   Timer2, Timer4, Timer5, Timer6, Timer7
//   (Timer8 hängt an APB2)
//--------------------------------------------------------------

// einen Timer auswählen
//#define DAC_DMA_T1_TIM2            2
//#define DAC_DMA_T1_TIM4            4
//#define DAC_DMA_T1_TIM5            5
#define DAC_DMA_T1_TIM6            6
//#define DAC_DMA_T1_TIM7            7


#ifdef DAC_DMA_T1_TIM2
  #define DAC_TIM1_TRG            DAC_Trigger_T2_TRGO
  #define DAC_TIM1_CLK            RCC_APB1Periph_TIM2
  #define DAC_TIM1_NAME           TIM2
#elif defined DAC_DMA_T1_TIM4
  #define DAC_TIM1_TRG            DAC_Trigger_T4_TRGO
  #define DAC_TIM1_CLK            RCC_APB1Periph_TIM4
  #define DAC_TIM1_NAME           TIM4
#elif defined DAC_DMA_T1_TIM5
  #define DAC_TIM1_TRG            DAC_Trigger_T5_TRGO
  #define DAC_TIM1_CLK            RCC_APB1Periph_TIM5
  #define DAC_TIM1_NAME           TIM5
#elif defined DAC_DMA_T1_TIM6
  #define DAC_TIM1_TRG            DAC_Trigger_T6_TRGO
  #define DAC_TIM1_CLK            RCC_APB1Periph_TIM6
  #define DAC_TIM1_NAME           TIM6
#elif defined DAC_DMA_T1_TIM7
  #define DAC_TIM1_TRG            DAC_Trigger_T7_TRGO
  #define DAC_TIM1_CLK            RCC_APB1Periph_TIM7
  #define DAC_TIM1_NAME           TIM7
#endif


//--------------------------------------------------------------
// Timer Einstellung
// Moegliche Timer fuer den DAC2 Trigger :
//   Timer2, Timer4, Timer5, Timer6, Timer7
//   (Timer8 hängt an APB2)
//--------------------------------------------------------------

// einen Timer auswählen
//#define DAC_DMA_T2_TIM2            2
//#define DAC_DMA_T2_TIM4            4
//#define DAC_DMA_T2_TIM5            5
//#define DAC_DMA_T2_TIM6            6
#define DAC_DMA_T2_TIM7            7


#ifdef DAC_DMA_T2_TIM2
  #define DAC_TIM2_TRG            DAC_Trigger_T2_TRGO
  #define DAC_TIM2_CLK            RCC_APB1Periph_TIM2
  #define DAC_TIM2_NAME           TIM2
#elif defined DAC_DMA_T2_TIM4
  #define DAC_TIM2_TRG            DAC_Trigger_T4_TRGO
  #define DAC_TIM2_CLK            RCC_APB1Periph_TIM4
  #define DAC_TIM2_NAME           TIM4
#elif defined DAC_DMA_T2_TIM5
  #define DAC_TIM2_TRG            DAC_Trigger_T5_TRGO
  #define DAC_TIM2_CLK            RCC_APB1Periph_TIM5
  #define DAC_TIM2_NAME           TIM5
#elif defined DAC_DMA_T2_TIM6
  #define DAC_TIM2_TRG            DAC_Trigger_T6_TRGO
  #define DAC_TIM2_CLK            RCC_APB1Periph_TIM6
  #define DAC_TIM2_NAME           TIM6
#elif defined DAC_DMA_T2_TIM7
  #define DAC_TIM2_TRG            DAC_Trigger_T7_TRGO
  #define DAC_TIM2_CLK            RCC_APB1Periph_TIM7
  #define DAC_TIM2_NAME           TIM7
#endif


//--------------------------------------------------------------
// Adressen der DACs
// (siehe Seite 55 vom Referenz Manual)
//--------------------------------------------------------------
#define DAC_BASE_ADR        ((uint32_t)0x40007400)


//--------------------------------------------------------------
// Adressen der Register
// (siehe Seite 325 vom Referenz Manual)
//--------------------------------------------------------------
#define DAC1_REG_DHR_12R1_OFFSET   0x08  // 12bit, rechtsbündig
#define DAC2_REG_DHR_12R2_OFFSET   0x14  // 12bit, rechtsbündig

#define DAC1_DHR_12R1_ADDRESS    (DAC_BASE_ADR | DAC1_REG_DHR_12R1_OFFSET)
#define DAC2_DHR_12R2_ADDRESS    (DAC_BASE_ADR | DAC2_REG_DHR_12R2_OFFSET)




//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_DAC_DMA_Init(DAC_DMA_MODE_t mode);
void UB_DAC_DMA_SetWaveform1(DAC_DMA_WAVE_NAME_t wave);
void UB_DAC_DMA_SetWaveform2(DAC_DMA_WAVE_NAME_t wave);
void UB_DAC_DMA_SetFrq1(uint16_t vorteiler, uint16_t periode);
void UB_DAC_DMA_SetFrq2(uint16_t vorteiler, uint16_t periode);




//--------------------------------------------------------------
#endif // __STM32F4_UB_DAC_DMA_H
