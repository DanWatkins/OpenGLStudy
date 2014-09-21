//==================================================================================================================|
// Created 2014.09.20 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef BIND_H
#define BIND_H

#include <QtGui/qopenglshaderprogram.h>

template<typename T>
class _AutoBind
{
public:
	_AutoBind(T &program)
	{
		boundItem = &program;
		boundItem->bind();
	}


	~_AutoBind()
	{
		boundItem->release();
	}


private:
	_AutoBind() {}

	T *boundItem;
};


#define Bind(what, body) { _AutoBind<decltype(what)> _bound(what); body }

#endif
