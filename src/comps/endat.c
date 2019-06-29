#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include <string.h>

HAL_COMP(endat);

HAL_PIN(req);
HAL_PIN(pos_len);
HAL_PIN(mpos_len);
HAL_PIN(bytes);

HAL_PIN(pos);
HAL_PIN(mpos);

HAL_PIN(f1);
HAL_PIN(crc);
HAL_PIN(shift);
HAL_PIN(timer);
HAL_PIN(swap);

HAL_PIN(scip);

HAL_PINA(byte, 6);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct endat_ctx_t *ctx = (struct endat_ctx_t *)ctx_ptr;
  struct endat_pin_ctx_t *pins = (struct endat_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;

  //TX enable for mosi
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  //tx enable for clock
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  //SPI3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  
  SPI3->CR1 |= SPI_CR1_LSBFIRST | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BIDIMODE | SPI_BaudRatePrescaler_32;

  //PC12 spi3 mosi
  //PC10 spi3 clock
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_SetBits(GPIOD, GPIO_Pin_10);  //clock tx enable
  PIN(req) = 224;
  PIN(pos_len) = 18; // 17
  PIN(mpos_len) = 12; // 15
  PIN(swap) = 1;
  PIN(scip) = 9;
  PIN(bytes) = 7;
}

union{
    uint64_t data;
    uint8_t dataa[8];
  } df;

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct endat_ctx_t *ctx      = (struct endat_ctx_t *)ctx_ptr;
  struct endat_pin_ctx_t *pins = (struct endat_pin_ctx_t *)pin_ptr;

  SPI3->CR1 &= ~SPI_CR1_SPE;//disable spi

  SPI3->CR1 |= SPI_CR1_LSBFIRST | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BIDIMODE | SPI_BaudRatePrescaler_32;


  GPIO_SetBits(GPIOD, GPIO_Pin_15);//tx enable
  SPI3->CR1 |= SPI_CR1_BIDIOE;//enable output
  SPI3->CR1 |= SPI_CR1_SPE;//enable spi

  SPI3->DR = PIN(req);
  while(!(SPI3->SR & SPI_SR_TXE));
  while(SPI3->SR & SPI_SR_BSY);

  SPI3->CR1 &= ~SPI_CR1_BIDIOE;//disable output, this activates the clock
  GPIO_ResetBits(GPIOD, GPIO_Pin_15);//tx disable

  if(PIN(swap) > 0.0){ 
    SPI3->CR1 &= ~SPI_CR1_SPE;//disable spi
    SPI3->CR1 |= SPI_CR1_LSBFIRST | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BIDIMODE | SPI_BaudRatePrescaler_32;
    SPI3->CR1 |= SPI_CR1_SPE;//enable spi
  }

  for(int i = 0; i < MIN(sizeof(df.data), PIN(bytes)); i++){
    while(!(SPI3->SR & SPI_SR_RXNE));
    df.dataa[i] = SPI3->DR;
  }

  df.data = df.data >> (int)PIN(scip);
  PIN(shift) = PIN(scip);
  while((df.data & 1) == 0 && PIN(shift) < 32.0){
    df.data = df.data >> 1;
    PIN(shift)++;
  }

  uint8_t start_bit = (df.data & 1);
  uint8_t f1_bit = (df.data & 2);
  // uint8_t f2_bit = (df.data & 3);
  df.data = df.data >> 2;

  uint32_t pos_len = PIN(pos_len);
  uint32_t mpos_len = PIN(mpos_len);

  uint32_t pos = df.data << (32 - pos_len);
  uint8_t crc = (df.data >> (pos_len + mpos_len)) & 31;
  uint32_t mpos = (df.data << (sizeof(df.data) * 8 - pos_len - mpos_len)) >> (64 - mpos_len);

  PIN(pos) = mod(pos * 2.0 * M_PI / 4294967295.0);
  PIN(mpos) = mpos;
  PIN(f1) = f1_bit;
  PIN(crc) = crc;

  PINA(byte,0) = df.dataa[0];
  PINA(byte,1) = df.dataa[1];
  PINA(byte,2) = df.dataa[2];
  PINA(byte,3) = df.dataa[3];
  PINA(byte,4) = df.dataa[4];
  PINA(byte,5) = df.dataa[5];


  //TODO: wait for busy flag?
  SPI3->CR1 &= ~SPI_CR1_SPE;//disable spi

  PIN(timer) += period;
}

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct endat_pin_ctx_t *pins = (struct endat_pin_ctx_t *)pin_ptr;
  uint64_t data = df.data;
  if(PIN(timer) > 0.2) {
    PIN(timer) = 0.0;
    int i = 0;
    for(; i < PIN(pos_len); i++) {
      if(data & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      data = data >> 1;
    }
    printf(" ");
    for(; i < PIN(pos_len) + PIN(mpos_len); i++) {
      if(data & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      data = data >> 1;
    }
    printf(" ");
    for(; i < PIN(pos_len) + PIN(mpos_len) + 5; i++) {
      if(data & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      data = data >> 1;
    }
     printf(" ");
    for(; i < PIN(bytes) * 8; i++) {
      if(data & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      data = data >> 1;
    }
    printf("\n");
  }
}

hal_comp_t endat_comp_struct = {
    .name      = "endat",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct endat_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
