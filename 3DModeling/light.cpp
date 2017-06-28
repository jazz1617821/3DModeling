#include "light.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

lit_t* newLight(const char* name)
{
	lit_t* light;

	light = (lit_t*)calloc(1, sizeof(lit_t));
	if (light == NULL) {
		assert("Null...\n");
	}
	strcpy(light->name, name);
	setLitPosition(light, 1, 1, 1, 0);
	setLitDirection(light, -1, -1, -1);
	setLitDiffuse(light, 0.6, 0.6, 0.6, 1.0);
	setLitSpecular(light, 1.0, 1.0, 1.0, 1.0);
	setLitEmission(light, 0.0, 0.0, 0.0, 0.0);
	setLitCutOff(light, 90.0);

	return light;
}

void deleteLight(lit_t** light)
{
	free((*light));
	*light = NULL;
}

glit_t* newGLight(void)
{
	glit_t* glight;

	glight = (glit_t*)calloc(1, sizeof(glit_t));
	if (glight == NULL) {
		assert("Null...\n");
	}

	setGLitAmbient(glight, 0.22, 0.22, 0.22, 1.0);

	return glight;
}

void deleteGLight(glit_t** glight)
{
	free((*glight));
	*glight = NULL;
}

void setLitPosition(lit_t* light, float x, float y, float z, float w)
{
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;
}

void setLitDirection(lit_t* light, float x, float y, float z)
{
	light->direction[0] = x;
	light->direction[1] = y;
	light->direction[2] = z;
}

void setLitDiffuse(lit_t* light, float r, float g, float b, float a)
{
	light->diffuse[0] = r;
	light->diffuse[1] = g;
	light->diffuse[2] = b;
	light->diffuse[3] = a;
}

void setLitSpecular(lit_t* light, float r, float g, float b, float a)
{
	light->specular[0] = r;
	light->specular[1] = g;
	light->specular[2] = b;
	light->specular[3] = a;
}

void setLitEmission(lit_t* light, float r, float g, float b, float a)
{
	light->emission[0] = r;
	light->emission[1] = g;
	light->emission[2] = b;
	light->specular[3] = a;
}

void setLitCutOff(lit_t * light, float angles)
{
	light->cutoff = angles;
}

void setGLitAmbient(glit_t * gLight, float r, float g, float b, float a)
{
	gLight->ambient[0] = r;
	gLight->ambient[1] = g;
	gLight->ambient[2] = b;
	gLight->ambient[3] = a;
}
