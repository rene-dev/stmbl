## stm32f303cbt

| port | pins | periph (remap) |
|---|---|---
| tx | pb6 | usart1_tx, tim4_ch1, i2c1_scl |
| tx_en | pb8 | tim8_ch2, tim16_ch1 |
| rx | pb7 | usart1_rx, tim4_ch2, i2c1_sda |
| rx_en | pb4 | tim3_ch2, tim8_ch2n, tim16_ch1, usart2_rx |
| mosi | pb5 | spi1_mosi |
| sck | pb3 | spi1_sck, tim2_ch2, uart2_tx |
| csq | pa15 | spi1_nss, tim2_ch1, uart2_rx |
| led0 | pc13 | |
| led1 | pc14 | |
| led2 | pc15 | |
| usb_dp | pa12 | |
| usb_dm | pa11 | |
| usb_en | pb2 | |
| swdio | pa13 | |
| swclk | pa14 | |
| nrst | nrst | |
| dclink | pa0 | adc1_in1 |
| temp | pa1 | adc1_in2 |
| in0 | pa2 | adc1_in3 |
| bemf1a | pa3 | adc1_in4 |
| bemf2a | pa4 | adc2_in1 |
| in1 | pa5 | adc2_in2 |
| opamp2_out | pa6 | adc2_in3 |
| shunt_low | pa7 | adc2_in4, opamp2_in |
| shunt_a | pb0 | adc3_in12, opamp3_in |
| opamp3_out | pb1 | adc3_in1 |
| shunt_b | pb11 | opamp4_in |
| opamp4_out | pb12 | adc4_in3 |
| out0 | pb10 | tim2_ch3 |
| out1 | pb9 | tim17_ch1, tim8_ch3 |
| pwm_1a | pa8 | tim1_ch1 |
| pwm_2a | pb13 | tim1_ch1n |
| a_en | pb15 | tim1_ch3n |
| pwm_1b | pa9 | tim1_ch2 |
| pwm_2b | pb14 | tim1_ch2n |
| b_en | pa10 | tim1_ch3 |
