#include "ringbuf.h"
#include "defines.h"
#include "uart_cdc.h"
#include "stm32f3xx_hal.h"

#define DATABAUD 921600

#define RX_QUEUE_SIZE  1024
#define TX_QUEUE_SIZE  4096

volatile uint8_t txdmabuf[512];
volatile uint8_t rxdmabuf[512];

struct ringbuf rx_buf = RINGBUF(RX_QUEUE_SIZE);
struct ringbuf tx_buf = RINGBUF(TX_QUEUE_SIZE);

void cdc_init(){
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
  HAL_UART_Init(&huart3);
  USART3->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
  
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
  
  __HAL_RCC_DMA1_CLK_ENABLE();
  
  //TX DMA
  DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel2->CPAR = (uint32_t)&(USART3->TDR);
  DMA1_Channel2->CMAR = (uint32_t)&(txdmabuf);
  DMA1_Channel2->CNDTR = 0;
  DMA1_Channel2->CCR = DMA_CCR_MINC | DMA_CCR_DIR;// | DMA_CCR_PL_0 | DMA_CCR_PL_1
  DMA1->IFCR = DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2 | DMA_IFCR_CGIF2;
  
  //RX DMA
  DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel3->CPAR = (uint32_t)&(USART3->RDR);
  DMA1_Channel3->CMAR = (uint32_t)&(rxdmabuf);
  DMA1_Channel3->CNDTR = ARRAY_SIZE(rxdmabuf);
  DMA1_Channel3->CCR = DMA_CCR_MINC | DMA_CCR_CIRC;// | DMA_CCR_PL_0 | DMA_CCR_PL_1
  DMA1->IFCR = DMA_IFCR_CTCIF3 | DMA_IFCR_CHTIF3 | DMA_IFCR_CGIF3;
  DMA1_Channel3->CCR |= DMA_CCR_EN;
}


void cdc_usbtx(){
  if(DMA1_Channel2->CNDTR == 0){
    int len = rb_read(&tx_buf,txdmabuf,sizeof(txdmabuf));
    DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);
    DMA1_Channel2->CNDTR = len;
    DMA1_Channel2->CCR |= DMA_CCR_EN;
  }
}

int cdc_tx(void* data, uint32_t len){
  int ret;
  ret = rb_write(&tx_buf, data, len);
  cdc_usbtx();
  return ret;
}

void cdc_poll(){
  int available,bufferpos;
  static int rxpos = 0;
  cdc_usbtx();
  //next received packet will be written to bufferpos
  bufferpos = sizeof(rxdmabuf) - DMA1_Channel3->CNDTR;
  //how many packets we have the the rx buffer for processing
  available = (bufferpos - rxpos + sizeof(rxdmabuf)) % sizeof(rxdmabuf);

  //TODO: ringbuf dual pointer magic
  for(int i = 0;i<available;i++){
    rb_putc(&rx_buf,rxdmabuf[rxpos++%sizeof(rxdmabuf)]);
  }
  rxpos = rxpos % sizeof(rxdmabuf);
}

int cdc_is_connected(){
    return 1;
}

int cdc_getline(char *ptr, int len){
   return rb_getline(&rx_buf, ptr, len);
}

int _write(int file, char *ptr, int len){
	return cdc_tx(ptr, len);
}
