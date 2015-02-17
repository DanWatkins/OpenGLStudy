#version 430 core

uniform int terrainSize;

out VSOut
{
	vec2 heightTc;
} vsOut;


void main()
{
	const vec4 vertices[] = vec4[](vec4(-0.5, 0.0, -0.5, 1.0),
                                   vec4( 0.5, 0.0, -0.5, 1.0),
                                   vec4(-0.5, 0.0,  0.5, 1.0),
                                   vec4( 0.5, 0.0,  0.5, 1.0));
	
	
	vec2 pos;
	pos.x = gl_InstanceID & (terrainSize-1);
	pos.y = gl_InstanceID / terrainSize;

	vsOut.heightTc = (vertices[gl_VertexID].xz + pos + vec2(0.5)) / 64.0f;
	gl_Position = vertices[gl_VertexID] + vec4(pos.x-32, 0, pos.y-32, 0);	
}