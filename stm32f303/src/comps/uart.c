#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"
//#include "common.h"

extern CRC_HandleTypeDef hcrc;

#define DATABAUD 115200

HAL_COMP(uart);

//process data from LS

HAL_PIN(current);
HAL_PIN(en);
HAL_PIN(crc_error);

HAL_PIN(rpm);
HAL_PIN(iabs);
HAL_PIN(udc);
HAL_PIN(temp);
HAL_PIN(mot_temp);


typedef struct{
   int16_t current;
   //uint32_t crc;
} ottersequence;

typedef struct{
   float speed;
   float current;
   float voltage;
   float temp;
   float mot_temp;
   //uint32_t crc;
} tachosequence;

struct uart_ctx_t{
   uint32_t timeout;
   volatile ottersequence ottersequence;
   volatile tachosequence tachosequence;
};

uint8_t startup = 0;

int16_t currentBuffer[11];


// volatile packet_to_hv_t packet_to_hv;
// volatile packet_from_hv_t packet_from_hv;

//f3_config_data_t config;
//f3_state_data_t state;

static void hw_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct uart_ctx_t * ctx = (struct uart_ctx_t *)ctx_ptr;
   // struct ls_pin_ctx_t * pins = (struct ls_pin_ctx_t *)pin_ptr;

   GPIO_InitTypeDef GPIO_InitStruct;

   /* Peripheral clock enable */
   __HAL_RCC_USART3_CLK_ENABLE();

   UART_HandleTypeDef huart3;
   huart3.Instance = USART3;
   huart3.Init.BaudRate = DATABAUD;
   huart3.Init.WordLength = UART_WORDLENGTH_8B;
   huart3.Init.StopBits = UART_STOPBITS_1;
   huart3.Init.Parity = UART_PARITY_NONE;
   huart3.Init.Mode = UART_MODE_TX_RX;
   huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart3.Init.OverSampling = UART_OVERSAMPLING_8;
   huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
   huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
   USART3->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR | USART_CR3_OVRDIS;
   HAL_UART_Init(&huart3);

   /**USART3 GPIO Configuration
   PB10     ------> USART3_TX
   PB11     ------> USART3_RX
   */
   GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
   GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


   GPIO_InitStruct.Pin = GPIO_PIN_15;  //RS485_EN
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);

   GPIO_InitStruct.Pin = GPIO_PIN_2;  //Green
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   GPIO_InitStruct.Pin = GPIO_PIN_7;  //Blue
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   GPIO_InitStruct.Pin = GPIO_PIN_0;  //Red
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   __HAL_RCC_DMA1_CLK_ENABLE();

     //TX DMA
    DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);
    DMA1_Channel2->CPAR  = (uint32_t) & (USART3->TDR);
    DMA1_Channel2->CMAR  = (uint32_t) & (ctx->tachosequence);
    DMA1_Channel2->CNDTR = sizeof(tachosequence);
    DMA1_Channel2->CCR   = DMA_CCR_MINC | DMA_CCR_DIR;  // | DMA_CCR_PL_0 | DMA_CCR_PL_1
    DMA1->IFCR           = DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2 | DMA_IFCR_CGIF2;


   //RX DMA
   DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR_EN);
   DMA1_Channel3->CPAR = (uint32_t)&(USART3->RDR);
   DMA1_Channel3->CMAR = (uint32_t)&(ctx->ottersequence);
   DMA1_Channel3->CNDTR = sizeof(ottersequence);
   DMA1_Channel3->CCR = DMA_CCR_MINC;// | DMA_CCR_PL_0 | DMA_CCR_PL_1
   DMA1->IFCR = DMA_IFCR_CTCIF3 | DMA_IFCR_CHTIF3 | DMA_IFCR_CGIF3;
   DMA1_Channel3->CCR |= DMA_CCR_EN;

   USART3->RTOR = 16; // 16 bits timeout
   USART3->CR2 |= USART_CR2_RTOEN; // timeout en
   USART3->ICR |= USART_ICR_RTOCF; // timeout clear flag
}

