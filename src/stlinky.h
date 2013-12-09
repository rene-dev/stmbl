#ifndef STLINKY_H
#define STLINKY_H
#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h>

#define STLINKY_MAGIC 0xDEADF00D
#define STLINKY_BSIZE 132

/* NOTE: Since rxsize/txsize will be always one
   AXI transaction these will be hopefully atomic
   Therefore we use them as a dumb locking mechanism
 */

struct stlinky {
	uint32_t magic; /* [3:0] */
	unsigned char bufsize; /* 4 */
	char txsize; /* 5 */
	char rxsize; /* 6 */
	char reserved; /* 7 */
	char txbuf[STLINKY_BSIZE];
	char rxbuf[STLINKY_BSIZE];
} __attribute__ ((packed));


// send bytes
// returns #send bytes
int stlinky_tx(volatile struct stlinky* st, const char* buf, int siz);

// receive bytes
// returns #received bytes
int stlinky_rx(volatile struct stlinky* st, char* buf, int siz);

// returns #bytes in rx buffer
int stlinky_avail(volatile struct stlinky* st);

// returns #bytes in tx buffer, 0 = can send
int stlinky_todo(volatile struct stlinky* st);

// wait for receive
void stlinky_wait_rx(volatile struct stlinky* st);

// wait for send
void stlinky_wait_tx(volatile struct stlinky* st);

extern volatile struct stlinky g_stlinky_term;

#endif

