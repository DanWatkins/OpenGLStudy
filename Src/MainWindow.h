//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "OpenGLWindow.h"
#include "Object.h"

enum BufferType
{
	PositionA,
	PositionB,
	VelocityA,
	VelocityB,
	Connection
};


const int PointsX = 50;
const int PointsY = 50;
const int PointsTotal = (PointsX*PointsY);
const int ConnectionsTotal = (PointsX-1)*PointsY + (PointsY-1)*PointsX;


class MainWindow : public OpenGLWindow
{
public:
	MainWindow()
		: mProgramUpdate(this),
		mProgramRender(this)
	{
	}

	void initialize();
	void render();


private:
	void keyPressEvent(QKeyEvent *ev);
	void initShaders();


private:
	QOpenGLShaderProgram mProgramUpdate, mProgramRender;

	GLuint mVao[2];
	GLuint mVbo[5];
	GLuint mIbo;
	GLuint mPosTbo[2];

	GLuint mCLoc;
	GLuint mIterationIndex;
	int mIterationsPerFrame;

	bool mDrawPoints;
	bool mDrawLines;
};

#endif
