//=======================================================================================================================|
// Created 2014.04.09 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#ifndef _glz_MATRIX_STACK_H
#define _glz_MATRIX_STACK_H

#include "../Main.h"
#include <stack>

namespace glz
{
	class MatrixStack
	{
	private:
		Mat4 mCurMat;
		std::stack<Mat4> mMatricies;


	public:
		MatrixStack() : mCurMat(1.0f) {}

		const Mat4 &top() { return mCurMat; }
		void push();
		void pop();

		void rotateX(Float angleDeg);
		void rotateY(Float angleDeg);
		void rotateZ(Float angleDeg);
		void scale(Vec3 scale);
		void translate(Vec3 offset);
	};
};


#endif

