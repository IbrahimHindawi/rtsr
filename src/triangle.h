#include "vector.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H
typedef struct {
	int a;
	int b; 
	int c;
} face_t;
typedef struct {
    vec2_t x;
    vec2_t y;
    vec2_t z;
} triangle_t;
#endif
