#version 330 core

in vec3 vPosition;
uniform mat4 modelMat;
uniform mat4 mvpMat;

void main()
{
	gl_Position = mvpMat * vec4(vPosition, 1.0);
}