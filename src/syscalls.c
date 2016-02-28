#include <reent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "stm32f4xx.h"
#include "usb_cdc.h"

//int __errno;
size_t   __malloc_margin = 256;
char    *__brkval;

int _close(int file) {
	return 0;
}

int _fstat(int file, struct stat *st) {
	return 0;
}

int _isatty(int file) {
	return 1;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _open(const char *name, int flags, int mode) {
	return -1;
}

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr asm ("sp");

void *_sbrk_r(struct _reent *r, ptrdiff_t incr)
{
    extern char end;   // provided by the linker script

    if (__brkval == 0)
        __brkval = &end;

    if (__brkval + incr > (char*)__get_MSP() - __malloc_margin) {
        r->_errno = ENOMEM;
        return (void*)-1;
    }

    void *ret = __brkval;
    __brkval += incr;

    return ret;
}

ssize_t _read(int fd, void *ptr, size_t len)
{
	(void) fd;
	while (!usb_rx_buf.len);

	if (len > usb_rx_buf.len)
		len = usb_rx_buf.len;

	char *c = (char *) ptr;
	for (uint16_t i = 0; i < len; i++)
		rb_getc(&usb_rx_buf, c++);

	return len;
}


int _write(int fd, const char *ptr, int len)
{
	char *c = (char *) ptr;
	(void) fd;
	int sent = 0;

	while (len--) {
		// send a queued byte - copy to usb stack buffer
                APP_Rx_Buffer[APP_Rx_ptr_in++] = *c;
                c++;

                // To avoid buffer overflow
                if (APP_Rx_ptr_in >= APP_RX_DATA_SIZE) {
                    APP_Rx_ptr_in = 0;
                }
		sent++;
	}
	return sent;
}
