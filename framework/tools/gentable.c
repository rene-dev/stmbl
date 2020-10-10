#include <stdio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

#include "../inc/sserial.h"
#include "../shared/defines.h"

memory_t memory;
uint8_t *heap_ptr;
uint32_t timeout;
pd_table_t pd_table;

uint16_t add_pd(char *name_string, char *unit_string, uint8_t data_size_in_bits, uint8_t data_type, uint8_t data_dir, float param_min, float param_max) {
  process_data_descriptor_t pdr;
  pdr.record_type    = RECORD_TYPE_PROCESS_DATA_RECORD;
  pdr.data_size      = data_size_in_bits;
  pdr.data_type      = data_type;
  pdr.data_direction = data_dir;
  pdr.param_min      = param_min;
  pdr.param_max      = param_max;
  pdr.data_addr      = MEMPTR(*heap_ptr);

  heap_ptr += NUM_BYTES(data_size_in_bits);
  // this aligns the heap pointer to 32bit.  Not doing this causes the floats in the pd to be misaligned, which crashes the arm.
  if((uint32_t)heap_ptr % 4) {
    heap_ptr += 4 - (uint32_t)heap_ptr % 4;
  }

  memcpy(heap_ptr, &pdr, sizeof(process_data_descriptor_t));
  // note that we don't store the names in the struct anymore.  The fixed-length struct is copied into memory, and then the nmaes go in directly behind it, so they'll read out properly

  uint16_t pd_ptr = MEMPTR(*heap_ptr);  // save off the ptr to return, before we modify the heap ptr

  heap_ptr = (uint8_t *)&(((process_data_descriptor_t *)heap_ptr)->names);

  // copy the strings in after the pd
  strcpy((char *)heap_ptr, unit_string);
  heap_ptr += strlen(unit_string) + 1;

  strcpy((char *)heap_ptr, name_string);
  heap_ptr += strlen(name_string) + 1;

  // moved this up to before the pd record
  /*
   // this aligns the heap pointer to 32bit.  Not doing this causes the floats in the pd to be misaligned, which crashes the arm.
   if((uint32_t)heap_ptr % 4){
   heap_ptr += 4 - (uint32_t)heap_ptr % 4;
   }
   */

  return pd_ptr;
}

uint16_t add_mode(char *name_string, uint8_t index, uint8_t type) {
  mode_descriptor_t mdr;
  mdr.record_type = RECORD_TYPE_MODE_DATA_RECORD;
  mdr.index       = index;
  mdr.type        = type;  //hw = 0, sw = 1
  mdr.unused      = 0x00;

  memcpy(heap_ptr, &mdr, sizeof(mode_descriptor_t));

  uint16_t md_ptr = MEMPTR(*heap_ptr);

  heap_ptr = (uint8_t *)&(((mode_descriptor_t *)heap_ptr)->names);

  strcpy((char *)heap_ptr, name_string);
  heap_ptr += strlen(name_string) + 1;

  return md_ptr;
}

void metadata(pd_metadata_t *pdm, process_data_descriptor_t *ptr) {
  pdm->ptr    = ptr;
  pdm->range  = ptr->data_type == DATA_TYPE_SIGNED ? MAX(ABS(ptr->param_min), ABS(ptr->param_max)) * 2 : ptr->param_max;
  pdm->bitmax = (1 << ptr->data_size) - 1;
}

void print_pd(process_data_descriptor_t *pd) {
  int strl   = strlen(&pd->names);
  char *unit = &pd->names;
  char *name = &pd->names + strl + 1;
  switch(pd->data_type) {
    case DATA_TYPE_PAD:
      printf("uint32_t padding : %u;\n", pd->data_size);
      break;
    case DATA_TYPE_BITS:
      for(int i = 0; i < pd->data_size; i++) {
        printf("uint32_t %s_%i : 1;\n", name, i);
      }
      break;
    case DATA_TYPE_UNSIGNED:
      if(pd->data_size == 8) {
        printf("uint8_t %s;\n", name);
      } else {
        printf("warning: unsupported int size!\n");
      }
      break;
    case DATA_TYPE_SIGNED:
      if(pd->data_size == 8) {
        printf("int8_t %s;\n", name);
      } else {
        printf("warning: unsupported int size!\n");
      }
      break;
    case DATA_TYPE_FLOAT:
      //TODO: check size
      if(pd->data_size != 32) {
        printf("warning: unsupported float size!");
      }
      printf("float %s;\n", name);
      break;
    case DATA_TYPE_BOOLEAN:
      printf("uint32_t %s : %u;\n", name, pd->data_size);
      break;
    case DATA_TYPE_NONVOL_UNSIGNED:
    case DATA_TYPE_NONVOL_SIGNED:
    case DATA_TYPE_STREAM:
    case DATA_TYPE_ENCODER:
    case DATA_TYPE_ENCODER_H:
    case DATA_TYPE_ENCODER_L:
    default:
      printf("unsupported data type: 0x%02X\n", pd->data_type);
  }
}

