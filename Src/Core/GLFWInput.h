//=======================================================================================================================|
// Created 2013.11.19 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#ifndef _glz_GLFWINPUT_H
#define _glz_GLFWINPUT_H

#include "Core.h"

namespace glz
{
	class OpenGLWindow;

	/**
	 * Used for delegating keyboard input from GLFW to OpenGLWindow
	 * class objects.
	 * Because GLFW is a C API, pointers to class methods cannot be
	 * passed to GLFW. So this static class is needed as a middle man.
	 */
	class GLFWInput
	{
	private:
		static std::vector<OpenGLWindow*> mWindows;

		GLFWInput() {};
		GLFWInput(GLFWInput const&) {}
		void operator=(GLFWInput const&) {}


	public:
		static GLFWInput& instance();
		static void addWindow(OpenGLWindow *window);
		static void keyboardCallback(Int action, Int key, Int a, Int b, Int c);
		static void windowResizeCallback(GLFWwindow *window, Int width, Int height);
	};
};


#endif
