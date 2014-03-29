//--------------------------------------------------------------
// File     : stm32_ub_dac_dma.c
// Datum    : 25.03.2013
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, DAC, TIM, DMA
// Funktion : DA-Wandler (DAC1 und DAC2) per DMA1
//
// Hinweis  : DAC1-Ausgang an PA4
//            DAC2-Ausgang an PA5
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_dac_dma.h"



//--------------------------------------------------------------
// Globale Variable
//--------------------------------------------------------------
DAC_InitTypeDef  DAC_InitStructure;  // MUSS Global sein (BUGFIX)
DAC_DMA_MODE_t akt_dac_dma_mode;



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_DAC_DMA_InitIO1(void);
void P_DAC_DMA_InitTIM1(void);
void P_DAC_DMA_InitDAC1(void);
void P_DAC_DMA_InitIO2(void);
void P_DAC_DMA_InitTIM2(void);
void P_DAC_DMA_InitDAC2(void);



//--------------------------------------------------------------
// standard Kurvenform für DAC=AUS
//--------------------------------------------------------------
const uint16_t WaveOff[] = {0}; // Dauer Lo-Pegel


//--------------------------------------------------------------
// Kurvenform die Ausgegeben werden soll als Array
// von 12bit Werten (0 bis 4095)
//--------------------------------------------------------------
const uint16_t Wave1[] = { // Sinus
  2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056,
  4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
  2047, 1647, 1263,  909,  599,  344,  155,   38,
     0,   38,  155,  344,  599,  909, 1263, 1647
};


//--------------------------------------------------------------
// Kurvenform die Ausgegeben werden soll als Array
// von 12bit Werten (0 bis 4095)
//--------------------------------------------------------------
const uint16_t Wave2[] = { // Sägezahn
     0,  256,  512,  768, 1024, 1280, 1536, 1792,
  2048, 2304, 2560, 2816, 3072, 3328, 3584, 3840,
  4095
};


//--------------------------------------------------------------
// Kurvenform die Ausgegeben werden soll als Array
// von 12bit Werten (0 bis 4095)
//--------------------------------------------------------------
const uint16_t Wave3[] = { // Dreieck
     0,  256,  512,  768, 1024, 1280, 1536, 1792,
  2048, 2304, 2560, 2816, 3072, 3328, 3584, 3840,
  4095, 3840, 3584, 3328, 3072, 2816, 2560, 2304,
  2048, 1792, 1536, 1280, 1024,  768,  512,  256
};


//--------------------------------------------------------------
// Kurvenform die Ausgegeben werden soll als Array
// von 12bit Werten (0 bis 4095)
//--------------------------------------------------------------
const uint16_t Wave4[] = { // Rechteck
     0,  0,  4095,  4095
};


//--------------------------------------------------------------
// Definition aller Kurvenformen
// Reihenfolge wie bei DAC_DMA_WAVE_NAME_t
//--------------------------------------------------------------
const DAC_WAVE_t DAC_WAVE[] = {
  // Name             , Kurve  , Länge
  {DAC_WAVE_OFF       , WaveOff, 1},
  {DAC_WAVE1_SINUS    , Wave1  , 32},
  {DAC_WAVE2_SAEGEZAHN, Wave2  , 17},
  {DAC_WAVE3_DREIECK  , Wave3  , 32},
  {DAC_WAVE4_RECHTECK , Wave4  , 4},
};




//--------------------------------------------------------------
// init vom DA-Wandler im DMA-Mode
// mode : [SINGLE_DAC1_DMA, SINGLE_DAC2_DMA, DUAL_DAC_DMA]
//--------------------------------------------------------------
void UB_DAC_DMA_Init(DAC_DMA_MODE_t mode)
{
  // DAC-Mode speichern
  akt_dac_dma_mode=mode;

  if((mode==SINGLE_DAC1_DMA) || (mode==DUAL_DAC_DMA)) {
    P_DAC_DMA_InitIO1();
    P_DAC_DMA_InitTIM1();
    P_DAC_DMA_InitDAC1();
    UB_DAC_DMA_SetWaveform1(DAC_WAVE_OFF);
  }
  if((mode==SINGLE_DAC2_DMA) || (mode==DUAL_DAC_DMA)) {
    P_DAC_DMA_InitIO2();
    P_DAC_DMA_InitTIM2();
    P_DAC_DMA_InitDAC2();
    UB_DAC_DMA_SetWaveform2(DAC_WAVE_OFF);
  }
}


//--------------------------------------------------------------
// stellt eine Kurvenform für DAC1 ein
//--------------------------------------------------------------
void UB_DAC_DMA_SetWaveform1(DAC_DMA_WAVE_NAME_t wave)
{
  DMA_InitTypeDef DMA_InitStructure;

  if(akt_dac_dma_mode==SINGLE_DAC2_DMA) return; // wenn nicht erlaubt

  // Clock Enable (DMA)
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); 

  // DMA init (DMA1, Channel7, Stream5)
  DMA_Cmd(DMA1_Stream5, DISABLE);
  DMA_DeInit(DMA1_Stream5);
  DMA_InitStructure.DMA_Channel = DMA_Channel_7;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DAC1_DHR_12R1_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DAC_WAVE[wave].ptr;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = DAC_WAVE[wave].wave_len;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream5, &DMA_InitStructure);

  // DMA enable
  DMA_Cmd(DMA1_Stream5, ENABLE);
  // DMA-DAC enable
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}


