#ifndef _OpenGLStudy_TessellationRect_H
#define _OpenGLStudy_TessellationRect_H

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <Common/OpenGLWindow.h>

class AppWindow : public OpenGLWindow
{
public:
    void initialize() override;
    void render() override;

private:
    QOpenGLShaderProgram mProgram;
	QOpenGLVertexArrayObject mVao;
};

#endif
