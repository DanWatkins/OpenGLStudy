//=======================================================================================================================|
// Created 2014.04.09 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "MatrixStack.h"

namespace glz
{
	void MatrixStack::push()
	{
		mMatricies.push(mCurMat);
	}


	void MatrixStack::pop()
	{
		mCurMat = mMatricies.top();
		mMatricies.pop();
	}


	void MatrixStack::rotateX(Float angleDeg)
	{
		Float angleRad = degToRad(angleDeg);
		Float lCos = cosf(angleRad);
		Float lSin = sinf(angleRad);

		Mat4 theMat(1.0f);
		theMat[1].y = lCos; theMat[2].y = -lSin;
		theMat[1].z = lSin; theMat[2].z = lCos;

		mCurMat = mCurMat * theMat;
	}


	void MatrixStack::rotateY(Float angleDeg)
	{
		Float angleRad = degToRad(angleDeg);
		Float lCos = cosf(angleRad);
		Float lSin = sinf(angleRad);

		Mat4 theMat(1.0f);
		theMat[0].x = lCos; theMat[2].x = lSin;
		theMat[0].z = -lSin; theMat[2].z = lCos;

		mCurMat = mCurMat * theMat;
	}


	void MatrixStack::rotateZ(Float angleDeg)
	{
		Float angleRad = degToRad(angleDeg);
		Float lCos = cosf(angleRad);
		Float lSin = sinf(angleRad);

		Mat4 theMat(1.0f);
		theMat[0].x = lCos; theMat[1].x = -lSin;
		theMat[0].y = lSin; theMat[1].y = lCos;

		mCurMat = mCurMat * theMat;
	}


	void MatrixStack::scale(Vec3 scale)
	{
		Mat4 scaleMat(1.0f);
		scaleMat[0].x = scale.x;
		scaleMat[1].y = scale.y;
		scaleMat[2].z = scale.z;

		mCurMat = mCurMat * scaleMat;
	}


	void MatrixStack::translate(Vec3 offset)
	{
		Mat4 translateMat(1.0f);
		translateMat[3] = Vec4(offset, 1.0f);

		mCurMat = mCurMat * translateMat;
	}
};