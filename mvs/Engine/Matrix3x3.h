#pragma once

namespace talga
{

	class Vector3;

	class Matrix3x3 {
	public:
		Matrix3x3(float v00, float v01, float v02,
			float v10, float v11, float v12,
			float v20, float v21, float v22 = 1.0f);
		Matrix3x3();
		Matrix3x3(const Matrix3x3& cpy);
		~Matrix3x3();


		float* operator[](int index);
		//this accessor array is differnet because it returns  by value, as opposed to operator[]
		float operator()(int indexRow, int indexCol) const;

	private:
		float matrix[3][3];

	};

	Matrix3x3 operator*(const Matrix3x3& mat1, const Matrix3x3& mat2);
	Vector3 operator*(const Matrix3x3& mat, const Vector3& vec);
	Matrix3x3 Transpose(const Matrix3x3& mat);

	Matrix3x3 GetOrtho(float r, float l, float t, float b, float n, float f);

	typedef Matrix3x3 mat3;
}