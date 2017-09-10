#version 400 core

in vec4 vPosition;
uniform mat4 mvpMat;

void main()
{
	gl_Position = mvpMat * vPosition;
}