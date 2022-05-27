#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define npts 8
extern vec3_t verts[npts];

#define nprim (6*2)
extern face_t prims[nprim];

#endif
