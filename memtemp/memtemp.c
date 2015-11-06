
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 1024

//process data
#define RECORD_TYPE_PROCESS_DATA_RECORD 0xA0
#define DATA_TYPE_PAD 0x00
#define DATA_TYPE_BITS 0x01
#define DATA_TYPE_UNSIGNED 0x02
#define DATA_TYPE_SIGNED 0x03
#define DATA_TYPE_NONVOL_UNSIGNED 0x04
#define DATA_TYPE_NONVOL_SIGNED 0x05
#define DATA_TYPE_NONVOL_STREAM 0x06
#define DATA_TYPE_NONVOL_BOOLEAN 0x07

#define DATA_DIRECTION_INPUT 0x00
#define DATA_DIRECTION_BI_DIRECTIONAL 0x40
#define DATA_DIRECTION_OUTPUT 0x80

//modes
#define RECORD_TYPE_MODE_DATA_RECORD 0xB0

#define NO_MODES 2//gtoc modes
#define NO_GPD 1//gtoc process data
#define NO_PD 4//process data

#define IS_INPUT(pdr) (pdr->data_direction != 0x80)
#define IS_OUTPUT(pdr) (pdr->data_direction != 0x00)

#define MAX_PD_STRLEN 32 // this is the max space for both the unit and name strings in the PD descriptors
#define MAX_PD_VAL_BYTES 64 // this should be >= the sum of the data sizes of all pd vars

#define MEMPTR(p) ((uint32_t)&p-(uint32_t)&memory)

#define MEMU8(ptr) (memory.bytes[ptr])
#define MEMU16(ptr) (memory.bytes[ptr] | memory.bytes[ptr+1]<<8)
#define MEMU32(ptr) (memory.bytes[ptr] | memory.bytes[ptr+1]<<8 | memory.bytes[ptr+2]<<16 | memory.bytes[ptr+3]<<24)

#define NUM_BYTES(bits) (bits / 8 + (bits % 8 > 0 ? 1 : 0))


typedef struct{
   uint8_t record_type;//0xa0
   uint8_t data_size;
   uint8_t data_type;
   uint8_t data_direction;
   float param_min;
   float param_max;
   uint16_t data_addr;
   char names; // we can pick up the address of that member for a memory location to write strings to.
} process_data_descriptor_t;

typedef struct{
   uint8_t record_type;//0xb0
   uint8_t index;
   uint8_t type;
   uint8_t unused;
   char names;
} mode_descriptor_t;



typedef struct{
	uint8_t input; // these are in BITS now.  I'll convert to bytes when I respond to the rpc.
	uint8_t output;
   uint16_t ptocp;//pointer to process data table
   uint16_t gtocp;//pointer to mode data table
} discovery_rpc_t;


typedef struct{
	mode_descriptor_t md[NO_MODES];
	process_data_descriptor_t pd[NO_GPD];
   uint16_t eot;//end of table
} gtoc_t;

typedef struct{
	process_data_descriptor_t pd[NO_PD];
   uint16_t eot;//end of table
} ptoc_t;


typedef union {
	struct {
		discovery_rpc_t discovery;
		uint8_t heap[MEM_SIZE - sizeof(discovery_rpc_t)];
	};

	uint8_t bytes[MEM_SIZE];
} memory_t;


static memory_t memory;
static uint8_t *heap_ptr;

process_data_descriptor_t create_pdr(uint8_t data_size_in_bits, uint8_t data_type, uint8_t data_dir, float param_min, float param_max) {
	process_data_descriptor_t pd;

	pd.record_type 		= RECORD_TYPE_PROCESS_DATA_RECORD;
	pd.data_size 		= data_size_in_bits;
	pd.data_type 		= data_type;
	pd.data_direction 	= data_dir;
	pd.param_min 		= param_min;
	pd.param_max 		= param_max;
//	strcpy(pd.names, unit_string);
//	strcpy(pd.names + strlen(unit_string) + 1, name_string);

	return pd;
}

mode_descriptor_t create_mdr(uint8_t index, uint8_t type) {
	mode_descriptor_t md;

	md.record_type = RECORD_TYPE_MODE_DATA_RECORD;
	md.index = index;
	md.type = type;
	md.unused = 0x00;
	//strcpy(md.name_string, name_string);

	return md;
}

