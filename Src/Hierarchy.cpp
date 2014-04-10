//=======================================================================================================================|
// Created 2014.04.09 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "Hierarchy.h"
#include "OpenGLStudy.h"

namespace glz
{
	inline float clamp(float Value, float minValue, float maxValue)
	{
		if (Value < minValue)
			return minValue;

		if (Value > maxValue)
			return maxValue;

		return Value;
	}


	Hierarchy::Hierarchy() : mBasePos(Vec3(3.0f, -5.0f, -40.0f)),
							mBaseLeftPos(Vec3(2.0f, 0.0f, 0.0f)),
							mBaseRightPos(Vec3(-2.0f, 0.0f, 0.0f)),
							mBaseAngle(-45.0f),
							mBaseScaleZ(3.0f),

							mMainArmAngle(-33.75f),
							mMainArmSize(9.0f),

							mLowerArmPos(Vec3(0.0f, 0.0f, 8.0f)),
							mLowerArmAngle(146.25f),
							mLowerArmLength(5.0f),
							mLowerArmWidth(1.5f)
	{
	}


	void Hierarchy::drawBase(MatrixStack &modelToCameraStack)
	{
		modelToCameraStack.translate(mBasePos);
		modelToCameraStack.rotateY(mBaseAngle);

		//draw left base
		{
			modelToCameraStack.push();
			modelToCameraStack.translate(mBaseLeftPos);
			modelToCameraStack.scale(Vec3(1.0f, 1.0f, mBaseScaleZ));

			glUniformMatrix4fv(mRenderData.mUnif_modelToCamera, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

			modelToCameraStack.pop();
		}

		//draw right base
		{
			modelToCameraStack.push();
			modelToCameraStack.translate(mBaseRightPos);
			modelToCameraStack.scale(Vec3(1.0f, 1.0f, mBaseScaleZ));

			glUniformMatrix4fv(mRenderData.mUnif_modelToCamera, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

			modelToCameraStack.pop();
		}

		drawMainArm(modelToCameraStack);
	}


	void Hierarchy::drawMainArm(MatrixStack &modelToCameraStack)
	{
		modelToCameraStack.push();
		modelToCameraStack.rotateX(mMainArmAngle);

		//draw main arm
		{
			modelToCameraStack.push();
			modelToCameraStack.translate(Vec3(0.0f, 0.0f, (mMainArmSize/2.0f)-1.0f));
			modelToCameraStack.scale(Vec3(1.0f, 1.0f, mMainArmSize/2.0f));

			glUniformMatrix4fv(mRenderData.mUnif_modelToCamera, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

			modelToCameraStack.pop();
		}

		drawLowerArm(modelToCameraStack);

		modelToCameraStack.pop();
	}


	void Hierarchy::drawLowerArm(MatrixStack &modelToCameraStack)
	{
		modelToCameraStack.push();
		modelToCameraStack.translate(mLowerArmPos);
		modelToCameraStack.rotateX(mLowerArmAngle);

		//draw lower arm
		{
			modelToCameraStack.push();
			modelToCameraStack.translate(Vec3(0.0f, 0.0f, mLowerArmLength/2.0f));
			modelToCameraStack.scale(Vec3(mLowerArmWidth/2.0f, mLowerArmWidth/2.0f, mLowerArmWidth/2.0f));

			glUniformMatrix4fv(mRenderData.mUnif_modelToCamera, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

			modelToCameraStack.pop();
		}

		modelToCameraStack.pop();
	}


	void Hierarchy::draw(RenderData &renderData)
	{
		MatrixStack modelToCameraStack;
		mRenderData = renderData;

		glUseProgram(mRenderData.mProgram);
		glBindVertexArray(mRenderData.mVao);

		drawBase(modelToCameraStack);

		glUseProgram(0);
		glBindVertexArray(0);
	}


	#define STANDARD 11.25f
	#define SMALL 9.0f

	void Hierarchy::adjustBase(Bool increment)
	{
		mBaseAngle += increment ? STANDARD : -STANDARD;
		mBaseAngle = fmodf(mBaseAngle, 360.0f);
	}


	void Hierarchy::adjustMainArm(Bool increment)
	{
		mMainArmAngle += increment ? STANDARD : -STANDARD;
		mMainArmAngle = clamp(mMainArmAngle, -90.0f, 0.0f);
	}


	void Hierarchy::adjustLowerArm(Bool increment)
	{
		mLowerArmAngle += increment ? STANDARD : -STANDARD;
		mLowerArmAngle = clamp(mLowerArmAngle, 0.0f, 146.0f);
	}
};