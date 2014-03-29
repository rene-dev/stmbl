#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stlinky.h"


volatile struct stlinky g_stlinky_term = {
	.magic = STLINKY_MAGIC,
	.bufsize = STLINKY_BSIZE,
	.txsize = 0,
	.rxsize = 0
};

int min(int a, int b){
	if(a < b){
		return(a);
	}
	return(b);
}

// returns #send bytes
int stlinky_tx(volatile struct stlinky* st, const char* buf, int siz){
	if(stlinky_todo(st)){
		return(0);
	}
	int sz = min(STLINKY_BSIZE, siz);
	memcpy((char*) st->txbuf, buf, sz);
	st->txsize = (unsigned char) sz;
	return sz;
}

// returns #received bytes
/* TODO: We loose data here if we read less than avaliable */
int stlinky_rx(volatile struct stlinky* st, char* buf, int siz){
	int sz = min(stlinky_avail(st), siz);
	memcpy(buf, (char*) st->rxbuf, sz);
	st->rxsize = 0;
	return sz;
}

// returns #bytes in rx buffer
int stlinky_avail(volatile struct stlinky* st){
	return st->rxsize;
}

// returns #bytes in tx buffer, 0 = can send
int stlinky_todo(volatile struct stlinky* st){
	return(st->txsize);
}

// wait for receive
void stlinky_wait_rx(volatile struct stlinky* st){
	while(!stlinky_avail(st));
}

// wait for send
void stlinky_wait_tx(volatile struct stlinky* st){
	while(stlinky_todo(st));
}
