//=======================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "OpenGLStudy.h"
#include "./Core/Core.h"
#include "Hierarchy.h"

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




	const Float frustumScale = calcFrustumScale(45.0f);

	void OpenGLStudy::initGlProgram()
	{
		//load shaders
		Uint shaders[2];
		shaders[0] = Shader::loadShader(String(gDefaultPathShaders) + "standard.vert", GL_VERTEX_SHADER);
		shaders[1] = Shader::loadShader(String(gDefaultPathShaders) + "standard.frag", GL_FRAGMENT_SHADER);

		mProgram = Shader::linkFromShaders(shaders, 2);

		
		//configure view matrices
		modelToCameraUnif = glGetUniformLocation(mProgram, "modelToCameraMatrix");
		cameraToClipMatrixUnif = glGetUniformLocation(mProgram, "cameraToClipMatrix");

		Float zNear = 1.0f;
		Float zFar = 61.0f;


		cameraToClipMatrix[0].x = frustumScale;
		cameraToClipMatrix[1].y = frustumScale;
		cameraToClipMatrix[2].z = (zFar+zNear) / (zNear-zFar);
		cameraToClipMatrix[2].w = -1.0f;
		cameraToClipMatrix[3].z = (2*zFar*zNear) / (zNear-zFar);

		glUseProgram(mProgram);
		glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
		glUseProgram(0);
	}


	void OpenGLStudy::initVertexBuffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}




	/*=================================================================================================
	   OpenGLWindow Specific
	=================================================================================================*/
	OpenGLStudy::OpenGLStudy() : cameraToClipMatrix(0.0f)
	{
	}

	
	void OpenGLStudy::onStartup()
	{
		sf::Clock seedTime;
		srand((Uint)seedTime.getElapsedTime().asMicroseconds());

		initGlProgram();
		initVertexBuffer();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		size_t colorDataOffset = sizeof(Float) * 3 * numberOfVertices;
		glBindBuffer(GL_ARRAY_BUFFER, vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBindVertexArray(0);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);
	}

	

	void OpenGLStudy::onUpdate(Double currentTime)
	{
		static const Float clearColor[] = { 1.0f, 1.0f, 0.9f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, clearColor);

		glClearColor(1.0f, 1.0f, 0.9f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(mProgram);
		glBindVertexArray(vao);

		

		glBindVertexArray(0);
		glUseProgram(0);
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