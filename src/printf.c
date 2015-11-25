
/*
+=============================================================================+
| includes
+=============================================================================+
*/

#include <stdarg.h>     // (...) parameter handling
#include <math.h>
#include "stm32_ub_usb_cdc.h"

#include "printf.h"
#define PRINTF_BSIZE 128
#define PRINTF_FLENGTH 6


/*
+=============================================================================+
| local declarations
+=============================================================================+
*/
int vfprintf_(char *, const char *format, va_list arg); //generic print


/*
+=============================================================================+
| global functions
+=============================================================================+
*/
int printf_(const char *format, ...)
{
	va_list arg;
	char buffer[PRINTF_BSIZE];
	int buffer_pos;

	va_start(arg, format);
	buffer_pos = vfprintf_(buffer, format, arg);
	va_end(arg);

	buffer[buffer_pos] = '\0';

	if(UB_USB_CDC_GetStatus()==USB_CDC_CONNECTED){
		UB_USB_CDC_SendString(buffer, NONE);
	}

	return 0;
}



/*
+=============================================================================+
| local functions
+=============================================================================+
*/


/*--------------------------------------------------------------------------------+
 * vfprintf_()
 * Prints a string to buffer. Supports %s, %c, %d, %ld %ul %02d %i %x  %lud  and %%
 *     - partly supported: long long, float (%ll %f, %F, %2.2f)
 *     - not supported: double float and exponent (%e %g %p %o \t)
 * returns length of string
 * limits string length to PRINTF_BSIZE
 *--------------------------------------------------------------------------------+
*/
int vfprintf_(char *buffer, const char* str,  va_list arp){
	int buffer_pos = 0; // position in output buffer
	int input_pos = 0; // position in input buffer

	int state = 0;  // 0 = last char = normal char
									// 1 = last char = '%'
									// 2 = last char = '\'

	int i = 0; // number to print
	uint32_t ui = 0; // number to print
	float f = 0; // float to print
	char tmp[20]; // tmp string for numbers
	int tmp_pos = 0; // pos in tmp
	char *s = 0; // string to print
	unsigned char float_length = PRINTF_FLENGTH;

	while(str[input_pos] != '\0' && buffer_pos < PRINTF_BSIZE){
		switch(state){
			case 0: // last = normal
				switch(str[input_pos]){
					case '%':
						state = 1;
					break;

					case '\'':
						state = 2;
					break;

					default:
						buffer[buffer_pos++] = str[input_pos];
				}
				input_pos++;
			break;

			case 1: // last = '%'
				switch(str[input_pos++]){
					case '%': // "%%"
						buffer[buffer_pos++] = '%';
					break;

					case 'c': // "%c"
						buffer[buffer_pos++] = (char)va_arg(arp, int);
					break;

					case 's': // "%s"
						s = va_arg(arp, char*);
						while(*s){
							buffer[buffer_pos++] = *(s++);
						}
					break;

					case 'i': // "%i"
						tmp_pos = 0;
						i = va_arg(arp, int);

						if(i < 0){
							buffer[buffer_pos++] = '-';
							i *= -1;
						}

						do{
							tmp[tmp_pos++] = i % 10 + '0';
							i /= 10;

						}
						while(i > 0);

						while(tmp_pos--){
							buffer[buffer_pos++] = tmp[tmp_pos];
						}
					break;

					case 'p': // "%p"
						tmp_pos = 0;
						ui = va_arg(arp, uint32_t);

						do{
							tmp[tmp_pos++] = ui % 10 + '0';
							ui /= 10;
						}
						while(ui > 0);

						while(tmp_pos--){
							buffer[buffer_pos++] = tmp[tmp_pos];
						}
					break;

					case 'b': // "%b"
						tmp_pos = 0;
						i = va_arg(arp, int);

						if(i < 0){
							buffer[buffer_pos++] = '-';
							i *= -1;
						}

						do{
							tmp[tmp_pos++] = i % 2 + '0';
							i /= 2;

						}
						while(i > 0);

						buffer[buffer_pos++] = 'b';

						while(tmp_pos--){
							buffer[buffer_pos++] = tmp[tmp_pos];
						}
					break;

					case 'h': // "%h"
						tmp_pos = 0;
						i = va_arg(arp, int);

						if(i < 0){
							buffer[buffer_pos++] = '-';
							i *= -1;
						}

						do{
							if(i % 16 < 10){
								tmp[tmp_pos++] = i % 16 + '0';
							}
							else{
								tmp[tmp_pos++] = i % 16 + 'A' - 10;
							}
							i /= 16;

						}
						while(i > 0);

						buffer[buffer_pos++] = '0';
						buffer[buffer_pos++] = 'x';

						while(tmp_pos--){
							buffer[buffer_pos++] = tmp[tmp_pos];
						}
					break;

					case 'f': // "%f"
						tmp_pos = 0;
						f = va_arg(arp, double); // change to float for stm32

						if(isnan(f) || f != f){
							buffer[buffer_pos++] = 'N';
							buffer[buffer_pos++] = 'a';
							buffer[buffer_pos++] = 'N';
							break;
						}
						if(f == INFINITY){
							buffer[buffer_pos++] = '+';
							buffer[buffer_pos++] = 'i';
							buffer[buffer_pos++] = 'n';
							buffer[buffer_pos++] = 'f';
							break;
						}
						if(f == -INFINITY){
							buffer[buffer_pos++] = '-';
							buffer[buffer_pos++] = 'i';
							buffer[buffer_pos++] = 'n';
							buffer[buffer_pos++] = 'f';
							break;
						}


						if(f < 0){
							buffer[buffer_pos++] = '-';
							f *= -1;
						}

						i = f;

						do{
							tmp[tmp_pos++] = i % 10 + '0';
							i /= 10;
						}
						while(i > 0);

						while(tmp_pos--){
							buffer[buffer_pos++] = tmp[tmp_pos];
						}

						buffer[buffer_pos++] = '.';

						f -= i;
						tmp_pos = float_length - 1;
						for(int j = 0; j < float_length; j++){
							f *= 10;
							tmp[tmp_pos--] = ((int)f) % 10 + '0';
						}
						tmp_pos = float_length;
						while(tmp_pos--){
							buffer[buffer_pos++] = tmp[tmp_pos];
						}
					break;

					default:
					break;
				}
				state = 0;
			break;

			case 2: // last = '\'
				switch(str[input_pos++]){
					case 'n': // "\n"
						buffer[buffer_pos++] = '\n';
						state = 0;
					break;

                    case 't': // "\t"
						buffer[buffer_pos++] = '\t';
						state = 0;
					break;

                    case 'r': // "\t"
						buffer[buffer_pos++] = '\r';
						state = 0;
					break;

					case '\'': // "\'"
						buffer[buffer_pos++] = '\'';
						state = 0;
					break;

					case '"': // "\""
						buffer[buffer_pos++] = '\"';
						state = 0;
					break;

					case '\\': // "\\"
						buffer[buffer_pos++] = '\\';
						state = 0;
					break;

					default:
					break;
				}
			break;
		}
	}
	return(buffer_pos);
}
