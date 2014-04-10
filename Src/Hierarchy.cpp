//=======================================================================================================================|
// Created 2014.04.09 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "Hierarchy.h"

namespace glz
{
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



	void Hierarchy::drawMainArm(MatrixStack &modelToCameraStack)
	{
		drawLowerArm(modelToCameraStack);
	}


	void Hierarchy::drawLowerArm(MatrixStack &modelToCameraStack)
	{

	}


	void Hierarchy::draw()
	{
		MatrixStack modelToCameraStack;

		drawMainArm(modelToCameraStack);
	}


	void Hierarchy::adjustBase(Bool increment)
	{
	}


	void Hierarchy::adjustMainArm(Bool increment)
	{
	}


	void Hierarchy::adjustLowerArm(Bool increment)
	{
	}
};