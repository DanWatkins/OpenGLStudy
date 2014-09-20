//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include "Main.h"
#include <QtGui\QOpenGLFunctions_4_3_Core>

class OpenGLWindow : public QWindow, protected QOpenGLFunctions_4_3_Core
{
public:
	explicit OpenGLWindow(QWindow *parent = 0);

	virtual void render(QPainter *painter);
	virtual void render();

	virtual void initialize();

	void setAnimating(bool animating);

public slots:
	void renderLater();
	void renderNow();

protected:
	bool event(QEvent *event);

	void exposeEvent(QExposeEvent *event);

private:
	bool mUpdatePending;
	bool mAnimating;

	QOpenGLContext *mContext;
	QOpenGLPaintDevice *mDevice;
};

#endif
