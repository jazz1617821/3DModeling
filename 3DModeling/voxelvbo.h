#ifndef _VOXELVBO_H
#define _VOXELVBO_H

#include "voxel.h"
#include "object.h"


vbo_t* createVoxelVBO(vobj_t* vo);
/*
vbo_t* createVoxelClipPlain(vbo_t* vbo, float clip[4], int voxIdx, vdata_t* obj);
void updateVoxelClipPlain(vbo_t* vbo, float clip[4], int voxIdx, vdata_t* obj);
*/

#endif // _VOXELVBO_H
