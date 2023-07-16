#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "Array.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define numpts 8
extern vec3_t cube_points[numpts];

#define numprims (6 * 2)
extern face_t cube_prims[numprims];

typedef struct {
    Array points;
    Array prims;
    vec3_t rotation;
} mesh_t;

// mesh must have position, uv and normal attributes!
void load_mesh_from_obj(char *filename, Array *varray, Array *iarray);

#endif
