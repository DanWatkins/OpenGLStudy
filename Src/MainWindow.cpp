//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"
#include "ktx.h"

#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
static const GLubyte tex_data[] =
{
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
};

static const unsigned texSize = 16;
#undef B
#undef W


void MainWindow::initShaders()
{
	mProgram = new QOpenGLShaderProgram(this);
	mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/std_vertex.glsl");
	mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/std_fragment.glsl");

	if (mProgram->link() == false)
		std::cout << "Problem linking shaders" << std::endl;

	uniforms.mvMatrix = mProgram->uniformLocation("mv_matrix");
	uniforms.projMatrix = mProgram->uniformLocation("proj_matrix");

	std::cout << "Initialized shaders" << std::endl;
}


void MainWindow::initTextures()
{
	glGenTextures(1, &mTexObject[0]);
	glBindTexture(GL_TEXTURE_2D, mTexObject[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, texSize, texSize);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texSize, texSize, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	std::cout << "Loading... KTX" << std::endl;

	mTexObject[1] = sb6::ktx::file::load("Textures/pattern1.ktx");

	std::cout << "Loading... SBM" << std::endl;

	mObject.load("Objects/torus_nrms_tc.sbm");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	std::cout << "Initialized texutres" << std::endl;
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

	static const GLfloat ones[] = { 1.0f };
	glClearBufferfv(GL_DEPTH, 0, ones);

	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);


	mProgram->bind();

	glBindTexture(GL_TEXTURE_2D, mTexObject[mTexIndex]);

	QMatrix4x4 mv;
	mProgram->setUniformValue(uniforms.mvMatrix, mv);

	QMatrix4x4 proj;
	proj.perspective(60.0f, (float)QWindow::width() / (float)QWindow::height(), 0.1f, 1000.0f);
	mProgram->setUniformValue(uniforms.projMatrix, proj);

	mObject.render();

	mProgram->release();
}
