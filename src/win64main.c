#include <stdio.h>
#include <stdint.h>
#include "salloc.h"
int main()
{
	uint8_t *memory = salloc(4);
	if (memory == NULL) {
		printf("ERROR: Out of memory.\n");
		return 1;
	}
	memory[0] = 0xFF;
	memory[1] = 0xFF;
	memory[2] = 0xFF;
	memory[3] = 0xFF;
	uint32_t *reint = (uint32_t*)memory;
	//*reint = 0xDEADBEEF;
	printf("%u\n", *reint);
	return 0;
}