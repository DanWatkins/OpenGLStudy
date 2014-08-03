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

	uniforms.mvp = mProgram->uniformLocation("mvp");
	uniforms.offset = mProgram->uniformLocation("offset");

	std::cout << "Initialized shaders" << std::endl;
}


void MainWindow::initTextures()
{
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	mTextures[TextureWall] = sb6::ktx::File::load("Textures/brick.ktx");
	mTextures[TextureCeiling] = sb6::ktx::File::load("Textures/ceiling.ktx");
	mTextures[TextureFloor] = sb6::ktx::File::load("Textures/floor.ktx");

	for (int n=0; n<3; ++n)
	{
		glBindTexture(GL_TEXTURE_2D, mTextures[n]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindVertexArray(0);
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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	mProgram->bind();
	{
		renderTunnel();
	}
	mProgram->release();
}


void MainWindow::renderTunnel()
{
	glBindVertexArray(mVao);

	static float offset = 0.0f;
	offset += 0.0001f;
	mProgram->setUniformValue(uniforms.offset, offset);

	QMatrix4x4 matProjection;
	matProjection.perspective(60.0f, (float)width() / (float)height(), 0.1f, 100.0f);

	GLuint sceneTextures[] = { TextureWall, TextureFloor, TextureWall, TextureCeiling };
	for (int n=0; n<4; n++)
	{
		QMatrix4x4 matMV;
		matMV.rotate(90.0f * (float)n, QVector3D(0.0f, 0.0f, 1.0f));
		matMV.translate(-0.5f, 0.0f, -10.0f);
		matMV.rotate(90.0f, 0.0f, 1.0f, 0.0f);
		matMV.scale(30.0f, 1.0f, 1.0f);

		QMatrix4x4 matMVP = matProjection * matMV;
		mProgram->setUniformValue(uniforms.mvp, matMVP);

		glBindTexture(GL_TEXTURE_2D, mTextures[sceneTextures[n]]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}
