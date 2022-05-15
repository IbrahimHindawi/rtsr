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

vec3_t new_vec3(float x, float y, float z) {
    vec3_t result = {x, y, z};
    return result;
}
vec2_t new_vec2(float x, float y) {
    vec2_t result = {x, y};
    return result;
}
#endif