## stm32f405vgt

| port | pins | periph (remap) |
| --- | --- | --- |
| cmd_d12 | pb4, pa10, pa15 | spi1_miso, usart1_rx, tim2_ch1, (tim3_ch1, tim1_ch3, spi3_miso) |
| cmd_e12 | pb6 | |
| cmd_d36 | pb3, pa8, pa0 | spi1_sck, usart1_ck, tim2_ch2, uart4_tx (tim1_ch1, spi3_sck, tim5_ch1) |
| cmd_e36 | pb7 | |
| cmd_d54 | pb5, pa9 | spi1_mosi, usart1_tx, (tim3_ch2, tim1_ch2, spi3_mosi) |
| cmd_e54 | pb9 | tim11_ch1 |
| cmd_d78 | pb8 | tim10_ch1 |
| cmd_e78 | pb2 | |
| cmd_l1 | pd7 | |
| cmd_l2 | pd6 | |
| fb0_d12 | pc11, pc7, pd12 | spi3_miso, usart6_rx, tim4_ch1, (uart4_rx, usart3_rx, tim3_ch2, tim8_ch2) |
| fb0_e12 | pd11 | |
| fb0_ain12 | pa6 | adc1_in6 |
| fb0_d36 | pc10, pc8, pd13 | spi3_sck, usart6_ck, tim4_ch2, (uart4_tx, usart3_tx, tim3_ch3, tim8_ch3) |
| fb0_e36 | pd10 | |
| fb0_ain36 | pa7 | adc2_in7 |
| fb0_d54 | pc12, pc6, pd14 | spi3_mosi, usart6_tx, tim4_ch3 (uart5_tx, usart3_ck, tim3_ch1, tim8_ch1) |
| fb0_e54 | pd15 | tim4_ch4 |
| fb0_ain78 | pc2 | adc3_in12 |
| fb0_l1 | pd8 | |
| fb0_l2 | pd9 | |
| fb1_d12 | pb14, pb11, pe9 | spi2_miso, usart3_rx, tim1_ch1, (tim1_ch2n, tim2_ch4) |
| fb1_e12 | pe10 | |
| fb1_ain12 | pa4 | adc1_in4 |
| fb1_d36 | pb13, pb12, pe11 | spi2_sck, usart3_ck, tim1_ch2, (tim1_ch1n) |
| fb1_e36 | pe12 | tim1_ch3n |
| fb1_ain36 | pa5 | adc2_in5 |
| fb1_d54 | pb15, pb10, pe13 | spi2_mosi, usart3_tx, tim1_ch3, (tim12_ch2, tim1_ch3n, tim8_ch3n, spi2_ck, tim2_ch3) |
| fb1_e54 | pe14 | tim1_ch4 |
| fb1_ain78 | pc3 | adc3_in13 |
| fb1_l1 | pe7 | |
| fb1_l2 | pe8 | |
| io_cantx | pd1 | can1_tx |
| io_canrx | pd0 | can1_rx |
| io_spimosi | pb5, pa9 | spi1_mosi, usart1_tx, (tim3_ch2, tim1_ch2, spi3_mosi) |
| io_spimiso | pb4, pa10, pa15 | spi1_miso, usart1_rx, tim2_ch1, (tim3_ch1, tim1_ch3, spi3_miso) |
| io_spisck | pb3, pa8, pa0 | spi1_sck, usart1_ck, tim2_ch2, uart4_tx (tim1_ch1, spi3_sck, tim5_ch1) |
| io_swclk | pa14 | sw_clk |
| io_swdio | pa13 | sw_dio |
| io_rst | nrst | nrst |
| io_ain0 | pc0 | adc3_in10 |
| io_ain1 | pc1 | adc3_in11 |
| io_ain_led0 | pe0 | |
| io_ain_led1 | pe1 | |
| io_led_red | pd3 | |
| io_led_yellow | pd4 | |
| io_led_green | pd5 | |
| io_fan | pe3 | |
| io_out0 | pe4 | |
| io_out1 | pe5 | tim9_ch1 |
| io_out2 | pe6 | tim9_ch2 |
| hv_tx | pa2 | uart2_tx |
| hv_rx | pa3 | uart2_rx |
| usb_dp | pa12 | usb_dp, can1_tx |
| usb_dm | pa11 | usb_dm, can1_rx |
| 24V | pb1 |  |
| 5V | pa1 |  |

## stm32f303cbt

| port | pins | periph (remap) |
|---|---|---
| hv_tx | pb10 | usart3_tx |
| hv_rx | pb11 | usart3_rx |
| led | pa8 | (tim1_ch1) |
| usb_dp | pa12 | |
| usb_dm | pa11 | |
| io_tx | pa9 | usart1_tx, (tim1_ch2, tim2_ch3, i2c2_scl)|
| io_rx | pa10 | usart1_rx, (tim1_ch3, tim2_ch4, i2c2_sda)|
| swdio | pa13 ||
| swclk | pa14 ||
| nrst | nrst ||
| hv_en | pa15 | (tim2_ch1) |
| hv_fault | pb7 | (tim8_bkin) |
| hv_temp | pb14 | adc4_in4 |
| hv_u | pb6 | tim8_ch1 |
| hv_nu | pb3 | tim8_ch1n |
| hv_v | pb8 | tim8_ch2 |
| hv_nv | pb4 | tim8_ch2n |
| hv_w | pb9 | tim8_ch3 |
| hv_nw | pb5 | tim8_ch3n |
| mot_temp | pb15 | adc4_in5 |
| u | pb13 | adc3_in5 |
| v | pa5 | adc2_in2 |
| w | pa3 | adc1_in4 |
| iu | pa7 | opamp2_in+, comp2_in+, (adc2_in4) |
| iv | pb0| opamp3_in+, comp4_in+, (adc3_in12) |
| iw | pa1 | opamp1_in+, comp1_in+, (adc1_in2) |
| iu_out | pa6 | opamp2_out, adc2_in3 |
| iv_out | pb1 | opamp3_out, adc3_in1 |
| iw_out | pa2 | opamp1_out, adc1_in3 |
| i_ref | pa4 | dac_out1 |
| hv | pb12 | adc4_in3 |
| brk | pb2 ||
