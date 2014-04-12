//=======================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "OpenGLStudy.h"
#include "./Core/Core.h"

namespace glz
{	
	Float calcFrustumScale(Float fovDeg)
	{
		Float fovRad = degToRad(fovDeg);
		return 1.0f / tan(fovRad / 2.0f);
	}


	Float calcLerpFactor(Float elapsedTime, Float loopDuration)
	{
		Float value = fmodf(elapsedTime, loopDuration) / loopDuration;

		if (value > 0.5f)
			value = 1.0f - value;

		return value * 2.0f;
	}


	Float computeAngleRad(Float elapsedTime, Float loopDuration)
	{
		Float scale = TWO_PI / loopDuration;
		Float currentTimeThroughLoop = fmodf(elapsedTime, loopDuration);
		
		return currentTimeThroughLoop * scale;
	}


	void OpenGLStudy::initGlProgram()
	{

	}


	void OpenGLStudy::initVertexBuffer()
	{
		
	}


	OpenGLStudy::OpenGLStudy() : cameraToClipMatrix(0.0f)
	{
	}

	
	void OpenGLStudy::onStartup()
	{
		initGlProgram();
		initVertexBuffer();
	}

	

	void OpenGLStudy::onUpdate(Double currentTime)
	{
		static const Float clearColor[] = { 1.0f, 1.0f, 0.9f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, clearColor);

	}


	void OpenGLStudy::onKey(Int key, Int action)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
			{
				terminate();
			}
			case GLFW_KEY_EQUAL:
			{
				addZoom(Vec2(0.005f, 0.005f));
				break;
			}
			case GLFW_KEY_M:
			{
				addZoom(Vec2(-0.005f, -0.005f));

				break;
			}
		}
	}

	
	void OpenGLStudy::onResize(Int width, Int height)
	{
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}


	void OpenGLStudy::onTerminate()
	{
	}
};