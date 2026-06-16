#include "Mathematics.h"
#define _USE_MATH_DEFINES
#include <math.h>

Matrix4x4 Mathematics::MakeIdentity() {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
	return result;
}
Matrix4x4 Mathematics::Multiply(Matrix4x4 m1, Matrix4x4 m2) 
{
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}
Matrix4x4 Mathematics::MakeTranslateMatrix( Vector3 translate) {
	Matrix4x4 result = MakeIdentity();
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 Mathematics::MakeRotateXMatrix( float theta) {
	Matrix4x4 result = MakeIdentity();
	float rad = theta * (float)M_PI / 180.0f;
	result.m[1][1] = cosf(rad);
	result.m[1][2] = sinf(rad);
	result.m[2][1] = -sinf(rad);
	result.m[2][2] = cosf(rad);
	return result;
}

Matrix4x4 Mathematics::MakeRotateYMatrix( float theta) 
{
	Matrix4x4 result = MakeIdentity();
	float rad = theta;
	result.m[0][0] = cosf(rad);
	result.m[0][2] = -sinf(rad);
	result.m[2][0] = sinf(rad);
	result.m[2][2] = cosf(rad);
	return result;
}

Matrix4x4 Mathematics::MakeRotateZMatrix( float theta)
{
	Matrix4x4 result = MakeIdentity();
	float rad = theta * (float)M_PI / 180.0f;
	result.m[0][0] = cosf(rad);
	result.m[0][1] = sinf(rad);
	result.m[1][0] = -sinf(rad);
	result.m[1][1] = cosf(rad);
	return result;
}

Matrix4x4 Mathematics::MakeScaleMatrix( Vector3 scale) {
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 Mathematics::MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) 
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 result = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	return result;
}

Matrix4x4 Mathematics::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) 
{
	
		Matrix4x4 result = {};
		result.m[0][0] = 2.0f / (right - left);
		result.m[1][1] = 2.0f / (top - bottom);
		result.m[2][2] = 1.0f / (farClip - nearClip);
		result.m[3][0] = -(right + left) / (right - left);
		result.m[3][1] = -(top + bottom) / (top - bottom);
		result.m[3][2] = -nearClip / (farClip - nearClip);
		result.m[3][3] = 1.0f;
		return result;
	}
