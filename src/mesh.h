#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define npts 8
vec3_t verts[npts] = {
	{-1.0f, -1.0f, -1.0f}, //0
	{-1.0f,  1.0f, -0.0f}, //1
	{ 1.0f,  1.0f, -0.0f}, //2
	{ 1.0f, -1.0f, -0.0f}, //3
	{ 1.0f,  1.0f,  0.0f}, //4
	{ 1.0f, -1.0f,  0.0f}, //5
	{-1.0f,  1.0f,  0.0f}, //6
	{-1.0f, -1.0f,  0.0f}  //7
};

#define nprim (6*2)
face_t prims[nprim] = {
	{1, 2, 3},
	{1, 3, 4},
	{4, 3, 5},
	{4, 5, 6},
	{6, 5, 7},
	{6, 7, 8},
	{2, 7, 5},
	{2, 5, 3},
	{6, 8, 1},
	{6, 1, 4}
};

#endif
