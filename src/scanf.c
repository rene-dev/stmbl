//
//  scanf.c
//  test
//
//  Created by crinq on 05.12.13.
//  Copyright (c) 2013 Rene Hopf. All rights reserved.
//

#include "scanf.h"
#include "usb_cdc.h"

int scanf_(const char *format, ...){
   int ret = 0;
   char rx_buf[APP_TX_BUF_SIZE];
   va_list arg;
   if (USB_CDC_is_connected()) {
      if (USB_VCP_get_string(rx_buf)) {
         va_start(arg, format);
         ret = vfsscanf_(rx_buf, format, arg);
         va_end(arg);
      } else {
         ret = -1;
      }
   }
   return(ret);
}

int sscanf_(const char *buf, const char *format, ...){
   int ret = 0;

   va_list arg;
   va_start(arg, format);
   ret = vfsscanf_(buf, format, arg);
   va_end(arg);
   return(ret);
}


int isBinDigit(char c){
   return(c == '0' || c == '1');
}

int isChar(char c){
   return((c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a') || isDecDigit(c));
}

int isNameChar(char c){
   return(isChar(c) || c == '_' || c == '-' || c == '.');
}

int isDecDigit(char c){
   return(c >= '0' && c <= '9');
}

int isHexDigit(char c){
   return((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')  || (c >= 'a' && c <= 'f'));
}

int isWhitespace(char c){
   return(c == ' ' || c == '\t');
}

int vfsscanf_(const char *buf, const char *format, va_list arg){
   int buffer_pos = 0;
   int format_pos = 0;
   int string_pos = 0;
   int ret = 0;
   int found = 0;

   int *i;
   float *f;
   char *c;
   int invert;
   float tf;

   int state = 0;  // 0 = last char = normal char
   // 1 = last char = '%'
   // 2 = last char = '\'

   while(format[format_pos]){
      found = 0;
      switch(state){
         case 0:
            switch(format[format_pos]){
               case '%':
                  state = 1;
                  format_pos++;
                  continue;
                  break;

               case '\'':
                  state = 2;
                  format_pos++;
                  continue;
                  break;

               case ' ': // parse whithespaces
                  while(isWhitespace(buf[buffer_pos])){
                     buffer_pos++;
                     found++;
                  }
                  found++;
                  break;

               default:
                  if(format[format_pos] != buf[buffer_pos++]){
                     return(ret);
                  }
                  found++;
            }
            break;

         case 1:
            switch(format[format_pos]){
               case '%':
                  if(buf[buffer_pos++] != '%'){
                     return(ret);
                  }
                  found++;
                  break;

               case 'c':
                  c = va_arg(arg, char *);
                  *c = buf[buffer_pos++];
                  found++;
                  break;

               case 's':
                  string_pos = 0;
                  c = va_arg(arg, char *);
                  //while(isChar(buf[buffer_pos])){
                  while(!isWhitespace(buf[buffer_pos])){
                     c[string_pos] = buf[buffer_pos++];
                     string_pos++;
                     c[string_pos] = '\0';
                     found++;
                  }
                  break;

               case 'N':
                  string_pos = 0;
                  c = va_arg(arg, char *);
                  //while(isChar(buf[buffer_pos])){
                  while(isNameChar(buf[buffer_pos])){
                     c[string_pos] = buf[buffer_pos++];
                     string_pos++;
                     c[string_pos] = '\0';
                     found++;
                  }
                  break;

               case 'i':
                  i = va_arg(arg, int *);
                  *i = 0;
                  invert = 1;
                  if(buf[buffer_pos] == '-'){
                     invert = -1;
                     buffer_pos++;
                  }
                  else if(buf[buffer_pos] == '+'){
                     buffer_pos++;
                  }
                  while(isDecDigit(buf[buffer_pos])){
                     *i *= 10;
                     *i += buf[buffer_pos++] - '0';
                     found++;
                  }
                  *i *= invert;
                  break;

               case 'b':
                  i = va_arg(arg, int *);
                  *i = 0;
                  invert = 1;
                  if(buf[buffer_pos] == '-'){
                     invert = -1;
                     buffer_pos++;
                  }
                  else if(buf[buffer_pos] == '+'){
                     buffer_pos++;
                  }
                  if(!(buf[buffer_pos] == 'b' || buf[buffer_pos] == 'B')){
                     return(ret);
                  }
                  buffer_pos++;
                  while(isBinDigit(buf[buffer_pos])){
                     *i *= 2;
                     *i += buf[buffer_pos++] - '0';
                     found++;
                  }
                  *i *= invert;
                  break;

               case 'h':
                  i = va_arg(arg, int *);
                  *i = 0;
                  invert = 1;
                  if(buf[buffer_pos] == '-'){
                     invert = -1;
                     buffer_pos++;
                  }
                  else if(buf[buffer_pos] == '+'){
                     buffer_pos++;
                  }
                  if(buf[buffer_pos++] != '0'){
                     return(ret);
                  }
                  if(buf[buffer_pos++] != 'x'){
                     return(ret);
                  }
                  while(isHexDigit(buf[buffer_pos])){
                     *i *= 16;
                     if(buf[buffer_pos] < 'A'){
                        *i += buf[buffer_pos++] - '0';
                     }
                     else if(buf[buffer_pos] < 'a'){
                        *i += buf[buffer_pos++] - 'A' + 10;
                     }
                     else{
                        *i += buf[buffer_pos++] - 'a' + 10;
                     }
                     found++;
                  }
                  *i *= invert;
                  break;

               case 'f':
                  f = va_arg(arg, float *);
                  *f = 0;
                  tf = 10;
                  invert = 1;
                  if(buf[buffer_pos] == '-'){
                     invert = -1;
                     buffer_pos++;
                  }
                  else if(buf[buffer_pos] == '+'){
                     buffer_pos++;
                  }
                  while(isDecDigit(buf[buffer_pos])){
                     *f *= 10;
                     *f += buf[buffer_pos++] - '0';
                     found++;
                  }
                  if(buf[buffer_pos] == '.'){
                     buffer_pos++;
                     while(isDecDigit(buf[buffer_pos])){
                        *f += (buf[buffer_pos++] - '0') / tf;
                        tf *= 10;
                        found++;
                     }
                  }
                  *f *= invert;
                  break;

               default:
                  return(ret);
                  break;
            }
            state = 0;
            break;

               case 2:
            switch(format[format_pos]){
               case '\'':
                  if(buf[buffer_pos++] != '\''){
                     return(ret);
                  }
                  found++;
                  break;

               case 'n':
                  if(buf[buffer_pos++] != '\n'){
                     return(ret);
                  }
                  found++;
                  break;

               default:
                  return(ret);
                  break;
            }
            state = 0;
            break;

               default:
            break;
            }

            format_pos++;
            if(!found){
               return(ret);
            }
            ret++;
            }
            return(ret);
      }
