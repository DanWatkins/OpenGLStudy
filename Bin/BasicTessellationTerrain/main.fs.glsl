#version 430 core

in TESOut
{
	vec2 heightTc;
	vec3 worldCoord;
	vec3 eyeCoord;
} fsIn;

out vec4 color;


void main()
{
	color = vec4(0.0, 0.0, 0.0, 1.0);
}