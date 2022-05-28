#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define numpts 8
extern vec3_t verts[numpts];

#define numprims (6*2)
extern face_t prims[numprims];

#define numverts (numprims * 3)

#endif
