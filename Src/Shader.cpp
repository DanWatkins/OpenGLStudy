//==================================================================================================================|
// Created 2013.07.27 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "Shader.h"

QOpenGLFunctions Shader::mOpenGL = QOpenGLFunctions();


GLuint Shader::loadShader(const String &filepath, GLenum shaderType)
{
	GLuint result = 0;
	FILE *file;
	char *data;
	size_t filesize;

	file = fopen(filepath.c_str(), "rb");

	if (!file)
		return 0;

	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	data = new char[filesize + 1];

	if (!data)
		return result;

	fread(data, 1, filesize, file);
	data[filesize] = 0;
	fclose(file);

	result = mOpenGL.glCreateShader(shaderType);

	if (!result)
		return result;

	mOpenGL.glShaderSource(result, 1, const_cast<const char**>(&data), NULL);
	delete[] data;

	mOpenGL.glCompileShader(result);


	//check for errors
	int status = 0;
	mOpenGL.glGetShaderiv(result, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		char buffer[8192];
		mOpenGL.glGetShaderInfoLog(result, 8192, NULL, buffer);

		std::cout << filepath << ":" << buffer << std::endl;

		mOpenGL.glDeleteShader(result);
	}

	return result;
}


GLuint Shader::linkFromShaders(const GLuint *shaders, int shaderCount)
{
	GLuint program;
	program = mOpenGL.glCreateProgram();

	for (int n = 0; n < shaderCount; n++)
		mOpenGL.glAttachShader(program, shaders[n]);

	mOpenGL.glLinkProgram(program);


	//check for errors
	int status;
	mOpenGL.glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (!status)
	{
		char buffer[8192];
		mOpenGL.glGetProgramInfoLog(program, 8192, NULL, buffer);
		std::cout << buffer << std::endl;

		mOpenGL.glDeleteProgram(program);
		return 0;
	}


	//delete shaders
	for (int n = 0; n < shaderCount; n++)
		mOpenGL.glDeleteShader(shaders[n]);

	return program;
}
