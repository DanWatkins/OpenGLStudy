#ifndef _OpenGLStudy_OpenGLWindow_H
#define _OpenGLStudy_OpenGLWindow_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions_4_3_Core>

#include "Helper.h"

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
    bool mUpdatePending;
    bool mAnimating;

    QOpenGLContext *mContext;
    QOpenGLPaintDevice *mDevice;
};

#endif
