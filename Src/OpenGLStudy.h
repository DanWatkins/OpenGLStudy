//=======================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#ifndef _OPEN_GL_STUDY_H
#define _OPEN_GL_STUDY_H

#include "Main.h"
#include "./Core/OpenGLWindow.h"

namespace glz
{
	/**
	 * Represents the main application
	 */
	class OpenGLStudy : public OpenGLWindow
	{
	private:
		Uint modelToCameraUnif = -1;
		Uint cameraToClipMatrixUnif = -1;

		Uint vbo, ibo, vao;
		Mat4 cameraToClipMatrix;


	private:
		void onStartup();
		void onUpdate(Double currentTime);
		void onKey(Int key, Int action);
		void onResize(Int width, Int height);
		void onTerminate();

		void initGlProgram();
		void initVertexBuffer();

	public:
		OpenGLStudy();
	};
};


#endif

