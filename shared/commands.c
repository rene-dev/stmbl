#include "commands.h"
#include "commandslist.h"
#include <string.h>
#include <stdio.h>

uint32_t call_cmd(char * s){
   char c[64];
   char a[64];
   uint32_t foo = sscanf(s, " %[a-zA-Z_0-9] %[ -~]", c, a);
   switch(foo){
      case 0:
         return(0);
      case 1:
         a[0] = 0;
      case 2:
         for(uint32_t i = 0; i < sizeof(cmd) / sizeof(cmd_t); i++){
            if(!strcmp(cmd[i].name, c)){
               cmd[i].ptr(a);
               return(1);
            }
         }
      default:
         return(0);
   }
   return(0);
}

void listcmd(char * ptr){
  for(uint32_t i = 0; i < sizeof(cmd) / sizeof(cmd_t); i++){
    printf("%s\n",cmd[i].name);
  }
}

COMMAND("help", listcmd);
