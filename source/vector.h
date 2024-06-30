#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vec2;
#define vec2tfmt(handle) "<%f, %f>" " ", (handle)[n].x, (handle)[n].y

typedef struct {
    float x;
    float y;
    float z;
} vec3;
#define vec3tfmt(handle) "<%f, %f, %f>" " ", (handle)[n].x, (handle)[n].y, (handle)[n].z

vec3 vec3_new(float x, float y, float z);
vec3 vec3_rotate_x(vec3 v, float angle);
vec3 vec3_rotate_y(vec3 v, float angle);
vec3 vec3_rotate_z(vec3 v, float angle);
vec3 vec3_scalar_multiply(vec3 v, float scalar);
vec3 vec3_scalar_add(vec3 v, float scalar);
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
float vec3_dot(vec3 a, vec3 b);
vec3 vec3_cross(vec3 a, vec3 b);
float vec3_magnitude(vec3 a);
vec3 vec3_normalize(vec3 a);

vec2 vec2_new(float x, float y);
vec2 perspective_projection(vec3 pt, float fov);
vec2 vec2_screen_offset(vec2 v, float screen_width, float screen_height);
float vec2_dot(vec2 a, vec2 b);
float vec2_magnitude(vec2 a);
vec2 vec2_normalize(vec2 a);

void vector_test();

#endif
