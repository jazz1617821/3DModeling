#version 400 core

in vec4 vPosition;
in vec4 clipPlane;
vec4 fragPos;
uniform mat4 modelMat;
uniform mat4 mvpMat;

void main()
{
	fragPos = modelMat * vPosition;
	
	gl_ClipDistance[0] = dot(fragPos, clipPlane);
	
	gl_Position = mvpMat * vPosition;
}