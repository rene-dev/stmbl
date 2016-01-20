#include <reent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
//#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "printf.h"

#include "stm32_ub_usb_cdc.h"

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

int _read(int file, char *ptr, int len) {
	if (file != 0) {
		return 0;
	}

   // // Use USB CDC Port for stdin
   // while(!VCP_get_char((uint8_t*)ptr)){};
   //
   // // Echo typed characters
   // VCP_put_char((uint8_t)*ptr);

	return 1;
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

int _write(int file, char *ptr, int len) {
   while(len--){
      UB_VCP_DataTx(*ptr);
      ptr++;
   }
   return len;
}
