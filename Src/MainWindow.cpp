//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"


void MainWindow::initialize()
{
	mProgram = new QOpenGLShaderProgram(this);
	mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/std_vertex.glsl");
	mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/std_fragment.glsl");
	mProgram->link();
}


void MainWindow::render()
{
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);



	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	mProgram->bind();


	mProgram->release();
}
