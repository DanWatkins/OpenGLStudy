#version 430 core

layout (quads, fractional_odd_spacing) in;

uniform sampler2D texDisplacement

uniform mat4 mvMatrix;
uniform mat4 projMatrix;
uniform float dmapDepth;

in TCSOut
{
	vec2 tc;
} tes_in[];

out TESOut
{
	vec2 tc;
	vec3 worldCoord;
	vec3 eyeCoord;
} tes_out;

void main()
{
	vec2 tc1 = mix(tes_in[0].tc, tes_in[1].tc, gl_TessCoord.x);
	vec2 tc2 = mix(tes_in[2].tc, tes_in[3].tc, gl_TessCoord.x);
	vec2 tc = mix(tc2, tc1, gl_TessCoord.y);
	
	vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 p = mix(p2, p1, gl_TessCoord.y);
	p.y = texture(texDisplacement, tc).r * dmapDepth;
	
	vec4 P_eye = mvMatrix * p;
	
	tes_out.tc = tc;
	tes_out.worldCoord = p.xyz;
	tes_out.eyeCoord = P_eye.xyz;
	
	gl_Position = projMatrix * P_eye;
}