/*
#define BITSLEFT(ptr) (8-ptr)

void process_data_rpc(uint8_t *input, uint8_t *output) {
	uint8_t pd_cnt = NO_PD;

	printf("in pdrpc; ptoc contains %d entries\n", pd_cnt);

	*(input++) = 0xA5; // fault byte, just for easy recognition

	// data needs to be packed and unpacked based on its type and size
	// input is a pointer to the data that gets sent back to the host
	// need a bit pointer to keep track of partials

	uint8_t output_bit_ptr = 0;

	for(uint8_t i = 0; i < pd_cnt; i++) {
		process_data_descriptor_t pd = memory.ptoc.pd[i];

		if (IS_INPUT(pd)) {
		//	printf("pd %d data size is %d\n", i, memory.ptoc.pd[i].data_size);
			*(input++) = MEMU8(pd.data_add);
		}
		if (IS_OUTPUT(pd)) {
			printf("pd %d data size is %d\n", i, pd.data_size);
			uint16_t data_addr = pd.data_add;
			uint8_t data_size = pd.data_size;

			uint8_t val_bits_remaining = 8;
			uint8_t val = 0x00;

			while(data_size > 0) {
				// the number of bits to unpack this iteration is the number of bits remaining in the pd, or the number of bits remaining in the output byte, 
				// whichever is smaller.  Then, it can be even smaller if we have less room in the current val.

				uint8_t bits_to_unpack = data_size < BITSLEFT(output_bit_ptr) ? data_size : BITSLEFT(output_bit_ptr);
				if (val_bits_remaining < bits_to_unpack) { bits_to_unpack = val_bits_remaining; }

				printf("decoding partial byte, need to read %d bits from current output byte 0x%02x at bit position %d\n", bits_to_unpack, *output, output_bit_ptr);

				// create a bitmask the width of the bits to read, shifted to the position in the output byte that we're pointing to
				uint8_t mask = ((1<<bits_to_unpack) - 1) << (BITSLEFT(output_bit_ptr) - bits_to_unpack);
				printf("mask is 0x%02x\n", mask);

				// val is what we get when we mask off output and then shift it to the proper place.  
				val = (val << bits_to_unpack) | (*output & mask) >> (BITSLEFT(output_bit_ptr) - bits_to_unpack); 
				// this works sorta like a shift register.  We shift the existing bits in val up by the number of bits we just read,
				// and or the new bits to the bottom of the byte.

				printf("val is 0x%02x\n", val);

				val_bits_remaining -= bits_to_unpack;
				data_size -= bits_to_unpack;
				output_bit_ptr += bits_to_unpack;
			
				if((output_bit_ptr %= 8) == 0) output++;

				if(val_bits_remaining == 0 || data_size == 0) {
					MEMU8(data_addr++) = val;
					printf("adding 0x%02x to data\n", val);
					val_bits_remaining = 8;
					val = 0x00;
				}
			}
		}
	}
}

*/

void dump_memory_range(uint16_t start, uint16_t end) {
	start -= (start % 16);
	end -= ((end+1) % 16);
	printf("dump memory from 0x%04x to 0x%04x\n\n", start, end);

	printf("    ");
	for(int i = 0; i < 16; i++) {
		printf(" %x ", i);
	}

	printf("\n---------------------------------------------------\n");

	for(int j = start >> 4; j <= end >> 4; j++) {
		printf("%03x ", j);
		for (int i = 0; i < 16; i++) {
			uint16_t addr = (j<<4) | i;
			printf("%02x ", memory.bytes[addr]);
		}
		printf("\n");
	}
	printf("\n");
}

void dump_memory() {
	dump_memory_range(0, MEM_SIZE);
}

void write_memory_to_file() {
	FILE *fileptr;
	uint8_t *buffer;
	long filelen;

	fileptr = fopen("test.hex", "wb");	

	fwrite(memory.bytes, sizeof(uint8_t), MEM_SIZE, fileptr);
	fclose(fileptr);
	printf("Wrote memory out to test.hex\n");
}

