## stm32f405vgt

| port | pins | periph (remap) |
| --- | --- | --- |
| cmd_d12 | pb4, pa10, pa15 | spi1_miso, usart1_rx, tim2_ch1, (tim3_ch1, tim1_ch3, spi3_miso) |
| cmd_e12 | pb | |
| cmd_d36 | pb3, pa8 | spi1_sck, usart1_ck, tim2_ch2, (tim1_ch1, spi3_sck) |
| cmd_e36 | pb | |
| cmd_d54 | pb5, pa9 | spi1_mosi, usart1_tx, (tim3_ch2, tim1_ch2, spi3_mosi) |
| cmd_e54 | pb0/pb1 | tim3_ch3/tim3_ch4 |
| cmd_d78 | pb0/pb1 | tim3_ch3/tim3_ch4 |
| cmd_e78 | pb | |
| cmd_l1 | | |
| cmd_l2 | | |
| fb0_d12 | pc11, pc7, pd12 | spi3_miso, usart6_rx, tim4_ch1, (uart4_rx, usart3_rx, tim3_ch2, tim8_ch2) |
| fb0_e12 | pd | |
| fb0_ain12 | | |
| fb0_d36 | pc10, pc8, pd13 | spi3_sck, usart6_ck, tim4_ch2, (uart4_tx, usart3_tx, tim3_ch3, tim8_ch3) |
| fb0_e36 | pd | |
| fb0_ain36 | | |
| fb0_d54 | pc12, pc6, pd14 | spi3_mosi, usart6_tx, tim4_ch3 (uart5_tx, usart3_ck, tim3_ch1, tim8_ch1) |
| fb0_e54 | pd15 | tim4_ch4 |
| fb0_pwm78 | | |
| fb0_en78 | | |
| fb0_ain78 | | |
| fb0_l1 | | |
| fb0_l2 | | |
| fb1_d12 | pb14, pb11, pe9 | spi2_miso, usart3_rx, tim1_ch1, (tim1_ch2n, tim2_ch4) |
| fb1_e12 | pe | |
| fb1_ain12 | | |
| fb1_d36 | pb13, pb12, pe11 | spi2_sck, usart3_ck, tim1_ch2, (tim1_ch1n) |
| fb1_e36 | pe | |
| fb1_ain36 | | |
| fb1_d54 | pb15, pb10, pe13 | spi2_mosi, usart3_tx, tim1_ch3, (tim12_ch2, tim1_ch3n, tim8_ch3n, spi2_ck, tim2_ch3) |
| fb1_e54 | pe14 | tim1_ch4 |
| fb1_pwm78 | | |
| fb1_en78 | | |
| fb1_ain78 | | |
| fb1_l1 | | |
| fb1_l2 | | |
| io_cantx | pb9 | can1_tx, i2c1_sda, tim11_ch1, tim4_ch4 |
| io_canrx | pb8 | can1_rx, i2c1_scl, tim10_ch1, tim4_ch3 |
| io_spimosi | pb5, pa9 | spi1_mosi, usart1_tx, (tim3_ch2, tim1_ch2, spi3_mosi) |
| io_spimiso | pb4, pa10, pa15 | spi1_miso, usart1_rx, tim2_ch1, (tim3_ch1, tim1_ch3, spi3_miso) |
| io_spisck | pb3, pa8 | spi1_sck, sw_o usart1_ck, tim2_ch2, (tim1_ch1, spi3_sck) |
| io_tx | pd5 | uart2_tx |
| io_rx | pd6 | uart2_rx |
| io_swclk | pa14 | sw_clk |
| io_swdio | pa13 | sw_dio |
| io_rst | nrst | nrst |
| io_ain0 | | |
| io_ain1 | | |
| io_fan | | |
| io_brake | | |
| io_out0 | | |
| io_out1 | | |
| hv_tx | pa0 | uart4_tx |
| hv_rx | pa1 | uart4_rx |
| usb_dp | pa12 | usb_dp, can1_tx |
| usb_dm | pa11 | usb_dm, can1_rx |

## stm32f303cbt

| port | pins | periph (remap) |
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
| hv_temp |||
| hv_u | pb6 | tim8_ch1 |
| hv_nu | pb3 | tim8_ch1n |
| hv_v | pb8 | tim8_ch2 |
| hv_nv | pb4 | tim8_ch2n |
| hv_w | pb9 | tim8_ch3 |
| hv_nw | pb5 | tim8_ch3n |
| mot_temp |||
| u |||
| v |||
| w |||
| iu |||
| iv |||
| iw |||
| 15v |||
| hv |||
