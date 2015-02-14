#include <QScreen>
#include <QtCore/QThread>

#include "AppWindow.h"


void AppWindow::initialize()
{
	//load shaders
	{
		QOpenGLShader vs(QOpenGLShader::Vertex);
			vs.compileSourceFile("TessellationRect/main.vs.glsl");
			mProgram.addShader(&vs);

		QOpenGLShader tcs(QOpenGLShader::TessellationControl);
			tcs.compileSourceFile("TessellationRect/main.tcs.glsl");
			mProgram.addShader(&tcs);

		QOpenGLShader tes(QOpenGLShader::TessellationEvaluation);
			tes.compileSourceFile("TessellationRect/main.tes.glsl");
			mProgram.addShader(&tes);

		QOpenGLShader fs(QOpenGLShader::Fragment);
			fs.compileSourceFile("TessellationRect/main.fs.glsl");
			mProgram.addShader(&fs);

		if (!mProgram.link())
			qFatal("Error linking shaders");
	}

	mVao.bind();
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void AppWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);
    
	mProgram.bind();
	{
		mVao.bind();
		glDrawArrays(GL_PATCHES, 0, 4);
	}
    mProgram.release();
}