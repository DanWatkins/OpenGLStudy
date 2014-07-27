#include "openglwindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent),
      mUpdatePending(false),
      mAnimating(false),
      mContext(0),
      mDevice(0)
{
    setSurfaceType(QWindow::OpenGLSurface);
}


void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}


void OpenGLWindow::initialize()
{
}


void OpenGLWindow::render()
{
    if (!mDevice)
        mDevice = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    mDevice->setSize(size());

    QPainter painter(mDevice);
    render(&painter);
}


void OpenGLWindow::renderLater()
{
    if (!mUpdatePending)
    {
        mUpdatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}


bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::UpdateRequest:
        {
            mUpdatePending = false;
            renderNow();
            return true;
        }
        default:
        {
            return QWindow::event(event);
        }
    }
}


void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}


void OpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!mContext) {
        mContext = new QOpenGLContext(this);
        mContext->setFormat(requestedFormat());
        mContext->create();

        needsInitialize = true;
    }

    mContext->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    mContext->swapBuffers(this);

    if (mAnimating)
        renderLater();
}


void OpenGLWindow::setAnimating(bool animating)
{
    mAnimating = animating;

    if (animating)
        renderLater();
}


