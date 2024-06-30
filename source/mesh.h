#pragma once

#include "vector.h"
#include "triangle.h"
#include <hkArray.h>

#define numpts 8
// extern vec3 cube_points[numpts];

#define numprims (6 * 2)
// extern face cube_prims[numprims];

typedef struct {
    hkArray_vec3 points;
    hkArray_face prims;
    vec3 rotation;
} mesh;

vec3 cube_points[numpts] = {
	{-1.0f, -1.0f, -1.0f}, //1
	{-1.0f,  1.0f, -1.0f}, //2
	{ 1.0f,  1.0f, -1.0f}, //3
	{ 1.0f, -1.0f, -1.0f}, //4
	{ 1.0f,  1.0f,  1.0f}, //5
	{ 1.0f, -1.0f,  1.0f}, //6
	{-1.0f,  1.0f,  1.0f}, //7
	{-1.0f, -1.0f,  1.0f}  //8
};

face cube_prims[numprims] = {
	{1, 2, 3},
	{1, 3, 4},
	{4, 3, 5},
	{4, 5, 6},
	{6, 5, 7},
	{6, 7, 8},
	{8, 7, 2},
	{8, 2, 1},
	{2, 7, 5},
	{2, 5, 3},
	{6, 8, 1},
	{6, 1, 4}
};

void load_mesh_from_obj(char *filename, hkArray_vec3 *varray, hkArray_face *iarray) {
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open file!");
        exit(-1);
        return;
    } else {
        printf("file found!\n");
    }
    char line[1024];

    while(fgets(line, 1024, file)) {
        // printf("%s", line);
        if(strncmp(line, "v ", 2) == 0) {
            vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            hkarray_vec3_append(varray, vertex);
        }
        if(strncmp(line, "f ", 2) == 0) {
            int indices[3];
            int texture[3];
            int normals[3];
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &indices[0], &texture[0], &normals[0],
                &indices[1], &texture[1], &normals[1],
                &indices[2], &texture[2], &normals[2]
            );
            face face = {indices[0], indices[1], indices[2]};
            hkarray_face_append(iarray, face);
        }
    }
    return;
}
