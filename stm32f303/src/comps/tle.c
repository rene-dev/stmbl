#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"

HAL_COMP(tle);

HAL_PIN(pos);
HAL_PIN(d1);
HAL_PIN(d2);
HAL_PIN(d3);
HAL_PIN(d4);

SPI_HandleTypeDef hspi1;

static void hw_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct tle_ctx_t * ctx = (struct tle_ctx_t *)ctx_ptr;
  struct tle_pin_ctx_t * pins = (struct tle_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;
  /* Peripheral clock enable */
  __HAL_RCC_SPI1_CLK_ENABLE();

  /**SPI1 GPIO Configuration    
  PB3     ------> SPI1_SCK
  PB5     ------> SPI1_MOSI 
  */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;//SPI_BAUDRATEPRESCALER_64;//1.125 Mhz
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  HAL_SPI_Init(&hspi1);
}

static void nrt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct tle_ctx_t * ctx = (struct tle_ctx_t *)ctx_ptr;
  struct tle_pin_ctx_t * pins = (struct tle_pin_ctx_t *)pin_ptr;
  uint16_t buf[10];
  buf[0] = 0x8004;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, buf, 1, 100);
  HAL_SPI_Receive(&hspi1, buf, 5, 100);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  PIN(d1) = buf[0];
  PIN(d2) = buf[1];
  PIN(d3) = (float)(buf[2] & 0x7fff - 0x4000) / 16384.0 * 2 * M_PI - M_PI;
  PIN(d4) = buf[3];
}

hal_comp_t tle_comp_struct = {
  .name = "tle",
  .nrt = nrt_func,
  .rt = 0,
  .frt = 0,
  .hw_init = hw_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct tle_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
