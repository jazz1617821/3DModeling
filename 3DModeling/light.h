#ifndef _LIGHT_H
#define _LIGHT_H

typedef struct Light {
	char name[1024];
	float position[4];
	float direction[3];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float cutoff;
}lit_t;

typedef struct GlobalLight {
	float ambient[4];
}glit_t;

lit_t* newLight(const char* name);
void deleteLight(lit_t** light);
glit_t* newGLight(void);
void deleteGLight(glit_t** glight);
void setLitPosition(lit_t* light, float x, float y, float z, float w);
void setLitDirection(lit_t* light, float x, float y, float z);
void setLitDiffuse(lit_t* light, float r, float g, float b, float a);
void setLitSpecular(lit_t* light, float r, float g, float b, float a);
void setLitEmission(lit_t* light, float r, float g, float b, float a);
void setLitCutOff(lit_t* light, float angles);
void setGLitAmbient(glit_t* gLight, float r, float g, float b, float a);

#endif