static void rt_start(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct uart_ctx_t * ctx = (struct uart_ctx_t *)ctx_ptr;
   struct uart_pin_ctx_t * pins = (struct uart_pin_ctx_t *)pin_ptr;
   ctx->timeout = 0;
   ledBlue(0);
   ledRed(0);
   ledGreen(0);
   PIN(current) = 0;
   for (int i = 0; i < 10; i++)
   {
     currentBuffer[i] = 0;
   }
   ctx->ottersequence.current = 0;
   ctx->timeout = 0;
   PIN(en) = 0;
}


void ledRed(uint8_t state) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, !state);
}

void ledBlue(uint8_t state) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, !state);
}

void ledGreen(uint8_t state) {
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, !state);
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct uart_ctx_t * ctx = (struct uart_ctx_t *)ctx_ptr;
   struct uart_pin_ctx_t * pins = (struct uart_pin_ctx_t *)pin_ptr;

   uint32_t dma_pos = sizeof(ottersequence) - DMA1_Channel3->CNDTR;


   if(dma_pos == sizeof(ottersequence) && startup){
      //uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *) &(ctx->ottersequence), sizeof(ottersequence) / 4 - 1);
      //if(crc == ctx->ottersequence.crc){
         ledBlue(1);
         ledGreen(0);

         currentBuffer[9] = currentBuffer[8];
         currentBuffer[8] = currentBuffer[7];
         currentBuffer[7] = currentBuffer[6];
         currentBuffer[6] = currentBuffer[5];
         currentBuffer[5] = currentBuffer[4];
         currentBuffer[4] = currentBuffer[3];
         currentBuffer[3] = currentBuffer[2];
         currentBuffer[2] = currentBuffer[1];
         currentBuffer[1] = currentBuffer[0];
         currentBuffer[0] = ctx->ottersequence.current;

         float current = 0;
         for (int i = 0; i < 10; i++)
         {
           current += currentBuffer[i];
         }
         PIN(current) = (current / 10.0) / 310.0 ;
         PIN(en) = 1.0;
         ctx->timeout = 0;

         // fill tx struct
        ctx->tachosequence.speed  = PIN(rpm);
        ctx->tachosequence.current  = PIN(iabs);
        ctx->tachosequence.voltage  = PIN(udc);
        ctx->tachosequence.temp  = PIN(temp);
        ctx->tachosequence.mot_temp  = PIN(mot_temp);
        //ctx->packet_from_hv.crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&(ctx->packet_from_hv), sizeof(packet_from_hv_t) / 4 - 1);

        // start tx DMA
        DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);
        DMA1_Channel2->CNDTR = sizeof(tachosequence);
        DMA1_Channel2->CCR |= DMA_CCR_EN;
      //}
      //else{
      //   PIN(crc_error)++;
      //}
   }

   if(USART3->ISR & USART_ISR_RTOF){ // idle line
      USART3->ICR |= USART_ICR_RTOCF | USART_ICR_FECF | USART_ICR_ORECF; // timeout clear flag
      GPIOA->BSRR |= GPIO_PIN_10;

      // reset rx DMA
      DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR_EN);
      DMA1_Channel3->CNDTR = sizeof(ottersequence);
      DMA1_Channel3->CCR |= DMA_CCR_EN;
      dma_pos = 0;
      GPIOA->BSRR |= GPIO_PIN_10 << 16;

      //ctx->send = 1;
   }

   if(ctx->timeout > 1000 && startup){//disable driver
      PIN(en) = 0.0;
      PIN(current) = 0.0;
      ledBlue(0);
      ledGreen(1);
   }
   if(ctx->timeout > 75000) {
     ledGreen(1);
     startup = 1;
   }

   ctx->timeout++;

   // TODO: sin = 0.5
}

hal_comp_t uart_comp_struct = {
   .name = "uart",
   .nrt = 0,
   .rt = rt_func,
   .frt = 0,
   .hw_init = hw_init,
   .rt_start = rt_start,
   .frt_start = 0,
   .rt_stop = 0,
   .frt_stop = 0,
   .ctx_size = sizeof(struct uart_ctx_t),
   .pin_count = sizeof(struct uart_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
