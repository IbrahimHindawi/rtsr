#include <stdint.h>
#define allocsize 0x36666666
static uint8_t sallocbuff[allocsize];
static uint8_t *sallocptr = sallocbuff;
uint8_t* salloc(uint32_t n);
void safree(uint8_t* p);