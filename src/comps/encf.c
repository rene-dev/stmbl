#include "encf_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include <string.h>

HAL_COMP(encf);

HAL_PIN(error);
HAL_PIN(dma);  //dma transfers

HAL_PIN(pos);
HAL_PIN(abs_pos);
HAL_PIN(state);
HAL_PIN(turns);
HAL_PIN(com_pos);
HAL_PIN(index);
HAL_PIN(req_len);

HAL_PIN(send_step);
HAL_PIN(crc_ok);
HAL_PIN(crc_er);

HAL_PIN(freq);
HAL_PIN(bit_ticks);

static volatile uint32_t sendf;
static uint32_t send_counterf;
static volatile uint16_t tim_data[160];

#pragma pack(push, 1)
typedef struct {
  uint32_t flag0 : 4;  // 0101
  uint32_t bat : 2;
  uint32_t flag1 : 3;  // 101
  uint32_t no_index : 1;
  uint32_t flag2 : 1;  // 0
  uint32_t pos_lo : 6;
  uint32_t flag3 : 2;  // 10
  uint32_t pos_hi : 16;
  uint32_t flag4 : 2;  // 10
  uint32_t turns : 16;
  uint32_t flag5 : 2;  // 10
  uint32_t com_pos : 10;
  uint32_t flag6 : 7;  // 0000011
  uint32_t crc : 5;
  uint32_t flag7 : 3;  // 000
} fanuc_t;
#pragma pack(pop)
/*
http://freeby.mesanet.com/regmap

Fanuc data format (76 bits): So far just for Aa64 (860-360-TXXX)
bits 0..4 	constant : = 0b00101
bit  5     	1=battery fail
bits 6,7	unknown = 0b10,a860-360 0b00,a860-370 
bit  8		1=un-indexed
bits 9..17	unknown, perhaps for higher res encoders
bits 18..33	16 bit absolute encoder data (0..65535 for one turn)
bits 34..35     unknown = 0b01
bits 36..51	16 bit absolute turns count
bits 52,53	unknown = 0b01
bits 54..63	10 bit absolute commutation encoder (four 0.1023 cycles per turn)
bits 64..70	unknown = 0b1100000
bits 71..75	ITU CRC-5 (calculated MSB first)

Note: Aa1000 (860-370-TXXX) is similar with bits 10..15 being 
additional lower order count bits (only 12..15 being of much use)
extending the 16 bit count from bits 18..33 with bits 12..15  
gives a resolution of 1048576 counts/turn

These encoders are absolute if the battery backup is maintained since the 
last homing. This can be determined by the status of the un-indexed bit, 
at least until the encoder crosses index. 

Surprisingly (well it surprised me anyway) the encoders maintain position 
and count with battery power alone. It appears that they keep the LEDS/
processor alive with a few 10s of uA of battery power (probably low duty 
cycle LED pulsing/analog circuits power cycling). 

This can be verified by reading the battery current and then moving the 
encoder, it goes from a few 10s of uA to many mA with a slow encoder move.

The commutation track is always absolute so can be used for commutation 
data regardless of the index status. Note that the commutation track
seems to be interpolated so is not better than maybe 1% accuracy
*/

static union {
  uint8_t enc_data[10];
  fanuc_t fanuc;
} data;
static uint8_t print_buf[10];

