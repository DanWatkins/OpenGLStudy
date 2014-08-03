//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"
#include "ktx.h"


void MainWindow::initShaders()
{
	mProgram = new QOpenGLShaderProgram(this);
	mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/std_vertex.glsl");
	mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/std_fragment.glsl");

	if (mProgram->link() == false)
		std::cout << "Problem linking shaders" << std::endl;

	std::cout << "Initialized shaders" << std::endl;
}


void MainWindow::initTextures()
{
	glGenTextures(1, &mTexture);
	sb6::ktx::File::load("Textures/rightarrows.ktx", mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
}


void MainWindow::initialize()
{
	OpenGLWindow::initialize();

	initShaders();
	initTextures();
}


void MainWindow::render()
{
	OpenGLWindow::render();

	static const GLfloat color[] = { 0.0f, 0.0f, 0.1f, 1.0f };
	static const GLfloat ones[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);
	glClearBufferfv(GL_DEPTH, 0, ones);

	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	mProgram->bind();
	{
		static const GLfloat yellow[] = { 0.4f, 0.4f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, yellow);

		static const GLenum wrapmodes[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT };
		static const float offsets[] = { -0.5f, -0.5f,
												  0.5f, -0.5f,
												 -0.5f,  0.5f,
												  0.5f,  0.5f };

		for (int i=0; i<4; i++)
		{
			mProgram->setUniformValue(0, offsets[i*2]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmodes[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmodes[i]);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}
	mProgram->release();
}
