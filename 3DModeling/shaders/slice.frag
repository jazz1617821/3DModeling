#version 400 core

uniform sampler3D dataTexture;
out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0,1.0,1.0,1.0);
}