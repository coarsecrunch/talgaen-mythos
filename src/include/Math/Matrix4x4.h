#pragma once

#include "Cmn.h"

namespace talga
{

	class Vector4;

	class Matrix4x4 {
	public:
		explicit Matrix4x4(const F32 arr[4][4]);
		Matrix4x4(F32, F32, F32, F32,
			F32, F32, F32, F32,
			F32, F32, F32, F32,
			F32, F32, F32, F32);
		Matrix4x4();
		Matrix4x4(const Matrix4x4& cpy);
		virtual ~Matrix4x4();


		F32* operator[](int index);
		const F32* operator[](int index) const;
		//this accessor array is differnet because it returns  by value, as opposed to operator[]
		F32 operator()(int indexRow, int indexCol) const;

	private:
		static const int SIZE = 4;
		F32 matrix[SIZE][SIZE];

	};

	void PrintMat(const Matrix4x4& mat);
	Matrix4x4 operator*(const Matrix4x4& mat1, const Matrix4x4& mat2);
	Vector4 operator*(const Matrix4x4& mat, const Vector4& vec);
	Matrix4x4 ProjectionPerspectiveMat(F32 near, F32 far, F32 fov, F32 aspectRatio);
	Matrix4x4 PerspectiveProjctionVFrustum(F32 n, F32 f, F32 r, F32 l, F32 t, F32 b);
	Matrix4x4 ViewMat(Vector4 eye, Vector4 target, Vector4 up);
	Matrix4x4 OrthographicProjectionMat(F32 near, F32 far, F32 right, F32 left, F32 top, F32 bottom);
	Matrix4x4 FPSViewRH(Vector4 eye, F32 pitch, F32 yaw);
	Matrix4x4 Transpose(const Matrix4x4& mat);

	typedef Matrix4x4 mat4;

	const F32 identityArr[4][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	const Matrix4x4 IDENTITY_MATRIX(identityArr);
}