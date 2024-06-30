#include "vector.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H

typedef struct {
	int a;
	int b; 
	int c;
} face;
#define facetfmt(handle) "<%d, %d, %d>" " ", (handle)[n].a, (handle)[n].b, (handle)[n].c

typedef struct {
    vec3 a;
    vec3 b;
    vec3 c;
} prim;

typedef struct {
    vec2 a;
    vec2 b;
    vec2 c;
} triangle;

#endif
