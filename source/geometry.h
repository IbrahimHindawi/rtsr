#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vector.h"
#include "triangle.h"

void create_circle_vertices(vec3 *points, int n, float radius);
void create_circle_indices(face *faces, int n);

#endif
