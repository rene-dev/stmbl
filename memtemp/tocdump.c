#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define MEMPTR(p) ((uint32_t)&p-(uint32_t)&memory)
#define RECORD_TYPE_PROCESS_DATA_RECORD 0xA0
#define RECORD_TYPE_MODE_DATA_RECORD 0xB0

#define MEMU8(m, ptr) (m[ptr])
#define MEMU16(m, ptr) (m[ptr] | m[ptr+1]<<8)
#define MEMU32(m, ptr) (m[ptr] | m[ptr+1]<<8 | m[ptr+2]<<16 | m[ptr+3]<<24)

void print_tocs(uint8_t *m, uint16_t ptocp, uint16_t gtocp) {
	uint16_t pd_ptr;

	printf("\nptoc: (at 0x%04x)\n", ptocp);
	while(MEMU16(m, ptocp) != 0x0000) {
		pd_ptr = MEMU16(m, ptocp);
		printf("pd ptr: 0x%04x\n", pd_ptr);

		printf("\trectype:  0x%02x\n", MEMU8(m, pd_ptr++));
		printf("\tdatasize: 0x%02x\n", MEMU8(m, pd_ptr++));
		printf("\tdatatype: 0x%02x\n", MEMU8(m, pd_ptr++));
		printf("\tdatadir:  0x%02x\n", MEMU8(m, pd_ptr++));
		printf("\tparmmin:  0x%08x\n", MEMU32(m, pd_ptr)); pd_ptr += 4;
		printf("\tparmmax:  0x%08x\n", MEMU32(m, pd_ptr)); pd_ptr += 4;
		printf("\tdataaddr: 0x%04x\n", MEMU16(m, pd_ptr)); pd_ptr += 2;
		char *unitstr = (char *)(m + pd_ptr);
		printf("\tunitstr:  %s\n", unitstr);
		pd_ptr += strlen(unitstr) + 1;
		char *namestr = (char *)(m + pd_ptr);
		printf("\tnamestr:  %s\n", namestr);
		pd_ptr += strlen(namestr) + 1;

		ptocp += 2;
	} 

	printf("\ngtoc: (at 0x%04x)\n", gtocp);
	while(MEMU16(m, gtocp) != 0x0000) {
		pd_ptr = MEMU16(m, gtocp);
		printf("pd ptr: 0x%04x\n", pd_ptr);

		uint8_t rectype = MEMU8(m, pd_ptr++);
		printf("\trectype:  0x%02x\n", rectype);
		switch(rectype) {
			case RECORD_TYPE_PROCESS_DATA_RECORD:
				printf("\tdatasize: 0x%02x\n", MEMU8(m, pd_ptr++));
				printf("\tdatatype: 0x%02x\n", MEMU8(m, pd_ptr++));
				printf("\tdatadir:  0x%02x\n", MEMU8(m, pd_ptr++));
				printf("\tparmmin:  0x%08x\n", MEMU32(m, pd_ptr)); pd_ptr += 4;
				printf("\tparmmax:  0x%08x\n", MEMU32(m, pd_ptr)); pd_ptr += 4;
				printf("\tdataaddr: 0x%04x\n", MEMU16(m, pd_ptr)); pd_ptr += 2;
				char *unitstr = (char *)(m + pd_ptr);
				printf("\tunitstr:  %s\n", unitstr);
				pd_ptr += strlen(unitstr) + 1;
				char *namestr = (char *)(m + pd_ptr);
				printf("\tnamestr:  %s\n", namestr);
				pd_ptr += strlen(namestr) + 1;
				break;
			case RECORD_TYPE_MODE_DATA_RECORD:
				printf("\tindex:    0x%02x\n", pd_ptr++);
				printf("\ttype:     0x%02x\n", pd_ptr++);
				pd_ptr++; // skip 'unused' byte
				namestr = (char *)(m + pd_ptr);
				printf("\tnamestr:  %s\n", namestr);
				pd_ptr += strlen(namestr);
				break;
		}

		gtocp += 2;
	}
}

int main(void) {

	FILE *fileptr;
	uint8_t *buffer;
	long filelen;

	fileptr = fopen("test.hex", "rb");	// open in binary read mode
	fseek(fileptr, 0, SEEK_END); 		// jump to the end of the file
	filelen = ftell(fileptr);			// get the byte offset of the end of the file
	rewind(fileptr);					// jump back to the beginning to start reading

	buffer = (uint8_t *)malloc((filelen)*sizeof(uint8_t));  
	fread(buffer, filelen, 1, fileptr);
	fclose(fileptr);

	printf("read mem.hex\n\n");

	// MY memory dumps have the discovery_rpc struct at the beginning, so byte 0 and 1 are input and output byte counts, bytes 2/3 and 4/5 are the ptocp and gtocp.
	uint16_t ptocp = buffer[2] | buffer[3]<<8;
	uint16_t gtocp = buffer[4] | buffer[5]<<8;
	print_tocs(buffer, ptocp, gtocp);
}