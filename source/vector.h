#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

vec2_t vec2_new(float x, float y);
vec3_t vec3_new(float x, float y, float z);
vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);
vec3_t vec3_scalar_multiply(vec3_t v, float scalar);
vec3_t vec3_scalar_add(vec3_t v, float scalar);

#define vec3tfmt(handle) "<%f, %f, %f>" " ", (handle)[n].x, (handle)[n].y, (handle)[n].z

#endif
