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


/*////////////////////////////////////////////////////////////////// 
 * Projection
 *//////////////////////////////////////////////////////////////////
/*
  similar triangles have the same ratios when we divide their sides
  Y
  |.
  |   .
  |      .  C
  |         .
  |         |   .
  |         |      .
  |         |         .
  -----------------------.
  X         B            A

  BC/XY = AB/AX
  if AB = 1 then BC = XY/AX
  else BC = (1/XY) * (AB/AX) then BC = (1*AB) / (XY/AX)
  C.x = Y.x/X.z
  basically: you divide by Z
*/

vec2_t perspective_projection(vec3_t pt, float fov) {
    float x, y;
    x = (pt.x * fov) / pt.z;
    y = (pt.y * fov) / pt.z;
    return vec2_new(x, y);
}

vec2_t vec2_screen_offset(vec2_t v, float x_offset, float y_offset) {
    vec2_t result = vec2_new(
        v.x += x_offset,
        v.y += y_offset
    );
    return result;
}
