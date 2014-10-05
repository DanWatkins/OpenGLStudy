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

bool MainWindow::initShaders()
{
	std::cout << "Loading update program..." << std::endl;
	mProgramUpdate.addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/update.vs.glsl");
	mProgramUpdate.addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/update.fs.glsl");
	
	std::cout << "Loading render program..." << std::endl;
	mProgramRender.addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/render.vs.glsl");
	mProgramRender.addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/render.fs.glsl");

	static const char* tfVaryings[] =
	{
		"tfPositionMass",
		"tfVelocity"
	};

	glTransformFeedbackVaryings(mProgramUpdate.programId(), 2, tfVaryings, GL_SEPARATE_ATTRIBS);
	

	if (mProgramUpdate.link() == false ||
		mProgramRender.link() == false)
	{
		std::cout << "Problem linking shaders" << std::endl;
		return false;
	}
	else
	{
		
		std::cout << "Initialized shaders" << std::endl;
		return true;
	}
}


void MainWindow::initialize()
{
	OpenGLWindow::initialize();
	if (initShaders() == false)
	{
		QWindow::close();
	}

	QVector4D *initialPositions = new QVector4D[PointsTotal];
	QVector3D *initialVelocities = new QVector3D[PointsTotal];
	QVector<int> *connectionVectors = new QVector<int>[PointsTotal];

	int n=0;

	for (int j=0; j<PointsY; j++)
	{
		float fj = (float)j / (float)PointsY;

		for (int i=0; i<PointsX; i++)
		{
			float fi = (float)i / (float)PointsX;

			initialPositions[n] = QVector4D((fi - 0.5f) * (float)PointsX,
											(fj - 0.5f) * (float)PointsY,
											0.6f * sinf(fi) * cosf(fj),
											1.0f);
			initialVelocities[n] = QVector3D(0, 0, 0);
			connectionVectors[n] = QVector<int>(4, -1);

			if (j != (PointsY - 1))
			{
				if (i != 0)
					connectionVectors[n][0] = n-1;

				if (j != 0)
					connectionVectors[n][1] = n-PointsX;

				if (i != (PointsX-1))
					connectionVectors[n][2] = n+1;

				if (j != (PointsY-1))
					connectionVectors[n][3] = n+PointsX;
			}

			n++;
		}
	}

	glGenVertexArrays(2, mVao);
	glGenBuffers(5, mVbo);

	for (int i=0; i<2; i++)
	{
		glBindVertexArray(mVao[i]);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo[BufferType::PositionA + i]);
		glBufferData(GL_ARRAY_BUFFER, PointsTotal*sizeof(QVector4D),
					 initialPositions, GL_DYNAMIC_COPY);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo[BufferType::VelocityA + i]);
		glBufferData(GL_ARRAY_BUFFER, PointsTotal*sizeof(QVector3D),
					 initialVelocities, GL_DYNAMIC_COPY);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo[BufferType::Connection]);
		glBufferData(GL_ARRAY_BUFFER, PointsTotal*sizeof(int)*4,
					 connectionVectors, GL_STATIC_DRAW);
		glVertexAttribIPointer(2, 4, GL_INT, 0, NULL);
		glEnableVertexAttribArray(2);
	}

	delete[] connectionVectors;
	delete[] initialVelocities;
	delete[] initialPositions;

	glGenTextures(2, mPosTbo);
	glBindTexture(GL_TEXTURE_BUFFER, mPosTbo[0]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, mVbo[BufferType::PositionA]);
	glBindTexture(GL_TEXTURE_BUFFER, mPosTbo[1]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, mVbo[BufferType::PositionB]);

	int lines = (PointsX-1)*PointsY + (PointsY-1)*PointsX;

	glGenBuffers(1, &mIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lines * 2 * sizeof(int), NULL, GL_STATIC_DRAW);

	int *e = (int*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, lines*2*sizeof(int), 
									GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int j=0; j<PointsY; j++)
	{
		for (int i=0; i<PointsX-1; i++)
		{
			*e++ = i + j*PointsX;
			*e++ = 1 + i + j*PointsX;
		}
	}

	for (int i=0; i<PointsX; i++)
	{
		for (int j=0; j<PointsY-1; j++)
		{
			*e++ = i + j*PointsX;
			*e++ = PointsX + i + j*PointsX;
		}
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}


void MainWindow::render()
{
	OpenGLWindow::render();

	Bind(mProgramUpdate,
	{
		glEnable(GL_RASTERIZER_DISCARD);

		for (int i=mIterationsPerFrame; i!=0; --i)
		{
			glBindVertexArray(mVao[mIterationIndex & 1]);
			glBindTexture(GL_TEXTURE_BUFFER, mPosTbo[mIterationIndex & 1]);
			mIterationIndex++;

			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,
							 mVbo[PositionA + (mIterationIndex & 1)]);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1,
							 mVbo[VelocityA + (mIterationIndex & 1)]);

			glBeginTransformFeedback(GL_POINTS);
			glDrawArrays(GL_POINTS, 0, PointsTotal);
			glEndTransformFeedback();
		}

		glDisable(GL_RASTERIZER_DISCARD);
	})

	//prepare
	{
		static const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		glClearBufferfv(GL_DEPTH, 0, ones);

		const qreal retinaScale = devicePixelRatio();
		glViewport(0, 0, width() * retinaScale, height() * retinaScale);
	}

	Bind(mProgramRender,
	{
		if (mDrawPoints)
		{
			glPointSize(4.0f);
			glDrawArrays(GL_POINTS, 0, PointsTotal);
		}

		if (mDrawLines)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
			glDrawElements(GL_LINES, ConnectionsTotal * 2, GL_UNSIGNED_INT, NULL);
		}
	})
}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
	switch (ev->key())
	{
	case Qt::Key_Escape: QWindow::close(); break;
	}
}