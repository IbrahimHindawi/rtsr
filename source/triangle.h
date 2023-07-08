#include "vector.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H
typedef struct {
	int a;
	int b; 
	int c;
} face_t;
typedef struct {
    vec2_t a;
    vec2_t b;
    vec2_t c;
} triangle_t;
#endif