static int32_t pos_offset;
static uint32_t state_counter;

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct encf_ctx_t *ctx = (struct encf_ctx_t *)ctx_ptr;
  struct encf_pin_ctx_t *pins = (struct encf_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;

  //TX enable
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  //TIM CH1 A
  GPIO_InitStruct.GPIO_Pin   = FB0_A_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(FB0_A_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(FB0_A_PORT, FB0_A_PIN_SOURCE, FB0_ENC_TIM_AF);

  //TIM rx dma
  //fb0 rx auf 12: tim4_ch1 dma1_0_2
  //fb1 rx auf 12: tim1_ch1 dma2_3_6
  DMA_InitTypeDef DMA_InitStruct;
  DMA_InitStruct.DMA_Channel            = DMA_Channel_2;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&FB0_ENC_TIM->CCR1;
  DMA_InitStruct.DMA_Memory0BaseAddr    = (uint32_t)&tim_data;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize         = ARRAY_SIZE(tim_data);
  DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryDataSize     = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStruct.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Cmd(DMA1_Stream0, DISABLE);
  DMA_DeInit(DMA1_Stream0);
  DMA_Init(DMA1_Stream0, &DMA_InitStruct);

  //timer setup
  RCC_APB1PeriphClockCmd(FB0_ENC_TIM_RCC, ENABLE);
  FB0_ENC_TIM->CR1 &= ~TIM_CR1_CEN;
  FB0_ENC_TIM->CCMR1 = TIM_CCMR1_CC1S_0;                                // cc1 input ti1
  FB0_ENC_TIM->CCER  = TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NP;  // cc1 en, rising edge, falling edge
  FB0_ENC_TIM->ARR   = 65535;
  FB0_ENC_TIM->DIER  = TIM_DIER_CC1DE;  // enable cc1 dma reeuest

  //SPI is used to generate request
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  SPI_InitTypeDef SPI_InitTypeDefStruct;
  SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitTypeDefStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitTypeDefStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitTypeDefStruct.SPI_DataSize          = SPI_DataSize_16b;
  SPI_InitTypeDefStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitTypeDefStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitTypeDefStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitTypeDefStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_Init(SPI3, &SPI_InitTypeDefStruct);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  SPI_Cmd(SPI3, ENABLE);

  GPIO_SetBits(GPIOD, GPIO_Pin_15);  //tx enable

  pos_offset    = 0;
  PIN(req_len)  = 2046;
  state_counter = 0;
  PIN(freq) = 1024000;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct encf_ctx_t *ctx      = (struct encf_ctx_t *)ctx_ptr;
  struct encf_pin_ctx_t *pins = (struct encf_pin_ctx_t *)pin_ptr;

  uint32_t count = ARRAY_SIZE(tim_data) - DMA1_Stream0->NDTR;
  PIN(dma)       = count;

  for(int i = 0; i < 10; i++) {
    data.enc_data[i] = 0;
  }

  //1 bit = 80 ticks 82e6/1.024e6
  PIN(bit_ticks) = 82000000 / PIN(freq);

  uint8_t bits_sum = 0;
  for(int i = 1; i < count; i++) {  //each capture form dma
    //calculate time between edges
    uint16_t diff = tim_data[i] - tim_data[i - 1];
    //number of bits to set
    int bits = (float)diff / PIN(bit_ticks) + 0.5;
    if(i % 2 == 0) {  //line starts high, set every even numbered captures to 1
      for(int j = bits_sum; j < bits + bits_sum; j++) {
        data.enc_data[j / 8] |= (1 << j % 8);
      }
    }
    bits_sum += bits;
  }
  //set remaining bits to 1
  for(int j = bits_sum; j < 77; j++) {
    data.enc_data[j / 8] |= (1 << j % 8);
  }

  if(!sendf) {
    memcpy((void *)print_buf, (void *)data.enc_data, 10);
    sendf = 1;
  }
  if(bits_sum > 50) {
    //check crc. TODO: use result, change to word/byte algorithm
    //http://freeby.mesanet.com/fabsread.pas
    uint8_t crc[5]    = {0, 0, 0, 0, 0};
    uint8_t oldcrc[5] = {0, 0, 0, 0, 0};
    for(uint8_t i = 76; i >= 1; i--) {
      uint8_t bit = (data.enc_data[i / 8] & (1 << i % 8)) ? 1 : 0;
      crc[0]      = oldcrc[4] ^ bit;
      crc[1]      = oldcrc[0];
      crc[2]      = oldcrc[1] ^ bit ^ oldcrc[4];
      crc[3]      = oldcrc[2];
      crc[4]      = oldcrc[3] ^ bit ^ oldcrc[4];
      oldcrc[0]   = crc[0];
      oldcrc[1]   = crc[1];
      oldcrc[2]   = crc[2];
      oldcrc[3]   = crc[3];
      oldcrc[4]   = crc[4];
    }
    if(crc[0] == 0 && crc[1] == 0 && crc[2] == 0 && crc[3] == 0 && crc[4] == 0) {
      PIN(crc_ok)++;
      int32_t pos = data.fanuc.pos_lo + (data.fanuc.pos_hi << 6);
      PIN(index) = data.fanuc.no_index;

      PIN(abs_pos) = mod((float)pos * 2.0 * M_PI / (1 << 22));

      if(PIN(index) > 0.0) {
        pos_offset    = pos;
        PIN(pos)      = PIN(abs_pos);
        PIN(state)    = 1;
        state_counter = 1;
      } else if(state_counter == 1) {
        state_counter = 2;
        pos_offset    = pos;
        PIN(pos)      = PIN(abs_pos);
      } else {
        state_counter = 3;
        PIN(pos)      = mod((float)(pos + pos_offset) * 2.0 * M_PI / (1 << 22));
        PIN(state)    = 3;
      }

      PIN(turns)   = data.fanuc.turns;
      pos          = data.fanuc.com_pos;
      PIN(com_pos) = mod(pos * 2.0 * M_PI / 1024);
      PIN(error)   = 0;
    } else {
      PIN(crc_er)++;
      PIN(state) = 1;
      PIN(error) = 1;
    }
  } else {
    PIN(error)    = 1;
    PIN(state)    = 1;
    state_counter = 0;
  }
  //reset timer
  FB0_ENC_TIM->CNT  = 0;
  FB0_ENC_TIM->CCR1 = 0;
  FB0_ENC_TIM->CR1 |= TIM_CR1_CEN;  // enable tim

  //send request, 1/(42e6/32)*11 = 8.4uS
  SPI3->DR = PIN(req_len);
  //start DMA
  DMA_Cmd(DMA1_Stream0, DISABLE);
  DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
  DMA_Cmd(DMA1_Stream0, ENABLE);
}

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct encf_ctx_t *ctx = (struct encf_ctx_t *)ctx_ptr;
  struct encf_pin_ctx_t *pins = (struct encf_pin_ctx_t *)pin_ptr;

  if(sendf == 1 && send_counterf++ >= PIN(send_step) - 1 && PIN(send_step) >= 50) {
    send_counterf = 0;
    for(int i = 1; i < 77; i++) {
      if(print_buf[i / 8] & (1 << i % 8)) {
        printf("1");
      } else {
        printf("0");
      }
    }
    printf("\n");
    sendf = 0;
  }
}

hal_comp_t encf_comp_struct = {
    .name      = "encf",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct encf_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