int main() {
  heap_ptr = memory.heap;

  uint16_t input_bits  = 8;  // this starts at 8 bits = 1 byte for the fault byte
  uint16_t output_bits = 0;

  // these are temp toc arrays that the macros will write pointers into.  the tocs get copied to main memory after everything else is written in
  uint16_t ptoc[32];
  uint16_t gtoc[32];

  uint16_t *ptocp = ptoc;
  uint16_t *gtocp = gtoc;
  process_data_descriptor_t *last_pd;

  ADD_PROCESS_VAR(("pos_cmd", "rad", 32, DATA_TYPE_FLOAT, DATA_DIRECTION_OUTPUT, -INFINITY, INFINITY));
  metadata(&(pd_table.pos_cmd), last_pd);
  ADD_PROCESS_VAR(("vel_cmd", "rad", 32, DATA_TYPE_FLOAT, DATA_DIRECTION_OUTPUT, -INFINITY, INFINITY));
  metadata(&(pd_table.vel_cmd), last_pd);
  ADD_PROCESS_VAR(("out", "none", 4, DATA_TYPE_BITS, DATA_DIRECTION_OUTPUT, 0, 1));
  metadata(&(pd_table.output_pins), last_pd);
  ADD_PROCESS_VAR(("enable", "none", 1, DATA_TYPE_BOOLEAN, DATA_DIRECTION_OUTPUT, 0, 1));
  metadata(&(pd_table.enable), last_pd);

  ADD_PROCESS_VAR(("pos_fb", "rad", 32, DATA_TYPE_FLOAT, DATA_DIRECTION_INPUT, -INFINITY, INFINITY));
  metadata(&(pd_table.pos_fb), last_pd);
  ADD_PROCESS_VAR(("vel_fb", "rad", 32, DATA_TYPE_FLOAT, DATA_DIRECTION_INPUT, -INFINITY, INFINITY));
  metadata(&(pd_table.vel_fb), last_pd);
  ADD_PROCESS_VAR(("current", "A", 8, DATA_TYPE_SIGNED, DATA_DIRECTION_INPUT, -30, 30));
  metadata(&(pd_table.current), last_pd);
  ADD_PROCESS_VAR(("in", "none", 4, DATA_TYPE_BITS, DATA_DIRECTION_INPUT, -100, 100));
  metadata(&(pd_table.input_pins), last_pd);
  ADD_PROCESS_VAR(("fault", "none", 1, DATA_TYPE_BOOLEAN, DATA_DIRECTION_INPUT, 0, 1));
  metadata(&(pd_table.fault), last_pd);
  ADD_PROCESS_VAR(("index_enable", "none", 1, DATA_TYPE_BOOLEAN, DATA_DIRECTION_BI_DIRECTIONAL, 0, 1));
  metadata(&(pd_table.index_enable), last_pd);

  ADD_GLOBAL_VAR(("scale", "none", 32, DATA_TYPE_FLOAT, DATA_DIRECTION_OUTPUT, -INFINITY, INFINITY));

  ADD_MODE(("Position mode", 0, 1));

  // automatically create padding pds based on the mod remainder of input/output bits
  if(input_bits % 8)
    ADD_PROCESS_VAR(("padding", "", 8 - (input_bits % 8), DATA_TYPE_PAD, DATA_DIRECTION_INPUT, 0, 0));
  if(output_bits % 8)
    ADD_PROCESS_VAR(("padding", "", 8 - (output_bits % 8), DATA_TYPE_PAD, DATA_DIRECTION_OUTPUT, 0, 0));

  // now that all the toc entries have been added, write out the tocs to memory and set up the toc pointers

  //calculate bytes from bits
  memory.discovery.input  = input_bits >> 3;
  memory.discovery.output = output_bits >> 3;

  memory.discovery.ptocp = MEMPTR(*heap_ptr);

  for(uint8_t i = 0; i < ptocp - ptoc; i++) {
    *heap_ptr++ = ptoc[i] & 0x00FF;
    *heap_ptr++ = (ptoc[i] & 0xFF00) >> 8;
  }
  // this is the ptoc end marker
  *heap_ptr++ = 0x00;
  *heap_ptr++ = 0x00;

  memory.discovery.gtocp = MEMPTR(*heap_ptr);

  for(uint8_t i = 0; i < gtocp - gtoc; i++) {
    *heap_ptr++ = gtoc[i] & 0x00FF;
    *heap_ptr++ = (gtoc[i] & 0xFF00) >> 8;
  }
  // this is the gtoc end marker
  *heap_ptr++ = 0x00;
  *heap_ptr++ = 0x00;

  // printf("gtoc:%u\n",memory.discovery.gtocp);
  // printf("ptoc:%u\n",memory.discovery.ptocp);
  // printf("%i\n",sizeof(memory_t));
  //printf("%u\n",MEMPTR(*heap_ptr));
  int nl = 0;
  printf("uint8_t sserial_slave[] = {\n");
  for(int i = 0; i < MEMPTR(*heap_ptr); i++) {
    //printf("%u %c\n",memory.bytes[i],memory.bytes[i]);
    printf("0x%02X,", memory.bytes[i]);
    nl++;
    if(nl > 7) {
      nl = 0;
      printf("// %i..%i\n", i - 7, i);
    }
  }
  printf("\n};\n\n");

  printf("const discovery_rpc_t discovery = {\n");
  printf("  .ptocp = 0x%04X,\n", memory.discovery.ptocp);
  printf("  .gtocp = 0x%04X,\n", memory.discovery.gtocp);
  printf("  .input = %u,\n", memory.discovery.input);
  printf("  .output = %u,\n", memory.discovery.output);
  printf("};\n\n");

  printf("typedef struct {\n");
  ptocp = (uint16_t *)(memory.bytes + memory.discovery.ptocp);
  gtocp = (uint16_t *)(memory.bytes + memory.discovery.gtocp);
  while(*ptocp != 0x0000) {
    process_data_descriptor_t *pd = (process_data_descriptor_t *)(memory.bytes + *ptocp++);
    //printf("0x%02X\n",pd->data_direction);
    if((pd->data_direction == DATA_DIRECTION_OUTPUT || pd->data_direction == DATA_DIRECTION_BI_DIRECTIONAL) && pd->record_type == RECORD_TYPE_PROCESS_DATA_RECORD) {
      print_pd(pd);
    }
    // printf("pd has data at %x with value %x\n", pd->data_addr, MEMU16(pd->data_addr));
  }
  printf("} sserial_out_process_data_t; //size:%u bytes\n", memory.discovery.output);
  printf("_Static_assert(sizeof(sserial_out_process_data_t) == %u, \"sserial_out_process_data_t size error!\");\n", memory.discovery.output);
  printf("\n");
  printf("typedef struct {\n");
  ptocp = (uint16_t *)(memory.bytes + memory.discovery.ptocp);
  while(*ptocp != 0x0000) {
    process_data_descriptor_t *pd = (process_data_descriptor_t *)(memory.bytes + *ptocp++);
    //printf("0x%02X\n",pd->data_direction);
    if((pd->data_direction == DATA_DIRECTION_INPUT || pd->data_direction == DATA_DIRECTION_BI_DIRECTIONAL) && pd->record_type == RECORD_TYPE_PROCESS_DATA_RECORD) {
      print_pd(pd);
    }
    // printf("pd has data at %x with value %x\n", pd->data_addr, MEMU16(pd->data_addr));
  }
  printf("} sserial_in_process_data_t; //size:%u bytes\n", memory.discovery.input - 1);
  printf("_Static_assert(sizeof(sserial_in_process_data_t) == %u, \"sserial_in_process_data_t size error!\");\n", memory.discovery.input - 1);
  gtocp = (uint16_t *)(memory.bytes + memory.discovery.gtocp);
  while(*gtocp != 0x0000) {
    process_data_descriptor_t *pd = (process_data_descriptor_t *)(memory.bytes + *gtocp++);
    if(pd->record_type == RECORD_TYPE_PROCESS_DATA_RECORD) {
      int strl   = strlen(&pd->names);
      char *unit = &pd->names;
      char *name = &pd->names + strl + 1;
      printf("//global name:%s addr:0x%x size:%i dir:0x%x\n", name, pd->data_addr, pd->data_size, pd->data_direction);
      printf("#define %s_address %i\n", name, pd->data_addr);
    }
  }
}