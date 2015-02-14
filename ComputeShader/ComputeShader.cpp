#include <QScreen>
#include "ComputeShader.h"

ComputeShader::ComputeShader() :
    m_frame(0)
{
}


void ComputeShader::initialize()
{
	QOpenGLShader vertexShader(QOpenGLShader::Vertex);
	vertexShader.compileSourceFile("main.vert.glsl");
	mProgram.addShader(&vertexShader);

	QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
	fragmentShader.compileSourceFile("main.frag.glsl");
	mProgram.addShader(&fragmentShader);

	if (!mProgram.link())
		qFatal("Error linking shaders");

    m_posAttr = mProgram.attributeLocation("posAttr");
    m_colAttr = mProgram.attributeLocation("colAttr");
    m_matrixUniform = mProgram.uniformLocation("matrix");
}


void ComputeShader::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    mProgram.bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    mProgram.setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    mProgram.release();

    ++m_frame;
}