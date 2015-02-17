#version 430 core

layout (vertices = 4) out;

void main()
{
	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 8.0;	//top
		gl_TessLevelInner[1] = 4.0; //left
		
		gl_TessLevelOuter[0] = 4.0;	//right
		gl_TessLevelOuter[1] = 5.0; //bottom
		gl_TessLevelOuter[2] = 8.0; //left
		gl_TessLevelOuter[3] = 8.0; //top
	}
	
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}