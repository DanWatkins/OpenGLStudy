#ifndef _OpenGLStudy_ComputeShader_H
#define _OpenGLStudy_ComputeShader_H

#include <QtGui/QOpenGLShaderProgram>
#include "OpenGLWindow.h"

class ComputeShader : public OpenGLWindow
{
public:
    void initialize() override;
    void render() override;

private:
    QOpenGLShaderProgram mProgram;
	
	static const int mNumberOfElements = 2048;

	float mInputData[mNumberOfElements];
	float mOutputData[mNumberOfElements];

	struct Names
	{
		GLuint dataBuffer[2];
	} mNames;
};

#endif
