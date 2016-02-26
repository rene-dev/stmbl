#include <reent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "cdc.h"

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

uint32_t __get_MSP(void)
{
  register uint32_t result;

  asm volatile ("MRS %0, msp\n" : "=r" (result) );
  return(result);
}

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
   return (void*)-1;
}

ssize_t _read(int fd, void *ptr, size_t len)
{
	(void) fd;
	//while (!usb_rx_buf.len);

	//if (len > usb_rx_buf.len)
	//	len = usb_rx_buf.len;

	char *c = (char *) ptr;
	for (uint16_t i = 0; i < len; i++){}
		//rb_getc(&usb_rx_buf, c++);

	return len;
}

int _write(int file, char *ptr, int len)
{
	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
      return cdcacm_tx(ptr,len);
	}
	errno = EIO;
	return -1;
}
