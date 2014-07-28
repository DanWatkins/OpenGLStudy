//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef TRIANGLETEST_H
#define TRIANGLETEST_H

#include "OpenGLWindow.h"

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
};

#endif
