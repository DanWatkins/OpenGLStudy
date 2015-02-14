#include <QScreen>
#include <QtCore/QThread>

#include "ComputeShader.h"

static inline float random_float()
{
    static unsigned int seed = 0x13371337;

    float res;
    
    seed *= 16807;
    unsigned int tmp = seed ^ (seed >> 4) ^ (seed << 15);
    *((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}


void ComputeShader::initialize()
{
	//load shaders
	{
		QOpenGLShader vertexShader(QOpenGLShader::Compute);
		vertexShader.compileSourceFile("main.cs.glsl");
		mProgram.addShader(&vertexShader);

		if (!mProgram.link())
			qFatal("Error linking shaders");
	}

	//reserve compute space and generate random data to sum
	{
		glGenBuffers(2, mNames.dataBuffer);

		for (int i=0; i<2; i++)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, mNames.dataBuffer[i]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, mNumberOfElements * sizeof(GLfloat), nullptr, GL_DYNAMIC_COPY);
		}

		for (int i=0; i<mNumberOfElements; i++)
			mInputData[i] = random_float();
	}
}


void ComputeShader::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);
	static const int totalSize = sizeof(float) * mNumberOfElements;

	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, mNames.dataBuffer[0], 0, totalSize);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, totalSize, mInputData);
    
	mProgram.bind();
	{
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, mNames.dataBuffer[1], 0, totalSize);
		glDispatchCompute(1, 1, 1);

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glFinish();

		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, mNames.dataBuffer[1], 0, totalSize);

		if (float *mapped = (float*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, totalSize, GL_MAP_READ_BIT))
		{
			for (int i=0; i<40&&i<mNumberOfElements; i++)
				qDebug() << mapped[i];

			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		}
	}
    mProgram.release();

	QThread::msleep(10000);
}