//--------------------------------------------------------------
// stellt eine Kurvenform für DAC2 ein
//--------------------------------------------------------------
void UB_DAC_DMA_SetWaveform2(DAC_DMA_WAVE_NAME_t wave)
{
  DMA_InitTypeDef DMA_InitStructure;

  if(akt_dac_dma_mode==SINGLE_DAC1_DMA) return; // wenn nicht erlaubt

  // Clock Enable (DMA)
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

  // DMA init (DMA1, Channel7, Stream6)
  DMA_Cmd(DMA1_Stream6, DISABLE);
  DMA_DeInit(DMA1_Stream6);
  DMA_InitStructure.DMA_Channel = DMA_Channel_7;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DAC2_DHR_12R2_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DAC_WAVE[wave].ptr;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = DAC_WAVE[wave].wave_len;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);

  // DMA enable
  DMA_Cmd(DMA1_Stream6, ENABLE);
  // DMA-DAC enable
  DAC_DMACmd(DAC_Channel_2, ENABLE);
}


//--------------------------------------------------------------
// stellt die Frequenz für DAC1 ein
// Grundfrequenz = 2*APB1 (APB1=42MHz) => TIM_CLK=84MHz
// vorteiler : 0 bis 0xFFFF
// periode   : 0 bis 0xFFFF
//
// Ausgangs-Frq = TIM_CLK/(vorteiler+1)/(periode+1)/wave_len
//--------------------------------------------------------------
void UB_DAC_DMA_SetFrq1(uint16_t vorteiler, uint16_t periode)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

  if(akt_dac_dma_mode==SINGLE_DAC2_DMA) return; // wenn nicht erlaubt

  // Timer-Config
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = periode;
  TIM_TimeBaseStructure.TIM_Prescaler = vorteiler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(DAC_TIM1_NAME, &TIM_TimeBaseStructure);
}


//--------------------------------------------------------------
// stellt die Frequenz für DAC2 ein
// Grundfrequenz = 2*APB1 (APB1=42MHz) => TIM_CLK=84MHz
// vorteiler : 0 bis 0xFFFF
// periode   : 0 bis 0xFFFF
//
// Ausgangs-Frq = TIM_CLK/(vorteiler+1)/(periode+1)/wave_len
//--------------------------------------------------------------
void UB_DAC_DMA_SetFrq2(uint16_t vorteiler, uint16_t periode)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
 
  if(akt_dac_dma_mode==SINGLE_DAC1_DMA) return; // wenn nicht erlaubt

  // Timer-Config
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = periode;
  TIM_TimeBaseStructure.TIM_Prescaler = vorteiler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(DAC_TIM2_NAME, &TIM_TimeBaseStructure);
}


//--------------------------------------------------------------
// interne Funktion
// Init vom IO-Pin für DAC1
//--------------------------------------------------------------
void P_DAC_DMA_InitIO1(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  // Clock Enable (GPIO)
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  // Config des DAC-Pins als Analog-Ausgang
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


//--------------------------------------------------------------
// interne Funktion
// Init vom IO-Pin für DAC2
//--------------------------------------------------------------
void P_DAC_DMA_InitIO2(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  // Clock Enable (GPIO)
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  // Config des DAC-Pins als Analog-Ausgang
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


//--------------------------------------------------------------
// interne Funktion
// Init vom Timer für DAC1
//--------------------------------------------------------------
void P_DAC_DMA_InitTIM1(void)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

  // Clock enable (Timer)
  RCC_APB1PeriphClockCmd(DAC_TIM1_CLK, ENABLE);

  // Timer-Config
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = DAC_DMA_DEF_PERIODE;
  TIM_TimeBaseStructure.TIM_Prescaler = DAC_DMA_DEF_PRESCALE;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(DAC_TIM1_NAME, &TIM_TimeBaseStructure);

  // Trigger
  TIM_SelectOutputTrigger(DAC_TIM1_NAME, TIM_TRGOSource_Update);

  // Timer enable
  TIM_Cmd(DAC_TIM1_NAME, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// Init vom Timer für DAC2
//--------------------------------------------------------------
void P_DAC_DMA_InitTIM2(void)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

  // Clock enable (Timer)
  RCC_APB1PeriphClockCmd(DAC_TIM2_CLK, ENABLE);

  // Timer-Config
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = DAC_DMA_DEF_PERIODE;
  TIM_TimeBaseStructure.TIM_Prescaler = DAC_DMA_DEF_PRESCALE;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(DAC_TIM2_NAME, &TIM_TimeBaseStructure);

  // Trigger
  TIM_SelectOutputTrigger(DAC_TIM2_NAME, TIM_TRGOSource_Update);

  // Timer enable
  TIM_Cmd(DAC_TIM2_NAME, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// Init vom DAC1
//--------------------------------------------------------------
void P_DAC_DMA_InitDAC1(void)
{
  // Clock Enable (DAC)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  // DAC init
  DAC_InitStructure.DAC_Trigger = DAC_TIM1_TRG;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  // DAC enable
  DAC_Cmd(DAC_Channel_1, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// Init vom DAC2
//--------------------------------------------------------------
void P_DAC_DMA_InitDAC2(void)
{
  // Clock Enable (DAC)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  // DAC init
  DAC_InitStructure.DAC_Trigger = DAC_TIM2_TRG;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  // DAC enable
  DAC_Cmd(DAC_Channel_2, ENABLE);
}
