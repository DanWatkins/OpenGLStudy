//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "MainWindow.h"
#include "ktx.h"
#include "Bind.h"

#define NUMBER_OF_DRAWS 50000

void MainWindow::initShaders()
{
	mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/std_vertex.glsl");
	mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/std_fragment.glsl");

	if (mProgram.link() == false)
		std::cout << "Problem linking shaders" << std::endl;
	else
	{
		uniforms.time = mProgram.uniformLocation("time");
		uniforms.matrixView = mProgram.uniformLocation("matrixView");
		uniforms.matrixProj = mProgram.uniformLocation("matrixProj");
		uniforms.matrixViewProj = mProgram.uniformLocation("matrixViewProj");
	}

	std::cout << "Initialized shaders" << std::endl;
}


void MainWindow::initialize()
{
	OpenGLWindow::initialize();
	initShaders();

	mObject.load("./Objects/asteroids.sbm");

	glGenBuffers(1, &mIndirectDrawBuffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, mIndirectDrawBuffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER,
				 NumberOfDraws * sizeof(DrawArraysIndirectCommand),
				 nullptr, GL_STATIC_DRAW);

	void *range = glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0,
								   NumberOfDraws * sizeof(DrawArraysIndirectCommand),
								   GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	DrawArraysIndirectCommand *cmd = (DrawArraysIndirectCommand*)range;

	for (int i=0; i<NumberOfDraws; i++)
	{
		mObject.get_sub_object_info(i % mObject.get_sub_object_count(),
									cmd[i].first, cmd[i].count);
		cmd[i].primCount = 1;
		cmd[i].baseInstance = i;
	}

	glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);

	glBindVertexArray(mObject.get_vao());
	{
		glGenBuffers(1, &mDrawIndexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mDrawIndexBuffer);
		glBufferData(GL_ARRAY_BUFFER, NumberOfDraws * sizeof(GLuint),
					 NULL, GL_STATIC_DRAW);

		GLuint *drawIndex = (GLuint*)glMapBufferRange(GL_ARRAY_BUFFER, 0,
													  NumberOfDraws * sizeof(GLuint),
													  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int i=0; i<NumberOfDraws; i++)
		{
			drawIndex[i] = i;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);

		glVertexAttribIPointer(10, 1, GL_UNSIGNED_INT, 0, nullptr);
		glVertexAttribDivisor(10, 1);
		glEnableVertexAttribArray(10);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
	}
	glBindVertexArray(0);
}


void MainWindow::render()
{
	OpenGLWindow::render();
	//prepare
	{
		static const GLfloat color[] = { 0.95f, 0.95f, 0.95f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		glClearBufferfv(GL_DEPTH, 0, ones);

		const qreal retinaScale = devicePixelRatio();
		glViewport(0, 0, width() * retinaScale, height() * retinaScale);
	}


	static double currentTime = 0.0;
	currentTime += 0.1;

	static double lastTime = 0.0;
	static double totalTime = 0.0;

	if (!mPaused)
		totalTime += (currentTime - lastTime);

	lastTime = currentTime;

	float t = float(totalTime);
	int i = int(totalTime * 3.0f);

	QMatrix4x4 matrixView;
	{
		QVector3D eye(100.0f * cosf(t * 0.023f), 100.0f * cosf(t * 0.023f), 300.0f * sinf(t * 0.037f) - 600.0f);
		QVector3D center(0.0f, 0.0f, 260.0f);
		QVector3D up(0.1f - cosf(t * 0.1f) * 0.3f, 1.0f, 0.0f);
		up.normalize();

		matrixView.lookAt(eye, center, up);
	}

	QMatrix4x4 matrixProj;
	matrixProj.perspective(50.0f, (float)width() / (float)height(), 1.0f, 2000.0f);


	Bind(mProgram,
	{
		mProgram.setUniformValue(uniforms.matrixView, matrixView);
		mProgram.setUniformValue(uniforms.matrixProj, matrixProj);
		mProgram.setUniformValue(uniforms.matrixViewProj, matrixProj * matrixView);
		
		glBindVertexArray(mObject.get_vao());
		
		printf("mode=%i\n", mMode);


		if (mMode == Mode::MultiDraw)
		{
			glMultiDrawArraysIndirect(GL_TRIANGLES, NULL, NumberOfDraws, 0);
		}
		else if (mMode == Mode::SeparateDraws)
		{
			for (int j=0; j<NumberOfDraws; j++)
			{
				GLuint first;
				GLuint count;

				mObject.get_sub_object_info(j % mObject.get_sub_object_count(), first, count);
				glDrawArraysInstancedBaseInstance(GL_TRIANGLES, first, count, 1, j);
			}
		}
	})
}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
	switch (ev->key())
	{
	case Qt::Key_Escape: QWindow::close(); break;
	case Qt::Key_M:
		mMode = Mode::MultiDraw;
		break;

	case Qt::Key_S:
		mMode = Mode::SeparateDraws;
		break;


	case Qt::Key_Q:
		mMode = Mode::Max;
		break;
	}
}