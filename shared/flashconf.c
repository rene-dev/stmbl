#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "stm32f3xx_hal.h"
#include "hal.h"

char config[1 * 1024];
const char *config_ro = (char *)0x0801fc00;

void flashloadconf(char *ptr) {
  strncpy(config, config_ro, sizeof(config));
}
COMMAND("flashloadconf", flashloadconf, "load config from flash");

void flashsaveconf(char *ptr) {
  printf("erasing flash page...\n");
  HAL_FLASH_Unlock();
  uint32_t PageError = 0;
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.PageAddress = (uint32_t)config_ro;
  eraseinitstruct.NbPages = 1;
  status = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);

  if(status != HAL_OK) {
    printf("error!\n");
    HAL_FLASH_Lock();
    return;
  }
  printf("saving conf\n");
  int ret = 0;
  for(int i = 0;i<sizeof(config);i+=4){
    uint32_t word = (config[i+0]) + (config[i+1] << 8) + (config[i+2] << 16) + (config[i+3] << 24);
    ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)config_ro + i, word) != HAL_OK;
    if(ret) {
      printf("error writing %i\n", ret);
      break;
    }
  }
  printf("OK %i bytes written\n", sizeof(config));
  HAL_FLASH_Lock();
}
COMMAND("flashsaveconf", flashsaveconf, "save config to flash");

void loadconf(char *ptr) {
  hal_parse(config);
}
COMMAND("loadconf", loadconf, "parse config");

void showconf(char *ptr) {
  printf("%s", config_ro);
}
COMMAND("showconf", showconf, "show config");

void appendconf(char *ptr) {
  printf("adding %s\n", ptr);
  strncat(config, ptr, sizeof(config) - 1);
  strncat(config, "\n", sizeof(config) - 1);
}
COMMAND("appendconf", appendconf, "append string to config");

void deleteconf(char *ptr) {
  config[0] = '\0';
}
COMMAND("deleteconf", deleteconf, "delete config");
