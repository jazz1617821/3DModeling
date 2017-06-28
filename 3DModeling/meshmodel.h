#ifndef _MESHMODEL_H
#define _MESHMODEL_H

typedef struct AxisAlignedBoundingBox
{
	float min[3];
	float max[3];
}aabb_t;

typedef struct Vector3D
{
	float v[3];
}vec3_t;

typedef struct Edge
{
	int v[2];
}edge_t;

typedef struct Face
{
	int v[3];
	int vn[3];
	int vt[3];
}face_t;

typedef struct HalfEdge
{
	HalfEdge* next;
	HalfEdge* previous;
	HalfEdge* pair;

	int vertex;
	int face;
	int edge;
}halfedge_t;

typedef struct VertexArray
{
	int numVertices;
	int capVertices;
	float* data;
}va_t;

typedef struct MeshModel
{
	char name[512];
	unsigned short id;

	va_t vertexArray;
	va_t normalArray;
	va_t texcoordArray;
	int numFaces;
	int capFaces;
	face_t* faces;
	int numHalfEdges;
	int capHalfEdges;
	halfedge_t* halfedges;
	float precision;
	aabb_t bbox;
}model_t;

typedef struct MeshModelBVH
{
	char name[512];
	unsigned short id;

	int numChildren;
	MeshModelBVH* parent;
	MeshModelBVH** children;
	model_t* model;
	aabb_t bbox;
}modelbvh_t;

typedef struct MeshModelObject
{
	char name[512];
	modelbvh_t* root;
	int numModels;
	model_t** models;
}modelobj_t;

typedef struct MeshModelObjectArray
{
	int numObjects;
	int capObjects;
	modelobj_t** objects;
}moa_t;

void initAABB(aabb_t* bbox);
bool addBound(aabb_t* bbox, float val[3]);
void setAABB(aabb_t* dest, aabb_t* src);
void aabbCenter(aabb_t bbox, float center[3]);

int addVertexArray3D(float v[3], va_t* va);
void initVertexArray(va_t* va);
void getVertex3D(va_t va, int index, float v[3]);
void calTriangleNormal(float v0[3], float v1[3], float v2[3], float vn[3]);

model_t* newModel();
void deleteModel(model_t** model);

modelbvh_t* newModelBVH();
void deleteModelBVH(modelbvh_t** bvh);

modelobj_t* newModelObject();
void deleteModelObject(modelobj_t** obj);

moa_t* newModelObjectArray();
void deleteModelObjectArray(moa_t** moa);

void turnZUp(modelobj_t* obj);
void addModelObject(modelobj_t* obj, moa_t* moa);
modelobj_t* loadSTL(const char* filename);
void getTriangle(model_t* model, int face, float triVert[9]);


#endif // _MESHMODEL_H
