#include <math.h>
#include "vector.h"

vec2_t vec2_new(float x, float y) {
    vec2_t result = {x, y};
    return result;
}

vec3_t vec3_new(float x, float y, float z) {
    vec3_t result = {x, y, z};
    return result;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated_vector = vec3_new(
		v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle)
	);
	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_vector = vec3_new(
		v.x * cos(angle) - v.z * sin(angle),
		v.y,
		v.x * sin(angle) + v.z * cos(angle)
	);
	return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_vector = vec3_new(
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z
	);
	return rotated_vector;
}

vec3_t vec3_scalar_multiply(vec3_t v, float scalar){
    vec3_t result_vector = {0};
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;
    result_vector = v;
    return result_vector;
}

vec3_t vec3_scalar_add(vec3_t v, float scalar){
    vec3_t result_vector = {0};
    v.x += scalar;
    v.y += scalar;
    v.z += scalar;
    result_vector = v;
    return result_vector;
}
