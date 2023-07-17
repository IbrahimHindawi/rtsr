#include "vector.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H

typedef struct {
	int a;
	int b; 
	int c;
} face_t;
#define facetfmt(handle) "<%d, %d, %d>" " ", (handle)[n].a, (handle)[n].b, (handle)[n].c

typedef struct {
    vec3_t a;
    vec3_t b;
    vec3_t c;
} prim_t;

typedef struct {
    vec2_t a;
    vec2_t b;
    vec2_t c;
} triangle_t;

#endif
