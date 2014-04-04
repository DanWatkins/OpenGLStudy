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


	Mat3 nullRotation(Float elapsedTime)
	{
		return Mat3(1.0f);
	}


	Float computeAngleRad(Float elapsedTime, Float loopDuration)
	{
		Float scale = TWO_PI / loopDuration;
		Float currentTimeThroughLoop = fmodf(elapsedTime, loopDuration);
		
		return currentTimeThroughLoop * scale;
	}


	Mat3 rotateX(Float elapsedTime)
	{
		Float angleRad = computeAngleRad(elapsedTime, 2.0f);
		Float lCos = cosf(angleRad);
		Float lSin = sinf(angleRad);

		Mat3 theMat(1.0f);
		theMat[1].y = lCos; theMat[2].y = -lSin;
		theMat[1].z = lSin; theMat[2].z = lCos;

		return theMat;
	}


	Mat3 rotateY(Float elapsedTime)
	{
		Float angleRad = computeAngleRad(elapsedTime, 2.0f);
		Float lCos = cosf(angleRad);
		Float lSin = sinf(angleRad);

		Mat3 theMat(1.0f);
		theMat[0].x = lCos; theMat[2].x = lSin;
		theMat[0].z = -lSin; theMat[2].z = lCos;

		return theMat;
	}


	Mat3 rotateZ(Float elapsedTime)
	{
		Float angleRad = computeAngleRad(elapsedTime, 2.0f);
		Float lCos = cosf(angleRad);
		Float lSin = sinf(angleRad);

		Mat3 theMat(1.0f);
		theMat[0].x = lCos; theMat[1].x = -lSin;
		theMat[0].y = lSin; theMat[1].y = lCos;

		return theMat;
	}


	Mat3 rotateAxis(Float elapsedTime)
	{
		float fAngRad = computeAngleRad(elapsedTime, 2.0);
		float fCos = cosf(fAngRad);
		float fInvCos = 1.0f - fCos;
		float fSin = sinf(fAngRad);
		float fInvSin = 1.0f - fSin;

		Vec3 axis(1.0f, 1.0f, 1.0f);
		axis = glm::normalize(axis);

		Mat3 theMat(1.0f);
		theMat[0].x = (axis.x * axis.x) + ((1 - axis.x * axis.x) * fCos);
		theMat[1].x = axis.x * axis.y * (fInvCos)-(axis.z * fSin);
		theMat[2].x = axis.x * axis.z * (fInvCos)+(axis.y * fSin);

		theMat[0].y = axis.x * axis.y * (fInvCos)+(axis.z * fSin);
		theMat[1].y = (axis.y * axis.y) + ((1 - axis.y * axis.y) * fCos);
		theMat[2].y = axis.y * axis.z * (fInvCos)-(axis.x * fSin);

		theMat[0].z = axis.x * axis.z * (fInvCos)-(axis.y * fSin);
		theMat[1].z = axis.y * axis.z * (fInvCos)+(axis.x * fSin);
		theMat[2].z = (axis.z * axis.z) + ((1 - axis.z * axis.z) * fCos);

		return theMat;
	}


	Vec3 dynamicNonUniformScale(Float elapsedTime)
	{
		const Float loopDurationX = 3.0f;
		const Float loopDurationY = 5.0f;

		Float x = glm::mix(1.0f, 0.5f, calcLerpFactor(elapsedTime, loopDurationX));
		Float y = 1.0f;
		Float z = glm::mix(1.0f, 10.0f, calcLerpFactor(elapsedTime, loopDurationY));

		return Vec3(x, y, z);
	}


	struct Instance
	{
		typedef Mat3(*RotationFunc)(Float);

		RotationFunc calcRotation;
		Vec3 offset;

		Mat4 constructMatrix(Float elapsedTime)
		{
			const Mat3 &rotationMatrix = calcRotation(elapsedTime);
			Mat4 mat(rotationMatrix);
			mat[3] = Vec4(offset, 1.0f);

			return mat;
		}
	};


	Instance instanceList[] =
	{
		{ nullRotation, Vec3(0.0f, 0.0f, -25.0f) },
		{ rotateX, Vec3(-5.0f, -5.0f, -25.0f) },
		{ rotateY, Vec3(-5.0f, 5.0f, -25.0f) },
		{ rotateZ, Vec3(5.0f, 5.0f, -25.0f) },
		{ rotateAxis, Vec3(5.0f, -5.0f, -25.0f) },
	};


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

	
	#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

	void OpenGLStudy::onUpdate(Double currentTime)
	{
		static const Float clearColor[] = { 1.0f, 1.0f, 0.9f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, clearColor);

		glClearColor(1.0f, 1.0f, 0.9f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(mProgram);
		glBindVertexArray(vao);

		Float elapsedTime = currentTime;
		for (Int n=0; n<ARRAY_COUNT(instanceList); n++)
		{
			Instance &instance = instanceList[n];
			const Mat4 &transformMatrix = instance.constructMatrix(elapsedTime);

			glUniformMatrix4fv(modelToCameraUnif, 1, GL_FALSE, glm::value_ptr(transformMatrix));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
		}

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