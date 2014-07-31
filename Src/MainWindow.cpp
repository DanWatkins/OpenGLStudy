//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"
#include "Object.h"

void generateTexture(float *data, unsigned textureSize)
{
	unsigned x, y;

	for (y = 0; y < textureSize; y++)
	{
		for (x = 0; x < textureSize; x++)
		{
			data[(y * textureSize + x) * 4 + 0] = (float)((x & y) & 0xFF) / 63.0f;
			data[(y * textureSize + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
			data[(y * textureSize + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 127.0f;
			data[(y * textureSize + x) * 4 + 3] = 1.0f;
		}
	}
}


void MainWindow::initialize()
{
	OpenGLWindow::initialize();

	sb6::object obj;
	obj.load("hey");

	//load shaders
	{
		mProgram = new QOpenGLShaderProgram(this);
		mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/std_vertex.glsl");
		mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/std_fragment.glsl");

		if (mProgram->link() == false)
			std::cout << "Problem linking shaders" << std::endl;
	}

	//load texture
	{
		const unsigned TEXTURE_SIZE = 256;

		//reserve space
		glGenTextures(1, &mTexture);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);

		//generate and upload texture data
		float *data = new float[TEXTURE_SIZE * TEXTURE_SIZE * sizeof(float)];
		generateTexture(data, TEXTURE_SIZE);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TEXTURE_SIZE, TEXTURE_SIZE, GL_RGBA, GL_FLOAT, data);
		delete [] data;
	}

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
}


void MainWindow::render()
{
	OpenGLWindow::render();

	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);


	mProgram->bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);

	mProgram->release();
}
