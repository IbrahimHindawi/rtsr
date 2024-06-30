#include <math.h>
#include <assert.h>
#include "vector.h"

/*////////////////////////////////////////////////////////////////// 
 * 3D vector operations
 *//////////////////////////////////////////////////////////////////

vec3 vec3_new(float x, float y, float z) {
    vec3 result = {x, y, z};
    return result;
}

vec3 vec3_rotate_x(vec3 v, float angle) {
	vec3 rotated_vector = vec3_new(
		v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle)
	);
	return rotated_vector;
}

vec3 vec3_rotate_y(vec3 v, float angle) {
	vec3 rotated_vector = vec3_new(
		v.x * cos(angle) - v.z * sin(angle),
		v.y,
		v.x * sin(angle) + v.z * cos(angle)
	);
	return rotated_vector;
}

vec3 vec3_rotate_z(vec3 v, float angle) {
	vec3 rotated_vector = vec3_new(
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z
	);
	return rotated_vector;
}

vec3 vec3_scalar_multiply(vec3 v, float scalar){
    vec3 result = {0};
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;
    result = v;
    return result;
}

vec3 vec3_scalar_add(vec3 v, float scalar){
    vec3 result = {0};
    v.x += scalar;
    v.y += scalar;
    v.z += scalar;
    result = v;
    return result;
}

vec3 vec3_add(vec3 a, vec3 b) {
    vec3 result = {0};
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

vec3 vec3_sub(vec3 a, vec3 b) {
    vec3 result = {0};
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3_cross(vec3 a, vec3 b) {
    vec3 result = {0};
    // x y z
    result.x = a.y * b.z - a.z * b.y;
    // y z x
    result.y = a.z * b.x - a.x * b.z;
    // z x y
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

float vec3_magnitude(vec3 a) {
    return sqrt(vec3_dot(a, a));
}

vec3 vec3_normalize(vec3 a) {
    if (a.x == 0.0f && a.y == 0.0f && a.z == 0.0f) {
        return a;
    }
    float magnitude = vec3_magnitude(a);
    float reciprocal = 1.0f / magnitude;
    vec3 result = {0};
    result.x = a.x * reciprocal;
    result.y = a.y * reciprocal;
    result.z = a.z * reciprocal;
    return result;
}

/*////////////////////////////////////////////////////////////////// 
 * 2D vector operations
 *//////////////////////////////////////////////////////////////////
/*
  Projection:
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

vec2 vec2_new(float x, float y) {
    vec2 result = {x, y};
    return result;
}

vec2 perspective_projection(vec3 pt, float fov) {
    float x, y;
    x = (pt.x * fov) / pt.z;
    y = (pt.y * fov) / pt.z;
    return vec2_new(x, y);
}

vec2 vec2_screen_offset(vec2 v, float x_offset, float y_offset) {
    vec2 result = vec2_new(
        v.x += x_offset,
        v.y += y_offset
    );
    return result;
}

float vec2_dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float vec2_magnitude(vec2 a) {
    return sqrt(vec2_dot(a, a));
}

vec2 vec2_normalize(vec2 a) {
    if (a.x == 0.0f && a.y == 0.0f) {
        return a;
    }
    float magnitude = vec2_magnitude(a);
    float reciprocal = 1.0f / magnitude;
    vec2 result = {0};
    result.x = a.x * reciprocal;
    result.y = a.y * reciprocal;
    return result;
}

void vector_test() {
    vec3 v0 = { 1.0f, 0.0f, 0.0f};
    vec3 v1 = { 0.0f, 1.0f, 0.0f};
    vec3 v2 = {3.0f, 2.0f, 10.0f};
    vec3 v3 = {100.0f, 0.0f, 0.0f};

    assert(vec3_dot(v0, v1) == 0.0f);

    vec3 c = vec3_cross(v0, v1);
    assert(c.x == 0.0f && c.y == 0.0f && c.z == 1.0f);

    float m = vec3_magnitude(v3);
    assert(m == 100.0f);
    printf("magnitude: %f\n", m);

    vec3 n = vec3_normalize(v3);
    assert(n.x == 1.0f && n.y == 0.0f && n.z == 0.0f);
    printf("normalize: <%f, %f, %f>\n", n.x, n.y, n.z);

    vec3 n2 = vec3_normalize(v2);
    printf("normalize: <%f, %f, %f>\n", n2.x, n2.y, n2.z);
    printf("magnitude: %f\n", vec3_magnitude(n2));

    return;
}
