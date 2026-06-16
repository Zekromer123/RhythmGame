#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Mathematics {
public:
	static Matrix4x4 MakeIdentity();
	static Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
	static Matrix4x4 MakeTranslateMatrix(Vector3 translate);
	static Matrix4x4 MakeRotateXMatrix(float theta);
	static Matrix4x4 MakeRotateYMatrix(float theta);
	static Matrix4x4 MakeRotateZMatrix(float theta);
	static Matrix4x4 MakeScaleMatrix(Vector3 scale);
	static Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate);
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
};