uint16_t add_pd(char *name_string, char *unit_string, uint8_t data_size_in_bits, uint8_t data_type, uint8_t data_dir, float param_min, float param_max) {
	process_data_descriptor_t pdr = create_pdr(data_size_in_bits, data_type, data_dir, param_min, param_max);

	pdr.data_addr = MEMPTR(*heap_ptr);
	heap_ptr += NUM_BYTES(data_size_in_bits);

	memcpy(heap_ptr, &pdr, sizeof(process_data_descriptor_t));
	// note that we don't store the names in the struct anymore.  The fixed-length struct is copied into memory, and then the nmaes go in directly behind it, so they'll read out properly
	dump_memory_range(0, 0x40);

	uint16_t pd_ptr = MEMPTR(*heap_ptr); // save off the ptr to return, before we modify the heap ptr

	heap_ptr = (uint8_t *)&(((process_data_descriptor_t *)heap_ptr)->names);

	// copy the strings in after the pd
	strcpy((char *)heap_ptr, unit_string);
	heap_ptr += strlen(unit_string)+1;
	dump_memory_range(0, 0x40);
	strcpy((char *)heap_ptr, name_string);
	heap_ptr += strlen(name_string)+1;
	dump_memory_range(0, 0x40);

	return pd_ptr;
}

uint16_t add_mode(char *name_string, uint8_t index, uint8_t type) {
	mode_descriptor_t mdr = create_mdr(index, type);

	memcpy(heap_ptr, &mdr, sizeof(mode_descriptor_t));

	uint16_t md_ptr = MEMPTR(*heap_ptr);

	heap_ptr = (uint8_t *)&(((mode_descriptor_t *)heap_ptr)->names);

	strcpy((char *)heap_ptr, name_string);
	heap_ptr += strlen(name_string)+1;

	return md_ptr;
}



#define INDIRECT_PD(pd_ptr) ((process_data_descriptor_t *)(memory.bytes + *pd_ptr))
#define DATA_DIR(pd_ptr) INDIRECT_PD(pd_ptr)->data_direction
#define DATA_SIZE(pd_ptr) INDIRECT_PD(pd_ptr)->data_size

#define ADD_PROCESS_VAR(args) *ptocp = add_pd args; input_bits += IS_INPUT(INDIRECT_PD(ptocp)) ? DATA_SIZE(ptocp) : 0; output_bits += IS_OUTPUT(INDIRECT_PD(ptocp)) ? DATA_SIZE(ptocp) : 0; ptocp++
#define ADD_GLOBAL_VAR(args) *gtocp++ = add_pd args
#define ADD_MODE(args) *gtocp++ = add_mode args


