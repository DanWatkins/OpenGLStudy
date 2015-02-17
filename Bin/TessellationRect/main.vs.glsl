#version 430 core

void main()
{
	const float M = 1.0f;
	const vec4 vertices[] = vec4[](vec4( M, -M, M, 1.0),
									vec4(-M, -M, M, 1.0),
									vec4( M,  M, M, 1.0),
									vec4(-M,  M, M, 1.0));
									
	gl_Position = vertices[gl_VertexID];
}