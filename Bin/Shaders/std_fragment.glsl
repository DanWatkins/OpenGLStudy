#version 430 core

layout (location = 0) out vec4 color;

in VS_OUT
{
	vec2 textureCoordinate;
} fs_in;

layout (binding = 0) uniform sampler2D tex;

void main()
{
	color = texture(tex, fs_in.textureCoordinate);
}