int main(void) {

	heap_ptr = memory.heap;

	uint16_t input_bits = 8; // this starts at 8 bits = 1 byte for the fault byte
	uint16_t output_bits = 0;

	// these are temp toc arrays that the macros will write pointers into.  the tocs get copied to main memory after everything else is written in
	uint16_t ptoc[32];
	uint16_t gtoc[32]; 

	uint16_t *ptocp = ptoc; uint16_t *gtocp = gtoc;

	printf("sizeof pdr: %ld\n", sizeof(process_data_descriptor_t));


	ADD_PROCESS_VAR(("output_pins", "none", 4, DATA_TYPE_BITS, DATA_DIRECTION_OUTPUT, 1.1, 2.2));
	ADD_PROCESS_VAR(("cmd_vel", "rps", 12, DATA_TYPE_UNSIGNED, DATA_DIRECTION_OUTPUT, 0, 0));
	ADD_PROCESS_VAR(("fb_vel", "rps", 12, DATA_TYPE_UNSIGNED, DATA_DIRECTION_INPUT, 0, 0));

	ADD_GLOBAL_VAR(("swr", "non", 8, DATA_TYPE_UNSIGNED, DATA_DIRECTION_OUTPUT, 0, 0));

	ADD_MODE(("foo", 0, 0));
	ADD_MODE(("io_", 1, 1));

	// todo: automatically create padding pds based on the mod remainder of input/output bits

	// now that all the toc entries have been added, write out the tocs to memory and set up the toc pointers

	memory.discovery.input = input_bits >> 3;
	memory.discovery.output = output_bits >> 3;

	memory.discovery.ptocp = MEMPTR(*heap_ptr);

	for(uint8_t i = 0; i < ptocp - ptoc; i++) {
		*heap_ptr++ = ptoc[i] & 0x00FF; 
		*heap_ptr++ = (ptoc[i] & 0xFF00) >> 8;
	}
	*heap_ptr++ = 0x00; *heap_ptr++ = 0x00; // this is the ptoc end marker

	memory.discovery.gtocp = MEMPTR(*heap_ptr);

	for(uint8_t i = 0; i < gtocp - gtoc; i++) {
		*heap_ptr++ = gtoc[i] & 0x00FF; 
		*heap_ptr++ = (gtoc[i] & 0xFF00) >> 8;
	}
	*heap_ptr++ = 0x00; *heap_ptr++ = 0x00; // this is the gtoc end marker

	dump_memory_range(0, 0x100);

	write_memory_to_file();

/*
	uint16_t param_addr = MEMPTR(memory.pd_values);
	uint8_t pd_num = 0, gpd_num = 0, mode_num = 0;

	// we increment these in add_pd
	discovery_rpc.input = 8; // 8 bits because the fault byte will always be present.
	discovery_rpc.output = 0;

	add_pd(&pd_num, &param_addr, 4, DATA_TYPE_BITS, DATA_DIRECTION_INPUT, 0, 0, "none", "input_pins");
	add_pd(&pd_num, &param_addr, 12, DATA_TYPE_UNSIGNED, DATA_DIRECTION_OUTPUT, 0, 0, "rps", "cmd_vel");
	add_pd(&pd_num, &param_addr, 12, DATA_TYPE_UNSIGNED, DATA_DIRECTION_INPUT, 0, 0, "rps", "fb_vel");

	add_gpd(&gpd_num, gpd_num + mode_num, &param_addr, 8, DATA_TYPE_UNSIGNED, DATA_DIRECTION_OUTPUT, 0, 0, "non", "swr");

	add_mode(&mode_num, gpd_num + mode_num, 0, 0, "foo");
	add_mode(&mode_num, gpd_num + mode_num, 1, 1, "io_");

	memory.ptoc.eot = 0x0000;
	memory.gtoc.eot = 0x0000;

	memory.ptocp[pd_num] = 0x0000;
	memory.gtocp[gpd_num + mode_num] = 0x0000; 


	discovery_rpc.ptocp = MEMPTR(memory.ptocp);
	discovery_rpc.gtocp = MEMPTR(memory.gtocp);

	printf("memory created\n\n");
*/




/*
	printf("discovery_rpc:\n");
	printf("input bytes: %d.  output bytes: %d\n", discovery_rpc.input>>3, discovery_rpc.output>>3);
	printf("gtocp: 0x%04x  ptocp: 0x%04x\n", discovery_rpc.gtocp, discovery_rpc.ptocp);

	printf("\nptoc: (at 0x%04x)\n", discovery_rpc.ptocp);
	uint16_t ptocp = discovery_rpc.ptocp;


	uint16_t pd_ptr;

	while(MEMU16(ptocp) != 0x0000) {
		pd_ptr = MEMU16(ptocp);
		printf("pd ptr: 0x%04x\n", pd_ptr);

		printf("\trectype:  0x%02x\n", MEMU8(pd_ptr++));
		printf("\tdatasize: 0x%02x\n", MEMU8(pd_ptr++));
		printf("\tdatatype: 0x%02x\n", MEMU8(pd_ptr++));
		printf("\tdatadir:  0x%02x\n", MEMU8(pd_ptr++));
		printf("\tparmmin:  0x%08x\n", MEMU32(pd_ptr)); pd_ptr += 4;
		printf("\tparmmax:  0x%08x\n", MEMU32(pd_ptr)); pd_ptr += 4;
		printf("\tdataaddr: 0x%04x\n", MEMU16(pd_ptr)); pd_ptr += 2;
		char *unitstr = (char *)(memory.bytes + pd_ptr);
		printf("\tunitstr:  %s\n", unitstr);
		pd_ptr += strlen(unitstr) + 1;
		char *namestr = (char *)(memory.bytes + pd_ptr);
		printf("\tnamestr:  %s\n", namestr);
		pd_ptr += strlen(namestr) + 1;

		ptocp += 2;
	} 

	printf("\ngtoc: (at 0x%04x)\n", discovery_rpc.gtocp);
	uint16_t gtocp = discovery_rpc.gtocp;

	while(MEMU16(gtocp) != 0x0000) {
		pd_ptr = MEMU16(gtocp);
		printf("pd ptr: 0x%04x\n", pd_ptr);

		uint8_t rectype = MEMU8(pd_ptr++);
		printf("\trectype:  0x%02x\n", rectype);
		switch(rectype) {
			case RECORD_TYPE_PROCESS_DATA_RECORD:
				printf("\tdatasize: 0x%02x\n", MEMU8(pd_ptr++));
				printf("\tdatatype: 0x%02x\n", MEMU8(pd_ptr++));
				printf("\tdatadir:  0x%02x\n", MEMU8(pd_ptr++));
				printf("\tparmmin:  0x%08x\n", MEMU32(pd_ptr)); pd_ptr += 4;
				printf("\tparmmax:  0x%08x\n", MEMU32(pd_ptr)); pd_ptr += 4;
				printf("\tdataaddr: 0x%04x\n", MEMU16(pd_ptr)); pd_ptr += 2;
				char *unitstr = (char *)(memory.bytes + pd_ptr);
				printf("\tunitstr:  %s\n", unitstr);
				pd_ptr += strlen(unitstr) + 1;
				char *namestr = (char *)(memory.bytes + pd_ptr);
				printf("\tnamestr:  %s\n", namestr);
				pd_ptr += strlen(namestr) + 1;
				break;
			case RECORD_TYPE_MODE_DATA_RECORD:
				printf("\tindex:    0x%02x\n", pd_ptr++);
				printf("\ttype:     0x%02x\n", pd_ptr++);
				pd_ptr++; // skip 'unused' byte
				namestr = (char *)(memory.bytes + pd_ptr);
				printf("\tnamestr:  %s\n", namestr);
				pd_ptr += strlen(namestr);
				break;
		}

		gtocp += 2;
	}

	// now I'm going to create a couple pointers to values in the pdval space, these will represent our pins
	// I can change those vals, and then do a mock pd_rpc to set all the outputs and send all the inputs


	uint8_t output_buf[discovery_rpc.output>>3];
	uint8_t input_buf[discovery_rpc.input>>3];

	output_buf[0] = 0xF2;
	output_buf[1] = 0x31;
	output_buf[2] = 0x45;
	printf("incoming output bytes: "); for (uint8_t i = 0; i < discovery_rpc.output>>3; i++) { printf("0x%02x ", output_buf[i]); } printf("\n");
	process_data_rpc(input_buf, output_buf);
	printf("outgoing input bytes: "); for (uint8_t i = 0; i < discovery_rpc.input>>3; i++) { printf("0x%02x ", input_buf[i]); } printf("\n");


	printf("value in the pd[2] data pointer: 0x%04x\n", MEMU16(memory.ptoc.pd[2].data_add));
*/


/*
	printf("setting fb and bidir\n");
	*fb_vel = 0xCA;

	output_buf[0] = 0x30;
	output_buf[1] = 0x40;

	printf("updated vals: cmd_vel 0x%02x   fb_vel 0x%02x   bidir 0x%02x\n", *cmd_vel, *fb_vel, *bidir);

	printf("incoming output bytes: "); for (uint8_t i = 0; i < discovery_rpc.output; i++) { printf("0x%02x ", output_buf[i]); } printf("\n");
	process_data_rpc(input_buf, output_buf);
	printf("outgoing input bytes: "); for (uint8_t i = 0; i < discovery_rpc.input; i++) { printf("0x%02x ", input_buf[i]); } printf("\n");

	printf("updated vals: cmd_vel 0x%02x   fb_vel 0x%02x   bidir 0x%02x\n", *cmd_vel, *fb_vel, *bidir);
	*/

	exit(0);
}