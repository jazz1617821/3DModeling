#version 400 core

layout(location = 0)in vec4 vPosition;
layout(location = 1)in vec4 coordinate;
uniform mat4 modelMat;
uniform mat4 mvpMat;

void main()
{
	
	gl_Position = mvpMat * vPosition;

}