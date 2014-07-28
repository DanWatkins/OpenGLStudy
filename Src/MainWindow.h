//==================================================================================================================|
// Created 2013.07.26 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef TRIANGLETEST_H
#define TRIANGLETEST_H

#include "OpenGLWindow.h"

class MainWindow : public OpenGLWindow
{
public:
	MainWindow()
		: m_program(0)
		, m_frame(0)
	{
	}

	void initialize();
	void render();

private:
	GLuint loadShader(GLenum type, const char *source);

	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform;

	QOpenGLShaderProgram *m_program;
	int m_frame;
};

#endif // TRIANGLETEST_H
