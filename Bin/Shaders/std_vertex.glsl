#version 430 core

uniform vec2 offset;

out vec2 textureCoordinate;

void main()
{
	const vec4 vertices[] = vec4[](vec4(-.45, -.45, .5, 1.0),
									vec4(.45, -.45, .5, 1.0),
									vec4(-.45, .45, .5, 1.0),
									vec4(.45, .45, .5, 1.0));

	gl_Position = vertices[gl_VertexID] + vec4(offset, 0.0, 0.0);
	textureCoordinate = vertices[gl_VertexID].xy * 3.0 + vec2(.45 * 3.0);
}
