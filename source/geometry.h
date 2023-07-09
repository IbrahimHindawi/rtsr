#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vector.h"
#include "triangle.h"

void create_circle_vertices(vec3_t *points, int n, float radius);
void create_circle_indices(face_t *faces, int n);

#endif
