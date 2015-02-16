#version 430 core

out vec4 color;

layout (binding = 1) uniform sampler2D texColor;

out TESOut
{
	vec2 tc;
	vec3 worldCoord;
	vec3 eyeCoord;
} fs_in;

void main()
{
	color = texture(texColor, fs_in.tc);
}