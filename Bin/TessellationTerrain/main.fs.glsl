#version 430 core

out vec4 color;

layout (binding = 1) uniform sampler2D texColor;

in TESOut
{
	vec2 tc;
	vec3 worldCoord;
	vec3 eyeCoord;
} fs_in;

uniform vec4 fog_color = vec4(0.7, 0.8, 0.9, 0.0);

vec4 fog(vec4 c)
{
    float z = length(fs_in.eyeCoord);

    float de = 0.025 * smoothstep(0.0, 6.0, 10.0 - fs_in.worldCoord.y);
    float di = 0.045 * (smoothstep(0.0, 40.0, 20.0 - fs_in.worldCoord.y));

    float extinction   = exp(-z * de);
    float inscattering = exp(-z * di);

    return c * extinction + fog_color * (1.0 - inscattering);
}


void main()
{
	color = fog(texture(texColor, fs_in.tc * 32));
	//color = vec4(1.0, 0.1, 0.2, 1.0);
}