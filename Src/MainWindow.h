//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef TRIANGLETEST_H
#define TRIANGLETEST_H

#include "OpenGLWindow.h"
#include "Object.h"

class MainWindow : public OpenGLWindow
{
public:
	MainWindow()
		: mProgram(0)
	{
	}

	void initialize();
	void render();

private:
	QOpenGLShaderProgram *mProgram;
	GLuint mVao;

	struct
	{
		GLint mvp;
		GLint offset;
	} uniforms;

	GLuint mTextures[3];
	static const GLuint TextureWall = 0;
	static const GLuint TextureCeiling = 1;
	static const GLuint TextureFloor = 2;


	void initShaders();
	void initTextures();

	void renderTunnel();
};


#endif
