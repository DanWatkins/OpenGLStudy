//=======================================================================================================================|
// Created 2014.04.09 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#ifndef _glz_HIERARCHY_H
#define _glz_HIERARCHY_H

#include "Main.h"

namespace glz
{
	struct RenderData
	{
		Uint program;
		Uint vao;
		GLshort *indexData;

		//uniform locations
		Uint unif_modelToCamera;

		RenderData() : program(0), vao(0), indexData(0), unif_modelToCamera(0) {}
	};



	class Hierarchy
	{
	private:
		RenderData mRenderData;

		//base
		Vec3 mBasePos, mBaseLeftPos, mBaseRightPos;
		Float mBaseAngle;
		Float mBaseScaleZ;
		
		//main arm
		Float mMainArmAngle;
		Float mMainArmSize;

		//lower arm
		Vec3 mLowerArmPos;
		Float mLowerArmAngle;
		Float mLowerArmLength;
		Float mLowerArmWidth;


	private:
		void drawBase(MatrixStack &modelToCameraStack);
		void drawMainArm(MatrixStack &modelToCameraStack);
		void drawLowerArm(MatrixStack &modelToCameraStack);

	public:
		Hierarchy();

		void draw(RenderData &renderData);

		void adjustBase(Bool increment);
		void adjustMainArm(Bool increment);
		void adjustLowerArm(Bool increment);
	};
};

#endif

