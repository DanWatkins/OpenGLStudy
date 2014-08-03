#version 430 core

uniform sampler2D sampler;

out vec4 color;
in vec2 textureCoordinate;

void main()
{
	color = texture(sampler, textureCoordinate);
}
