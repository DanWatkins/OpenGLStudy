#version 420 core

uniform sampler2D s;
out vec4 color;

void main()
{
	color = texture(s, gl_FragCoord.xy / textureSize(s, 0));
}
