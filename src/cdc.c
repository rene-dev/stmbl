/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>

#include "cdc.h"

volatile uint8_t txdmabuf[512];
volatile uint8_t rxdmabuf[512];

struct ringbuf rx_buf = { .buf = (char[RX_QUEUE_SIZE]) {0}, .bufsize = RX_QUEUE_SIZE };
struct ringbuf tx_buf = { .buf = (char[TX_QUEUE_SIZE]) {0}, .bufsize = TX_QUEUE_SIZE };

static void cdc_dmatx(){
   //when dma is not busy, make a tx request
   if(DMA_CNDTR(DMA1,DMA_CHANNEL7) == 0){
      int len = rb_read(&tx_buf,txdmabuf,sizeof(txdmabuf));
      usart_disable_tx_dma(USART2);
      dma_channel_reset(DMA1, DMA_CHANNEL7);
      dma_set_peripheral_address(DMA1, DMA_CHANNEL7, (uint32_t)&USART2_TDR);
      dma_set_memory_address(DMA1, DMA_CHANNEL7, (uint32_t)txdmabuf);
      dma_set_number_of_data(DMA1, DMA_CHANNEL7, len);
      dma_set_read_from_memory(DMA1, DMA_CHANNEL7);
      dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL7);
      dma_set_peripheral_size(DMA1, DMA_CHANNEL7, DMA_CCR_MSIZE_8BIT);
      dma_set_memory_size(DMA1, DMA_CHANNEL7, DMA_CCR_MSIZE_8BIT);
      dma_set_priority(DMA1, DMA_CHANNEL7, DMA_CCR_PL_HIGH);
      dma_disable_peripheral_increment_mode(DMA1, DMA_CHANNEL7);
      dma_enable_channel(DMA1, DMA_CHANNEL7);
      usart_enable_tx_dma(USART2);
   }
}

int cdcacm_tx(void* data, uint32_t len){
   int ret;
   ret = rb_write(&tx_buf, data, len);
   cdc_dmatx();
   return ret;
}

void cdc_poll(){
   int available,bufferpos;
   static int rxpos = 0;
   cdc_dmatx();
   //next received packet will be written to bufferpos
   bufferpos = sizeof(rxdmabuf) - DMA_CNDTR(DMA1,DMA_CHANNEL6);
   //how many packets we have the the rx buffer for processing
   available = (bufferpos - rxpos + sizeof(rxdmabuf)) % sizeof(rxdmabuf);
   
   //TODO: ringbuf dual pointer magic
   for(int i = 0;i<available;i++){
      rb_putc(&rx_buf,rxdmabuf[rxpos++%sizeof(rxdmabuf)]);
   }
   rxpos = rxpos % sizeof(rxdmabuf);
}

int cdcacm_is_connected(){
   return 1;
}

int cdcacm_getline(char *ptr, int len){
   int ret = 0;
   char c;
   if (rx_buf.len == 0){ 
		return 0;
	}

   while(rb_getc(&rx_buf, &c) && ret < len){
      ret++;
      *ptr++ = c;
      if(c == '\n'){
         *--ptr = '\0';
         return ret;
      }
   }
	rb_undo(&rx_buf, ret);
	return 0;
}

void cdc_init(void){
   //USART2
   //pa2 tx
   //pa3 rx
   rcc_periph_clock_enable(RCC_GPIOA);
   rcc_periph_clock_enable(RCC_USART2);
   
   gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
   gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO2);
   
   gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
   gpio_set_af(GPIOA, GPIO_AF7, GPIO3);

   usart_set_baudrate(USART2, 38400);
   usart_set_databits(USART2, 8);
   usart_set_stopbits(USART2, USART_STOPBITS_1);
   usart_set_mode(USART2, USART_MODE_TX_RX);
   usart_set_parity(USART2, USART_PARITY_NONE);
   usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

   usart_enable(USART2);
   rcc_periph_clock_enable(RCC_DMA1);
   
   // rx dma config
   usart_disable_rx_dma(USART2);
   dma_channel_reset(DMA1, DMA_CHANNEL6);
   dma_set_peripheral_address(DMA1, DMA_CHANNEL6, (uint32_t)&USART2_RDR);
   dma_set_memory_address(DMA1, DMA_CHANNEL6, (uint32_t)rxdmabuf);
   dma_set_number_of_data(DMA1, DMA_CHANNEL6, sizeof(rxdmabuf));
   dma_set_read_from_peripheral(DMA1, DMA_CHANNEL6);
   dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL6);
   dma_enable_circular_mode(DMA1, DMA_CHANNEL6);
   dma_set_peripheral_size(DMA1, DMA_CHANNEL6, DMA_CCR_PSIZE_8BIT);
   dma_set_memory_size(DMA1, DMA_CHANNEL6, DMA_CCR_MSIZE_8BIT);
   dma_set_priority(DMA1, DMA_CHANNEL6, DMA_CCR_PL_HIGH);
   dma_disable_peripheral_increment_mode(DMA1, DMA_CHANNEL6);
   
   // start rx dma
   dma_enable_channel(DMA1, DMA_CHANNEL6);
   usart_enable_rx_dma(USART2);
}
