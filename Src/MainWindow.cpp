//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"
#include "ktx.h"

static const GLfloat grass_blade[] =
{
	-0.3f, 0.0f,
	 0.3f, 0.0f,
	-0.20f, 1.0f,
	 0.1f, 1.3f,
	-0.05f, 2.3f,
	 0.0f, 3.3f
};


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
	uniforms.mvpMatrix = mProgram->uniformLocation("mvpMatrix");

	glActiveTexture(GL_TEXTURE1);
	mTexGrassLength = sb6::ktx::File::load("Textures/grass_length.ktx");
	glActiveTexture(GL_TEXTURE2);
	mTexGrassOrientation = sb6::ktx::File::load("Textures/grass_orientation.ktx");
	glActiveTexture(GL_TEXTURE3);
	mTexGrassColor = sb6::ktx::File::load("Textures/grass_color.ktx");
	glActiveTexture(GL_TEXTURE4);
	mTexGrassgBend = sb6::ktx::File::load("Textures/grass_bend.ktx");

	if (mTexGrassColor == 0 || mTexGrassgBend == 0 || mTexGrassLength == 0 || mTexGrassOrientation == 0)
	{
		std::cout << "Error loading textures" << std::endl;
	}
}


void MainWindow::initialize()
{
	OpenGLWindow::initialize();

	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	initShaders();
	initTextures();
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

	float currentTime = 3.0;
	float t = (float)currentTime * 0.02f;
	float r = 550.0f;

	mProgram->bind();
	{
		QMatrix4x4 mvMatrix;
		mvMatrix.lookAt(QVector3D(sinf(t) * r, 25.0f, cosf(t) * r),
												 QVector3D(0.0f, -50.0f, 0.0f),
												 QVector3D(0.0f, 1.0f, 0.0));
		QMatrix4x4 projMatrix;
		projMatrix.perspective(45.0f, (float)this->width() / (float)this->height(), 0.1f, 1000.0f);

		mProgram->setUniformValue(uniforms.mvpMatrix, projMatrix*mvMatrix);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glBindVertexArray(mVao);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024*1024);
	}
	mProgram->release();
}
