#version 430 core

out vec4 color;

in VSOut
{
	vec3 normal;
	vec4 color;
} fsIn;

void main()
{
	vec3 N = normalize(fsIn.normal);
	color = fsIn.color * abs(N.z);
}