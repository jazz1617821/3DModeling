#version 400 core

in vec4 vPosition;
out float gl_ClipDistance[1];


uniform mat4 modelMat;
uniform mat4 mvpMat;
uniform vec4 ClipPlane;

void main()
{
	
	gl_Position = mvpMat * vPosition;
	gl_ClipDistance[0] = dot(modelMat * gl_Position, clipPlane);

}