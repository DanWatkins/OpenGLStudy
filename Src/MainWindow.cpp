//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"
#include "ktx.h"
#include "Bind.h"

static const GLfloat square_vertices[] =
{
    -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f
};

static const GLfloat instance_colors[] =
{
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f
};

static const GLfloat instance_positions[] =
{
    -2.0f, -2.0f, 0.0f, 0.0f,
        2.0f, -2.0f, 0.0f, 0.0f,
        2.0f,  2.0f, 0.0f, 0.0f,
    -2.0f,  2.0f, 0.0f, 0.0f
};


void MainWindow::initShaders()
{
	mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/std_vertex.glsl");
	mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/std_fragment.glsl");

	if (mProgram.link() == false)
		std::cout << "Problem linking shaders" << std::endl;

	std::cout << "Initialized shaders" << std::endl;
}



void MainWindow::initialize()
{
	OpenGLWindow::initialize();

	mVao.create();
	Bind(mVao,
	{
		
		mBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		mBuffer.create();
		mBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		mBuffer.bind();
		mBuffer.allocate(sizeof(square_vertices) + sizeof(instance_colors) + sizeof(instance_positions));
		mBuffer.write(0, square_vertices, sizeof(square_vertices));
		mBuffer.write(sizeof(square_vertices), instance_colors, sizeof(instance_colors));
		mBuffer.write(sizeof(square_vertices)+sizeof(instance_colors), instance_positions, sizeof(instance_positions));

		initShaders();

		mProgram.enableAttributeArray(0);
		mProgram.enableAttributeArray(1);
		mProgram.enableAttributeArray(2);
		mProgram.setAttributeBuffer(0, GL_FLOAT, 0, 4);
		mProgram.setAttributeBuffer(1, GL_FLOAT, sizeof(square_vertices), 4);
		mProgram.setAttributeBuffer(2, GL_FLOAT, sizeof(square_vertices)+sizeof(instance_colors), 4);

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
	})
}


void MainWindow::render()
{
	OpenGLWindow::render();

	static const GLfloat color[] = { 0.95f, 0.95f, 0.95f, 1.0f };
	static const GLfloat ones[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);
	glClearBufferfv(GL_DEPTH, 0, ones);

	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	Bind(mProgram,
	{
		Bind(mVao,
		{
			glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 4);
		})
	})
}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
	if (ev->key() == Qt::Key_Escape)
	{
		QWindow::close();
	}
}