#version 430 core

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 instance_color;
layout (location = 2) in vec4 instance_position;

out vec4 color;

void main()
{
	gl_Position = (vertex+instance_position) * vec4(0.25, 0.25, 1.0, 1.0);
	color = instance_color;
}