#version 400 core

layout(location = 0) in vec4 vPosition;
uniform mat4 mvpMat;
uniform mat4 modelMat;

void main()
{
	gl_Position = mvpMat * vPosition;
}