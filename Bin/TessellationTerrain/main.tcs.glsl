#version 430 core

layout (vertices = 4) out;

in VSOut
{
	vec2 tc;
} tcs_in[];

out TCSOut
{
	vec2 tc;
} tcs_out[];

uniform mat4 mvpMatrix;

void main()
{
	if (gl_InvocationID == 0)
	{
		vec4 p0 = mvpMatrix * gl_in[0].gl_Position;
		vec4 p1 = mvpMatrix * gl_in[1].gl_Position;
		vec4 p2 = mvpMatrix * gl_in[2].gl_Position;
		vec4 p3 = mvpMatrix * gl_in[3].gl_Position;
		
		p0 /= p0.w;
		p1 /= p1.w;
		p2 /= p2.w;
		p3 /= p3.w;
		
		const float MAX = 16.0;
		float l0 = length(p2.xy - p0.xy) * MAX + 1;
		float l1 = length(p3.xy - p2.xy) * MAX + 1;
		float l2 = length(p3.xy - p1.xy) * MAX + 1;
		float l3 = length(p1.xy - p0.xy) * MAX + 1;
		
		gl_TessLevelOuter[0] = l0;
		gl_TessLevelOuter[1] = l1;
		gl_TessLevelOuter[2] = l2;
		gl_TessLevelOuter[3] = l3;
		gl_TessLevelOuter[0] = min(l1, l3);
		gl_TessLevelOuter[1] = min(l0, l2);
	}
	
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcs_out[gl_InvocationID].tc = tcs_in[gl_InvocationID].tc;
}