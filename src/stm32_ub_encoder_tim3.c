//--------------------------------------------------------------
// File     : stm32_ub_encoder_tim3.c
// Datum    : 21.07.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, TIM
// Funktion : Drehgeber Messung (Rotary Encoder) per Timer3
//
// Hinweis  : mögliche Pinbelegungen
//            CH1 : [PA6, PB4, PC6]
//            CH2 : [PA7, PB5, PC7]
//
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_encoder_tim3.h"



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_ENCODER_InitIO(void);
void P_ENCODER_InitTIM(ENC_TIM3_MODE_t mode, ENC_TIM3_TYP_t typ, uint16_t maxwert);


//--------------------------------------------------------------
// Definition der zwei Rotary-Encoder Pins
//
// Channel : [1 und 2]
//--------------------------------------------------------------
ENC_TIM3_t ENC_TIM3[] = {
  // Name   ,Channel, PORT , PIN       , CLOCK               , Source
  {ENC_T3_A ,1      ,GPIOB ,GPIO_Pin_4 ,RCC_AHB1Periph_GPIOB ,GPIO_PinSource4},
  {ENC_T3_B ,2      ,GPIOB ,GPIO_Pin_5 ,RCC_AHB1Periph_GPIOB ,GPIO_PinSource5},
};


//--------------------------------------------------------------
// init und start vom Encoder (Drehgeber-Messung) mit Timer3
//
// mode    : [ENC_MODE_2A, ENC_MODE_2B, ENC_MODE_4AB]
// typ     : [ENC_TYP_NORMAL, ENC_TYP_REVERS]
// maxwert : [0 bis 65535]
//-------------------------------------------------------------- 
void UB_ENCODER_TIM3_Init(ENC_TIM3_MODE_t mode, ENC_TIM3_TYP_t typ, uint16_t maxwert)
{
  // init der Funktionen
  P_ENCODER_InitIO();
  P_ENCODER_InitTIM(mode, typ, maxwert);
}


//--------------------------------------------------------------
// auslesen der aktuellen Position vom Drehgeber
//
// Return_Wert : [0 bis eingestelltem Maxwert]
//--------------------------------------------------------------
uint16_t UB_ENCODER_TIM3_ReadPos(void)
{
  uint16_t ret_wert=0;

  ret_wert=TIM_GetCounter(TIM3);

  return(ret_wert);
}



//--------------------------------------------------------------
// interne Funktion
// Init aller IO-Pins (mit PullUp)
//--------------------------------------------------------------
void P_ENCODER_InitIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ENC_TIM3_NAME_t vname;

  for(vname=0;vname<2;vname++) {
    // Clock Enable
    RCC_AHB1PeriphClockCmd(ENC_TIM3[vname].ENC_CLK, ENABLE);

    // Config des Pins als AF-Input
    GPIO_InitStructure.GPIO_Pin = ENC_TIM3[vname].ENC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(ENC_TIM3[vname].ENC_PORT, &GPIO_InitStructure);

    // Alternative-Funktion mit dem IO-Pin verbinden
    GPIO_PinAFConfig(ENC_TIM3[vname].ENC_PORT, ENC_TIM3[vname].ENC_SOURCE, GPIO_AF_TIM3);
  }
}



//--------------------------------------------------------------
// interne Funktion
// Init vom Timer für den Rotary-Encoder
//--------------------------------------------------------------
void P_ENCODER_InitTIM(ENC_TIM3_MODE_t mode, ENC_TIM3_TYP_t typ, uint16_t maxwert)
{
  uint16_t enc_mode, enc_edge;

  // Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  // Encoder Mode
  if(mode==ENC_T3_MODE_2A) {
    enc_mode=TIM_EncoderMode_TI1;
  }
  else if(mode==ENC_T3_MODE_2B){
    enc_mode=TIM_EncoderMode_TI2;
  }
  else {
    enc_mode=TIM_EncoderMode_TI12;
  }

  // Encoder Typ
  if(typ==ENC_T3_TYP_NORMAL) {
    enc_edge=TIM_ICPolarity_Rising;
  }
  else {
    enc_edge=TIM_ICPolarity_Falling;
  }

  // Modus einstellen
  TIM_EncoderInterfaceConfig(TIM3, enc_mode, enc_edge, TIM_ICPolarity_Rising);

  // Maxwert setzen
  TIM_SetAutoreload(TIM3, maxwert);

  // Timer enable
  TIM_Cmd(TIM3, ENABLE);
}

