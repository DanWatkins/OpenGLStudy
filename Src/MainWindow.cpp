//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"


void MainWindow::initialize()
{
	OpenGLWindow::initialize();

	mProgram = new QOpenGLShaderProgram(this);
	mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/std_vertex.glsl");
	mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/std_fragment.glsl");
	mProgram->link();
}


void MainWindow::render()
{
	OpenGLWindow::render();

	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);


	mProgram->bind();

	glPointSize(40.0f);
	glDrawArrays(GL_POINTS, 0, 1);

	mProgram->release();
}
