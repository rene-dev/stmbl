#include "commands.h"
#include "commandslist.h"
#include <string.h>
#include <stdio.h>

uint32_t call_cmd(char *s) {
  char c[64];
  char a[64];
  uint32_t matches = sscanf(s, " %[a-zA-Z_0-9] %[ -~]", c, a);
  switch(matches) {
    case 0:
      return (0);
    case 1:
      a[0] = 0;
      /* FALLTHRU */
    case 2:
      for(uint32_t i = 0; i < sizeof(cmd) / sizeof(cmd_t); i++) {
        if(!strcmp(cmd[i].name, c)) {
          cmd[i].ptr(a);
          return (1);
        }
      }
      /* FALLTHRU */
    default:
      return (0);
  }
  return (0);
}

void listcmd(char *ptr) {
  for(uint32_t i = 0; i < sizeof(cmd) / sizeof(cmd_t); i++) {
    printf("%s: %s\n", cmd[i].name, cmd[i].doc);
  }
}

COMMAND("help", listcmd, "print this");

void uptime(char *ptr) {
  extern uint64_t systime;
  uint32_t min = systime/1000/60;
  uint32_t sec = (systime/1000)%60;
  uint32_t hr = min/60;
  min = min%60;
  printf("up %02lu:%02lu:%02lu\n",hr,min,sec);
}

COMMAND("uptime", uptime, "display uptime");
