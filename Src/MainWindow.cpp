//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"
#include "ktx.h"
#include "Bind.h"

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
	mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/std_vertex.glsl");
	mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/std_fragment.glsl");

	if (mProgram.link() == false)
		std::cout << "Problem linking shaders" << std::endl;

	std::cout << "Initialized shaders" << std::endl;
}


void MainWindow::initTextures()
{
	uniforms.mvpMatrix = mProgram.uniformLocation("mvpMatrix");

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

	mVao.bind();
	{
		mBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		mBuffer.create();
		mBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
		mBuffer.bind();
		mBuffer.allocate(grass_blade, sizeof(grass_blade));
		
		initShaders();

		mProgram.enableAttributeArray(0);
		mProgram.setAttributeBuffer(0, GL_FLOAT, 0, 2);

		initTextures();
	}
	mVao.release();
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

	

	//BIND(mProgram,
	mProgram.bind();
	{
		float currentTime = 3.0;
		float t = (float)currentTime * 0.02f;
		float r = 550.0f;

		QMatrix4x4 mvMatrix;
		mvMatrix.lookAt(QVector3D(sinf(t) * r, 25.0f, cosf(t) * r),
												 QVector3D(0.0f, -50.0f, 0.0f),
												 QVector3D(0.0f, 1.0f, 0.0));
		QMatrix4x4 projMatrix;
		projMatrix.perspective(45.0f, (float)this->width() / (float)this->height(), 0.1f, 1000.0f);

		mProgram.setUniformValue(uniforms.mvpMatrix, projMatrix*mvMatrix);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		mVao.bind();
		{
			const int count = 1024*1024*8;
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, count);
		}
		mVao.release();
	}//)
	mProgram.release();

}