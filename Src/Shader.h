//==================================================================================================================|
// Created 2013.07.27 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef SHADER_H
#define SHADER_H

#include "Main.h"

class Shader
{
public:
	/**
	 * Creates and compiles an OpenGL shader object.
	 *
	 * \param filepath The path to the source file
	 * \param shaderType The type of OpenGL shader to create
	 */
	static GLuint loadShader(const String &filepath, GLenum shaderType);

	/**
	 * Links the shader objects to a new OpenGL program object. The old shader objects are deleted.
	 *
	 * \param shaders Array of shader objects to link
	 * \param shaderCount Number of elements in the shaders array
	 *
	 * \returns The new OpenGL program object
	 */
	static GLuint linkFromShaders(const GLuint *shaders, int shaderCount);


private:
	static QOpenGLFunctions mOpenGL;
};

#endif
