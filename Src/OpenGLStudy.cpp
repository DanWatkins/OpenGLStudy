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
	//=================================================================================================
	// Utility
	//=================================================================================================
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


	Mat4 calcLookAtMatrix(Vec3 cameraPos, Vec3 lookAtPos, Vec3 upPos)
	{
		Vec3 lookDir = glm::normalize(lookAtPos - cameraPos);
		Vec3 upDir = glm::normalize(upPos);

		Vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
		Vec3 perpUpDir = glm::cross(rightDir, lookDir);

		Mat4 rotation(1.0f);
		rotation[0] = Vec4(rightDir, 0.0f);
		rotation[1] = Vec4(perpUpDir, 0.0f);
		rotation[2] = Vec4(-lookDir, 0.0f);

		rotation = glm::transpose(rotation);

		Mat4 translation(1.0f);
		translation[3] = Vec4(-cameraPos, 1.0f);

		return rotation * translation;
	}



	//=================================================================================================
	// Drawing
	//=================================================================================================
	void DrawTree(MatrixStack &modelMatrix, Float trunkHeight=2.0f, Float coneHeight=3.0f)
	{

	}


	void DrawForest(MatrixStack &modelMatrix)
	{

	}


	//=================================================================================================
	// Initialization
	//=================================================================================================
	ProgramData loadProgram(String vertexShader, String fragmentShader)
	{
		Uint shaders[2];
		shaders[0] = Shader::loadShader(String(gDefaultPathShader+vertexShader), GL_VERTEX_SHADER);
		shaders[1] = Shader::loadShader(String(gDefaultPathShader+fragmentShader), GL_FRAGMENT_SHADER);

		Shader::linkFromShaders(shaders, 2);
	}


	void OpenGLStudy::initGlProgram()
	{
		uniformColor = loadProgram("PosOnlyWorldTransform.vert", "ColorUniform.frag");
		objectColor = loadProgram("PosColorWorldTransform.vert", "ColorPassthrough.frag");
		uniformColorTint = loadProgram("PosColorWorldTransform.vert", "ColorMultUniform.frag");
	}


	OpenGLStudy::OpenGLStudy() : cameraToClipMatrix(0.0f)
	{
	}

	
	void OpenGLStudy::onStartup()
	{
		initGlProgram();

		try
		{
			meshCone = new Mesh("UnitConeTint.xml");
			meshCylinder = new Mesh("UnitCylinderTint.xml");
			meshCubeTint = new Mesh("UnitCubeTint.xml");
			meshCubeColor = new Mesh("UnitCubeColor.xml");
			meshPlane = new Mesh("UnitPlane.xml");
		}
		catch (std::exception &e)
		{
			printf("%s\n", e.what());
			throw;
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);
		glEnable(GL_DEPTH_CLAMP);
	}

	

	//=================================================================================================
	// Update
	//=================================================================================================
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