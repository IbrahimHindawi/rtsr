#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "Array.h"

#define numpts 8
extern vec3_t cube_points[numpts];

#define numprims (6 * 2)
extern face_t cube_prims[numprims];

typedef struct {
    Array points;
    Array tris;
    vec3_t rotation;
} mesh_t;

#endif
