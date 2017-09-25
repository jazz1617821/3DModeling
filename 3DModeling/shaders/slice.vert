#version 400 core

layout(location = 0) in vec4 vPosition;
out float gl_ClipDistance[2];

uniform mat4 modelMat;
uniform mat4 mvpMat;
uniform vec4 ClipPlaneUp;
uniform vec4 ClipPlaneDown;

void main()
{
	
	gl_Position = mvpMat * vPosition;
	gl_ClipDistance[0] = dot(modelMat * gl_Position, ClipPlaneUp);
	gl_ClipDistance[1] = dot(modelMat * gl_Position, ClipPlaneDown);

}