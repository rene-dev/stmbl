#include "comps/dmm_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(dmm);

HAL_PIN(pos);
HAL_PIN(error);
HAL_PIN(state);
HAL_PIN(dma);  //dma transfers left
HAL_PIN(dump);

#pragma pack(push,1)
typedef struct DMM_AsBitFieldInts
{
    // byte 0
    unsigned int C7 : 1;
    unsigned int L7 : 1;
    unsigned int H7 : 1;
    unsigned int F : 4;
    unsigned int _byte0_x : 1;

    // byte 1
    unsigned int H : 7;
    unsigned int _byte1_1 : 1;

    // byte 2
    unsigned int L : 7;
    unsigned int _byte2_1 : 1;

    // byte 3
    unsigned int C : 7;
    unsigned int _byte3_1 : 1;
} DMM_AsBitFieldInts;
typedef struct DMM_AsBitFieldBits
{
    // byte 0
    unsigned int C7 : 1;
    unsigned int L7 : 1;
    unsigned int H7 : 1;
    unsigned int F0 : 1;
    unsigned int F1 : 1;
    unsigned int F2 : 1;
    unsigned int F3 : 1;
    unsigned int _byte0_x : 1;

    // byte 1
    unsigned int H0 : 1;
    unsigned int H1 : 1;
    unsigned int H2 : 1;
    unsigned int H3 : 1;
    unsigned int H4 : 1;
    unsigned int H5 : 1;
    unsigned int H6 : 1;
    unsigned int _byte1_1 : 1;

    // byte 2
    unsigned int L0 : 1;
    unsigned int L1 : 1;
    unsigned int L2 : 1;
    unsigned int L3 : 1;
    unsigned int L4 : 1;
    unsigned int L5 : 1;
    unsigned int L6 : 1;
    unsigned int _byte2_1 : 1;

    // byte 3
    unsigned int C0 : 1;
    unsigned int C1 : 1;
    unsigned int C2 : 1;
    unsigned int C3 : 1;
    unsigned int C4 : 1;
    unsigned int C5 : 1;
    unsigned int C6 : 1;
    unsigned int _byte3_1 : 1;
} DMM_AsBitFieldBits;
typedef union DMM_Packet
{
    uint8_t bytes[4];
    DMM_AsBitFieldInts ints;
    DMM_AsBitFieldBits bits;
} DMM_Packet;
#pragma pack(pop)

uint8_t SGenerate8BitsCRC(int16_t Data16)   /*Input 16bits Data, make 8 bits CRC for X^8 + X^3 + 1    */
{
    int16_t StateX,StateY,i;
    uint8_t CRC8bits;
    StateX = Data16&0xFF00; //High 8bits for the States
    Data16 = Data16<<8;
    for(i=0;i<8;i++)
    {
        StateY = StateX<<1;
        if((StateX & 0x8000) != 0)
        {     //X7 = 1
            if((Data16 & 0x8000) != 0)
                StateY = StateY & 0xFE00; //D15=1 so X0=0
            else
                StateY = StateY | 0x0100; //D15=0 so X0=1
            if((StateX & 0x0400) != 0)
                StateY = StateY & 0xF700;  //X2 = 1 so X3 = 0
            else
                StateY = StateY | 0x0800;  //X2 = 0 so X3 = 1
        }
        else
        {     //X7 = 0
            if((Data16 & 0x8000) != 0)
                StateY = StateY | 0x0100; //D15=1 so X0=1
            else
                StateY = StateY & 0xFE00; //D15=0 so X0=0
            if((StateX & 0x0400) != 0)
                StateY = StateY | 0x0800;  //X2 = 1 so X3 = 1
            else
                StateY = StateY & 0xF700;  //X2 = 0 so X3 = 0
        }
        StateX = StateY;
        Data16 = Data16<<1;
    }
    StateX = StateX >> 8;
    CRC8bits = StateX & 0x00FF;
    return(CRC8bits);
}

struct dmm_ctx_t {
  uint8_t rxbuf[15];
  DMM_Packet data;
};

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct dmm_ctx_t *ctx = (struct dmm_ctx_t *)ctx_ptr;
  // struct dmm_pin_ctx_t * pins = (struct dmm_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  DMA_InitTypeDef DMA_InitStructure;

  //TX enable
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  //USART TX
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  //USART RX
  // GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART6);
  // GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  // GPIO_Init(GPIOB, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate            = 1875000;
  USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits            = USART_StopBits_1;
  USART_InitStruct.USART_Parity              = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStruct);

  /* Enable the USART */
  USART_Cmd(USART6, ENABLE);
  USART_HalfDuplexCmd(USART6, ENABLE);

  // DMA-Disable
  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_DeInit(DMA2_Stream1);

  // DMA2-Config
  DMA_InitStructure.DMA_Channel            = DMA_Channel_5;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&ctx->rxbuf;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = ARRAY_SIZE(ctx->rxbuf);
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  GPIO_ResetBits(GPIOD, GPIO_Pin_15);  //tx disable
  USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  DMA_Cmd(DMA2_Stream1, ENABLE);
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct dmm_ctx_t *ctx      = (struct dmm_ctx_t *)ctx_ptr;
  struct dmm_pin_ctx_t *pins = (struct dmm_pin_ctx_t *)pin_ptr;
  PIN(dma)                   = DMA2_Stream1->NDTR;;
  
  //next received byte will be written to bufferpos
  uint32_t bufferpos = ARRAY_SIZE(ctx->rxbuf) - DMA2_Stream1->NDTR;

  uint8_t crc = 0;
  uint16_t angle = 0;
  uint8_t found = 0;
  //TODO: clear buffer after reading
  for(uint32_t i = 4; i < 8; i++){
    for(int j = 0;j<4;j++){
      ctx->data.bytes[j] = ctx->rxbuf[(bufferpos+ARRAY_SIZE(ctx->rxbuf)-(i+3-j)) % ARRAY_SIZE(ctx->rxbuf)];
    }
    crc = (ctx->data.ints.C7 << 7) | ctx->data.ints.C;
    angle = (ctx->data.ints.H7 << 15) | (ctx->data.ints.H << 8) | (ctx->data.ints.L7 << 7) | ctx->data.ints.L;
    uint8_t res  = SGenerate8BitsCRC(angle);
    if(res == crc && ctx->data.bits._byte1_1 == 1 && ctx->data.bits._byte2_1 == 1 && ctx->data.bits._byte3_1 == 1 && ctx->data.bits._byte0_x == 0){
      found = 1;
      break;
    }
  }
  if(found){
    PIN(pos) = (angle * M_PI * 2.0f / 65536.0f) - M_PI;
    PIN(error) = 0;
  }else{
    PIN(error) = 1;
  }
}

hal_comp_t dmm_comp_struct = {
    .name      = "dmm",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct dmm_ctx_t),
    .pin_count = sizeof(struct dmm_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
