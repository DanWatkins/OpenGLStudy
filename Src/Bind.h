//==================================================================================================================|
// Created 2014.09.20 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef BIND_H
#define BIND_H

#include <QtGui/qopenglshaderprogram.h>
#include <functional>

class Bind
{
public:
	template<typename W>
	Bind(QOpenGLShaderProgram &program, W &&body)
	{
		boundItem = &program;
		boundItem->bind();

		body();
	}


	~Bind()
	{
		boundItem->release();
	}


private:
	QOpenGLShaderProgram *boundItem;
};


//#define BIND(what) Bind(what, []() -> void


#define BIND(what, body) Bind(what, [this]() -> void body);

#endif
