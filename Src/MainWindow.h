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

const int NumberOfDraws = 50000;

struct DrawArraysIndirectCommand
{
	GLuint count, primCount, first, baseInstance;
};


enum class Mode
{
	First,
	MultiDraw = 0,
	SeparateDraws,
	Max = SeparateDraws
};


class MainWindow : public OpenGLWindow
{
public:
	MainWindow()
		: mProgram(this)
	{
	}

	void initialize();
	void render();

private:
	QOpenGLShaderProgram mProgram;

	sb6::object mObject;
	GLuint mIndirectDrawBuffer, mDrawIndexBuffer;

	struct
	{
		GLint time;
		GLint matrixView, matrixProj, matrixViewProj;
	} uniforms;

	Mode mMode;
	bool mPaused, mVsync;

	void keyPressEvent(QKeyEvent *ev);
	void initShaders();
};


#endif
