#version 430 core

in VSOut
{
	vec2 tc;
} vs_out;

void main()
{
	const vec4 vertices[] vec4[](vec4(-.5, 0, -.5, 1),
									vec4(.5, 0, -.5, 1),
									vec4(-.5, 0, .5, 1),
									vec4(.5, 0, .5, 1));
									
	int x = gl_InstanceID & 63;
	int y = gl_InstanceID / 64;	//TODO try bit-shifting right 6 to see if it's actually faster
	vec2 offset = vec2(x, y);
	
	vs_out.tc = (vertices[gl_VertexID].xz + offs + vec2(.5)) / 64.0;
	gl_Position = vertices[gl_VertexID] + vec4(float(x-32), 0, float(y-32), 0);
}