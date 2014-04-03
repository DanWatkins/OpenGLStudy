//=======================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "Main.h"
#include "OpenGLStudy.h"

using namespace glz;

Int main()
{
	OpenGLStudy *app = new OpenGLStudy;
	return app->init(1600, 900, gWindowTitle);
}