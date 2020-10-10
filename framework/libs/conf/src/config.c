#include "config.h"
#include "defines.h"
#include "string.h"
#include "commands.h"

extern config_template_t config_templates[];
extern const uint32_t num_of_config_templates;

void link(char *ptr) {
  for(int i = 0; i < num_of_config_templates; i++) {
    if(strlen(ptr) == strlen(config_templates[i].name) && !strncmp(config_templates[i].name, ptr, strlen(ptr))) {
      hal_parse(config_templates[i].config);
      return;
    }
  }
}

COMMAND("link", link, "load config template");

void show_config_template(char *ptr) {
  for(int i = 0; i < num_of_config_templates; i++) {
    if(strlen(ptr) == strlen(config_templates[i].name) && !strncmp(config_templates[i].name, ptr, strlen(ptr))) {
      printf("\nHAL config template: %s\n", config_templates[i].name);
      printf("%s\n", config_templates[i].config);
      return;
    }
  }
  printf("HAL config template %s not found\n", ptr);
  printf("available templates:\n");
  for(int i = 0; i < num_of_config_templates; i++) {
    printf(" - %s\n", config_templates[i].name);
  }
}

COMMAND("show_config", show_config_template, "show config templates");
