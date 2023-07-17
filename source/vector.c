#include <math.h>
#include <assert.h>
#include "vector.h"

/*////////////////////////////////////////////////////////////////// 
 * 3D vector operations
 *//////////////////////////////////////////////////////////////////

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
    vec3_t result = {0};
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;
    result = v;
    return result;
}

vec3_t vec3_scalar_add(vec3_t v, float scalar){
    vec3_t result = {0};
    v.x += scalar;
    v.y += scalar;
    v.z += scalar;
    result = v;
    return result;
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t result = {0};
    // x y z
    result.x = a.y * b.z - a.z * b.y;
    // y z x
    result.y = a.z * b.x - a.x * b.z;
    // z x y
    result.z = a.x * b.y - a.y * b.z;
    return result;
}

float vec3_magnitude(vec3_t a) {
    return sqrt(vec3_dot(a, a));
}

vec3_t vec3_normalize(vec3_t a) {
    if (a.x == 0.0f && a.y == 0.0f && a.z == 0.0f) {
        return a;
    }
    float magnitude = vec3_magnitude(a);
    float reciprocal = 1.0f / magnitude;
    vec3_t result = {0};
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

vec2_t vec2_new(float x, float y) {
    vec2_t result = {x, y};
    return result;
}

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

float vec2_dot(vec2_t a, vec2_t b) {
    return a.x * b.x + a.y * b.y;
}

float vec2_magnitude(vec2_t a) {
    return sqrt(vec2_dot(a, a));
}

vec2_t vec2_normalize(vec2_t a) {
    if (a.x == 0.0f && a.y == 0.0f) {
        return a;
    }
    float magnitude = vec2_magnitude(a);
    float reciprocal = 1.0f / magnitude;
    vec2_t result = {0};
    result.x = a.x * reciprocal;
    result.y = a.y * reciprocal;
    return result;
}

void vector_test() {
    vec3_t v0 = { 1.0f, 0.0f, 0.0f};
    vec3_t v1 = { 0.0f, 1.0f, 0.0f};
    vec3_t v2 = {3.0f, 2.0f, 10.0f};
    vec3_t v3 = {100.0f, 0.0f, 0.0f};

    assert(vec3_dot(v0, v1) == 0.0f);

    vec3_t c = vec3_cross(v0, v1);
    assert(c.x == 0.0f && c.y == 0.0f && c.z == 1.0f);

    float m = vec3_magnitude(v3);
    assert(m == 100.0f);
    printf("magnitude: %f\n", m);

    vec3_t n = vec3_normalize(v3);
    assert(n.x == 1.0f && n.y == 0.0f && n.z == 0.0f);
    printf("normalize: <%f, %f, %f>\n", n.x, n.y, n.z);

    vec3_t n2 = vec3_normalize(v2);
    printf("normalize: <%f, %f, %f>\n", n2.x, n2.y, n2.z);
    printf("magnitude: %f\n", vec3_magnitude(n2));

    return;
}
