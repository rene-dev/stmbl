## F4 ##
### timebase ###
master timer: tim3  
slave timer: tim5  
resolver timer: tim4  

master timer up (1.2Mhz) ->
- adc1/2/3 scan, discon. -> dma tc (240) -> rt (5khz) 
- slave timer up (20khz) -> frt 
- resolver timer oc (5-30khz) + phase shift

### CMD ###
| periph | DMA | stream | channel |
| --- | --- | --- | --- |
| SPI1 TX | 2 | 5 | 3 |
| SPI1 RX | 2 | 2 | 3 |
| USART1 TX | 2 | 7 | 4 |
| USART1 RX | 2 | 5 | 4 |
| TIM2 CH1 |  |  |  |
| TIM2 CH2 |  |  |  |

### FB0 ###
| periph | DMA | stream | channel |
| --- | --- | --- | --- |
| SPI3 TX | 1 | 7 | 0 |
| SPI3 RX | 1 | 2 | 0 |
| USART6 TX | 2 | 6 | 5 |
| USART6 RX | 2 | 1 | 5 |
| TIM4 CH1 | 1 | 0 | 2 |
| TIM4 CH2 | (1) | (3) | (2) |
| TIM4 CH3 | 1 | 7 | 2 |

### FB1 ###
| periph | DMA | stream | channel |
| --- | --- | --- | --- |
| SPI2 TX | 1 | 4 | 0 |
| SPI2 RX | 1 | 3 | 0 |
| USART3 TX | 1 | 4 | 7 |
| USART3 RX | 1 | 1 | 4 |
| TIM1 CH1 | 2 | 3 | 6 |
| TIM1 CH2 | (2) | (2) | (6) |
| TIM1 CH3 | (2) | (6) | (6) |

### HV ###
| periph | DMA | stream | channel |
| --- | --- | --- | --- |
| USART2 TX | 1 | 6 | 4 |
| USART2 RX | 1 | 5 | 4 |

### ADC ###
| periph | DMA | stream | channel |
| --- | --- | --- | --- |
| ADC1 | 2 | 4 | 0 |
| ADC2 | (2) | (3) | (1) |
| ADC3 | 2 | 0 | 2 |


## F3 ##
### timebase ###
pwm timer: tim8  
pwm mode: center aligned flat bottom  

pwm timer up (30khz) ->
- adc1/2/3/4 -> dma (6)
- rt (15khz)

### HV ###
| periph | DMA | channel |
| --- | --- | --- |
| USART3 TX | 1 | 2 |
| USART3 RX | 1 | 3 |

### IO ###
| periph | DMA | channel |
| --- | --- | --- |
| USART1 TX | 1 | 4 |
| USART1 RX | 1 | 5 |

### ADC ###
| periph | DMA | channel |
| --- | --- | --- |
| ADC1 | 1 | 1 |
| ADC2 | (2) | (1) |
| ADC3 | 2 | 5 |
| ADC4 | (2) | (4) |

