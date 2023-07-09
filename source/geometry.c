#include "geometry.h"
#include "math.h"

#define PI 3.14159265359

void create_circle_vertices(vec3_t *points, int n, float radius) {
    float theta = 0.0f;
    float step = (2 * PI) / n;
    for(int i = 0; i < n; i++) {
        points[i].x = sin(theta) * radius;
        points[i].y = cos(theta) * radius;
        points[i].z = 0.0f;
        theta += step;
    }
}

