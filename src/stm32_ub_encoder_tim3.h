//--------------------------------------------------------------
// File     : stm32_ub_encoder_tim3.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_ENCODER_TIM3_H
#define __STM32F4_UB_ENCODER_TIM3_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"


//--------------------------------------------------------------
// Zwei Kanäle [0 und 1] werden für den Rotary-Encoder benötigt
//--------------------------------------------------------------
typedef enum
{
  ENC_T3_A  = 0,  // Encoder Kanal-A
  ENC_T3_B  = 1   // Encoder Kanal-B
}ENC_TIM3_NAME_t;


//--------------------------------------------------------------
// Mode vom Encoder
//--------------------------------------------------------------
typedef enum {
  ENC_T3_MODE_2A  = 0, // Counter Zählt bei jeder Flanke von Eingang_A
  ENC_T3_MODE_2B  = 1, // Counter Zählt bei jeder Flanke von Eingang_B
  ENC_T3_MODE_4AB = 2  // Counter Zählt bei jeder Flanke von Eingang_A UND Eingang_B
}ENC_TIM3_MODE_t;


//--------------------------------------------------------------
// Richtung vom Encoder
//--------------------------------------------------------------
typedef enum {
  ENC_T3_TYP_NORMAL  = 0, // Counter incrementiert bei rechts
  ENC_T3_TYP_REVERS  = 1  // Counter decrementiert bei rechts
}ENC_TIM3_TYP_t;



//--------------------------------------------------------------
// Struktur von einem Encoder-Kanal
//--------------------------------------------------------------
typedef struct {
  ENC_TIM3_NAME_t ENC_NAME;      // Name
  const uint8_t CHANNEL;         // Channel [1...2]
  GPIO_TypeDef* ENC_PORT;        // Port
  const uint16_t ENC_PIN;        // Pin
  const uint32_t ENC_CLK;        // Clock
  const uint8_t ENC_SOURCE;      // Source
}ENC_TIM3_t;



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_ENCODER_TIM3_Init(ENC_TIM3_MODE_t mode, ENC_TIM3_TYP_t typ, uint16_t maxwert);
uint16_t UB_ENCODER_TIM3_ReadPos(void);




//--------------------------------------------------------------
#endif // __STM32F4_UB_ENCODER_TIM3_H
