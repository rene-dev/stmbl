//driver for TLE5012 GMR-Based Angular Sensor
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"

HAL_COMP(tle);

HAL_PIN(pos);
HAL_PIN(error);

SPI_HandleTypeDef hspi1;

#pragma pack(1)
typedef union{
   struct {
      uint16_t nd   :4;//4-bit Number of data words
      uint16_t addr :6;//6-bit Address
      uint16_t upd  :1;//Update register access 0: Access to current values 1: Access to updated values
      uint16_t lock :4;//4-bit lock value 0000B: Default operating access for addresses 0x00:0x04, 1010B: Config-access for addresses 0x05:0x11
      uint16_t rw   :1;//Read-write 0:Write 1:Read
   };
   uint16_t word;
} tle_cmd_t;

typedef union{
   struct {
      uint8_t crc         :8;//Cyclic Redundancy Check
      uint8_t resp        :4;//Sensor number response indicator
      uint8_t angle_valid :1;//Valid angle value 1: Angle value valid
      uint8_t if_err      :1;//interface error 1: No error
      uint8_t sys_err     :1;//system error 1: No error
      uint8_t rst         :1;//Indication of chip reset 0: Reset occurred 1: No reset
   };
   uint16_t word;
} tle_safety_t;
//TODO: alles hier drunter drehen
typedef union{
   struct {
      uint8_t rd_st     :1;//Read Status
      uint8_t s_nr      :2;//Slave Number
      uint8_t no_gmr_a  :1;//No valid GMR Angle Value
      uint8_t no_gmr_xy :1;//No valid GMR XY Values
      uint8_t s_rom     :1;//Status ROM
      uint8_t s_adct    :1;//Status ADC-Test
      uint8_t res       :1;//reserved
      uint8_t s_magol   :1;//Status Magnitude Out of Limit
      uint8_t s_xyol    :1;//Status X,Y Data Out of Limit
      uint8_t s_ov      :1;//Status Overflow
      uint8_t s_dspu    :1;//Status Digital Signal Processing Unit
      uint8_t s_fuse    :1;//Status Fuse CRC
      uint8_t s_vr      :1;//Status Voltage Regulator
      uint8_t s_wd      :1;//Status Watchdog
      uint8_t s_rst     :1;//Status Reset
   };
   uint16_t word;
} tle_reg_stat_t;

typedef union{
   struct {
      uint8_t res1       :6;//Reserved
      uint8_t as_adct    :1;//Enable ADC Testvector Check
      uint8_t res2       :1;//Reserved
      uint8_t as_vec_mag :1;//Activation of Magnitude Check
      uint8_t as_vec_xy  :1;//Activation of X,Y out of limit Check
      uint8_t as_ov      :1;//Enable of DSPU Overflow Check
      uint8_t as_dspu    :1;//Activation DSPU BIST
      uint8_t as_fuse    :1;//Activation Fuse CRC
      uint8_t as_vr      :1;//Enable Voltage Regulator Check
      uint8_t as_wd      :1;//Enable DSPU Watchdog-HW-Reset
      uint8_t as_rst     :1;//Activation of Hardware Reset
   };
   uint16_t word;
} tle_reg_acstat_t;

typedef union{
   struct {
      uint8_t  fir_md     :2;//Filter Decimation Setting (Update Rate Setting)
      uint16_t res        :9;//Reserved
      uint8_t  clk_sel    :1;//Clock Source Select
      uint8_t  ssc_od     :1;//SSC Interface 0: Push-pull 1: Open drain
      uint8_t  dsp_hold   :1;//Hold DSPU Operation
      uint8_t  iif_mod    :2;//Incremental Interface Mode
   };
   uint16_t word;
} tle_reg_mod1_t;

#define TLE_REG_STAT   0x00 //Status Register
#define TLE_REG_ACSTAT 0x01 //Activation Status Register
#define TLE_REG_AVAL   0x02 //Angle Value Register
#define TLE_REG_ASPD   0x03 //Angle Speed Register
#define TLE_REG_AREV   0x04 //Angle Revolution Register
#define TLE_REG_FSYNC  0x05 //Frame Synchronization Register
#define TLE_REG_MOD_1  0x06 //Interface Mode1 Register
#define TLE_REG_SIL    0x07 //SIL Register
#define TLE_REG_MOD_2  0x08 //Interface Mode2 Register
#define TLE_REG_MOD_3  0x09 //Interface Mode3 Register
#define TLE_REG_OFFX   0x0A //Offset X
#define TLE_REG_OFFY   0x0B //Offset Y
#define TLE_REG_SYNCH  0x0C //Synchronicity
#define TLE_REG_IFAB   0x0D //IFAB Register
#define TLE_REG_MOD_4  0x0E //Interface Mode4 Register
#define TLE_REG_TCO_Y  0x0F //Temperature Coefficient Register
#define TLE_REG_ADC_X  0x10 //X-raw value
#define TLE_REG_ADC_Y  0x11 //Y-raw value
#define TLE_REG_CNT    0x20 //IIF Counter value

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;//SPI_BAUDRATEPRESCALER_64;//1.125 Mhz
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  HAL_SPI_Init(&hspi1);
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct tle_ctx_t * ctx = (struct tle_ctx_t *)ctx_ptr;
  struct tle_pin_ctx_t * pins = (struct tle_pin_ctx_t *)pin_ptr;
  uint16_t buf[10];
  tle_cmd_t tle_cmd;
  tle_safety_t tle_safety;
  tle_cmd.word = 0x0000;
  tle_cmd.rw = 1;
  tle_cmd.addr = 2;
  tle_cmd.nd = 1;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &tle_cmd.word, 1, 100);
  HAL_SPI_Receive(&hspi1, buf, tle_cmd.nd + 1, 100);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  PIN(pos) = (float)(buf[0] & 0x7fff - 0x4000) / 16384.0 * 2 * M_PI - M_PI;
  tle_safety.word = buf[1];
  PIN(error) = !tle_safety.angle_valid;
}

hal_comp_t tle_comp_struct = {
  .name = "tle",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .hw_init = hw_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct tle_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
