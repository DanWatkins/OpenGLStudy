#ifndef _OpenGLStudy_ComputeShader_H
#define _OpenGLStudy_ComputeShader_H

#include <QtGui/QOpenGLShaderProgram>
#include "OpenGLWindow.h"

class ComputeShader : public OpenGLWindow
{
public:
    ComputeShader();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif
