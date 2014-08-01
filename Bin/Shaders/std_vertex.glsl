#version 420

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout (location = 0) in vec4 position;
layout (location = 4) in vec2 textureCoordinate;


out VS_OUT
{
	vec2 tc;
} vs_out;

void main()
{
	vec4 pos_vs = mv_matrix * position;
	vs_out.tc = textureCoordinate;

	gl_Position = proj_matrix * pos_vs;
}
