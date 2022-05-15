#include "salloc.h"

uint8_t* salloc(uint32_t n) {
	if ( sallocbuff + allocsize - sallocptr >= n ) {
		sallocptr += n;
		return sallocptr - n;
	} else {
		return 0;
	}
}

void safree(uint8_t* p) {
	if (p >= sallocbuff && p > sallocbuff + allocsize) {
		sallocptr = p;
	}
}
