#ifndef TRIANGLETEST_H
#define TRIANGLETEST_H

#include "openglwindow.h"


class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow()
        : m_program(0)
        , m_frame(0)
    {
    }

    void initialize();
    void render();

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // TRIANGLETEST_H
