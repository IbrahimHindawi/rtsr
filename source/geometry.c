#include "geometry.h"
#include "math.h"

#define PI 3.14159265359

void create_circle_vertices(vec3 *points, int n, float radius) {
    float theta = 0.0f;
    float step = (2 * PI) / (n - 1);
    for (int i = 1; i < n; i++) {
        points[i].x = sin(theta) * radius;
        points[i].y = cos(theta) * radius;
        points[i].z = 0.0f;
        theta += step;
    }
}

void create_circle_indices(face *faces, int n) {
    for (int i = 0; i < n; i++) {
        faces[i].a = 0;
        faces[i].b = i + 1;
        faces[i].c = i + 2;
    }
    faces[n-1].c = 1;
}
