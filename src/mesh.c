#ifndef MESH_H
#define MESH_H

#include "vector.h"

#define npts 8
vec3_t mesh_verts[npts] = {
	{-1.0f, -1.0f, -1.0f}, //0
	{-1.0f,  1.0f, -1.0f}, //1
	{ 1.0f,  1.0f, -1.0f}, //2
	{ 1.0f, -1.0f, -1.0f}, //3
	{ 1.0f,  1.0f,  1.0f}, //4
	{ 1.0f, -1.0f,  1.0f}, //5
	{-1.0f,  1.0f,  1.0f}, //6
	{-1.0f, -1.0f,  1.0f}  //7
};
#endif
