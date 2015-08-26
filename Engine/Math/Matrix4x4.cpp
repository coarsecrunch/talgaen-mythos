#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Cmn.h"
#include <cmath>
#include <iostream>

namespace talga
{
	Matrix4x4::Matrix4x4(const float arr[4][4]) : matrix{}
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				matrix[i][j] = arr[i][j];
			}
		}
	}

	Matrix4x4::Matrix4x4()
	{
		matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
		matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
		matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
		matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
	}

	Matrix4x4::Matrix4x4(F32 val1, F32 val2, F32 val3, F32 val4,
		F32 val5, F32 val6, F32 val7, F32 val8,
		F32 val9, F32 val10, F32 val11, F32 val12,
		F32 val13, F32 val14, F32 val15, F32 val16)
	{
		matrix[0][0] = val1; matrix[0][1] = val2; matrix[0][2] = val3; matrix[0][3] = val4;
		matrix[1][0] = val5; matrix[1][1] = val6; matrix[1][2] = val7; matrix[1][3] = val8;
		matrix[2][0] = val9; matrix[2][1] = val10; matrix[2][2] = val11; matrix[2][3] = val12;
		matrix[3][0] = val13; matrix[3][1] = val14; matrix[3][2] = val15; matrix[3][3] = val16;
	}

	const F32* Matrix4x4::operator[](int index) const
	{
		return matrix[index];
	}

	void PrintMat(const Matrix4x4& mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::cout << "arr[" << i << "][" << j << "]:" << mat(i, j) << "  ";
			}
			std::cout << std::endl;
		}
	}

	float* Matrix4x4::operator[](int index)
	{
		return matrix[index];
	}

	Matrix4x4::Matrix4x4(const Matrix4x4& cpy) : matrix{}
	{

		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				matrix[i][j] = cpy(i, j);
			}
		}
	}



	float Matrix4x4::operator ()(int indexRow, int indexCol) const
	{
		return matrix[indexRow][indexCol];
	}

	Matrix4x4 operator *(const Matrix4x4& mat1, const Matrix4x4& mat2)
	{
		Matrix4x4 product;
		product[0][0] = mat1(0, 0) * mat2(0, 0) + mat1(0, 1) * mat2(1, 0) + mat1(0, 2) * mat2(2, 0) + mat1(0, 3) * mat2(3, 0);
		product[0][1] = mat1(0, 0) * mat2(0, 1) + mat1(0, 1) * mat2(1, 1) + mat1(0, 2) * mat2(2, 1) + mat1(0, 3) * mat2(3, 1);
		product[0][2] = mat1(0, 0) * mat2(0, 2) + mat1(0, 1) * mat2(1, 2) + mat1(0, 2) * mat2(2, 2) + mat1(0, 3) * mat2(3, 2);
		product[0][3] = mat1(0, 0) * mat2(0, 3) + mat1(0, 1) * mat2(1, 3) + mat1(0, 2) * mat2(2, 3) + mat1(0, 3) * mat2(3, 3);

		product[1][0] = mat1(1, 0) * mat2(0, 0) + mat1(1, 1) * mat2(1, 0) + mat1(1, 2) * mat2(2, 0) + mat1(1, 3) * mat2(3, 0);
		product[1][1] = mat1(1, 0) * mat2(0, 1) + mat1(1, 1) * mat2(1, 1) + mat1(1, 2) * mat2(2, 1) + mat1(1, 3) * mat2(3, 1);
		product[1][2] = mat1(1, 0) * mat2(0, 2) + mat1(1, 1) * mat2(1, 2) + mat1(1, 2) * mat2(2, 2) + mat1(1, 3) * mat2(3, 2);
		product[1][3] = mat1(1, 0) * mat2(0, 3) + mat1(1, 1) * mat2(1, 3) + mat1(1, 2) * mat2(2, 3) + mat1(1, 3) * mat2(3, 3);

		product[2][0] = mat1(2, 0) * mat2(0, 0) + mat1(2, 1) * mat2(1, 0) + mat1(2, 2) * mat2(2, 0) + mat1(2, 3) * mat2(3, 0);
		product[2][1] = mat1(2, 0) * mat2(0, 1) + mat1(2, 1) * mat2(1, 1) + mat1(2, 2) * mat2(2, 1) + mat1(2, 3) * mat2(3, 1);
		product[2][2] = mat1(2, 0) * mat2(0, 2) + mat1(2, 1) * mat2(1, 2) + mat1(2, 2) * mat2(2, 2) + mat1(2, 3) * mat2(3, 2);
		product[2][3] = mat1(2, 0) * mat2(0, 3) + mat1(2, 1) * mat2(1, 3) + mat1(2, 2) * mat2(2, 3) + mat1(2, 3) * mat2(3, 3);

		product[3][0] = mat1(3, 0) * mat2(0, 0) + mat1(3, 1) * mat2(1, 0) + mat1(3, 2) * mat2(2, 0) + mat1(3, 3) * mat2(3, 0);
		product[3][1] = mat1(3, 0) * mat2(0, 1) + mat1(3, 1) * mat2(1, 1) + mat1(3, 2) * mat2(2, 1) + mat1(3, 3) * mat2(3, 1);
		product[3][2] = mat1(3, 0) * mat2(0, 2) + mat1(3, 1) * mat2(1, 2) + mat1(3, 2) * mat2(2, 2) + mat1(3, 3) * mat2(3, 2);
		product[3][3] = mat1(3, 0) * mat2(0, 3) + mat1(3, 1) * mat2(1, 3) + mat1(3, 2) * mat2(2, 3) + mat1(3, 3) * mat2(3, 3);


		return product;
	}

	Vector4 operator*(const Matrix4x4& mat, const Vector4& vec)
	{
		return vec4(
			mat[0][0] * vec[0] + mat[0][1] * vec[1] + mat[0][2] * vec[2] + mat[0][3] * vec[3],
			mat[1][0] * vec[0] + mat[1][1] * vec[1] + mat[1][2] * vec[2] + mat[1][3] * vec[3],
			mat[2][0] * vec[0] + mat[2][1] * vec[1] + mat[2][2] * vec[2] + mat[2][3] * vec[3],
			mat[3][0] * vec[0] + mat[3][1] * vec[1] + mat[3][2] * vec[2] + mat[3][3] * vec[3]
			);
	}

	vec3 operator*(const Matrix4x4& mat, const vec3& vec)
	{
		return vec3(
			mat[0][0] * vec[0] + mat[0][1] * vec[1] + mat[0][2] * vec[2] + mat[0][3],
			mat[1][0] * vec[0] + mat[1][1] * vec[1] + mat[1][2] * vec[2] + mat[1][3],
			mat[2][0] * vec[0] + mat[2][1] * vec[1] + mat[2][2] * vec[2] + mat[2][3]
			);
	}

	Matrix4x4 ProjectionPerspectiveMat(float near, float far, float fov, float aspectRatio)
	{
		Matrix4x4 mat;
		mat[0][0] = 1 / (std::tan(fov * 0.5f) * aspectRatio);
		mat[1][1] = 1 / std::tan(fov * 0.5f);
		mat[2][2] = (far + near) / (near - far);
		mat[2][3] = -(2 * far * near) / (near - far);
		mat[3][2] = -1;
		mat[3][3] = 0;

		return mat;
	}

	Matrix4x4 PerspectiveProjctionVFrustum(float n, float f, float r, float l, float t, float b)
	{
		float projectArr[4][4] = {
			{ (2 * n) / (r - l), 0.0f, (r + l) / (r - l), 0.0f },
			{ 0.0f, (2 * n) / (t - b), (t + b) / (t - b), 0.0f },
			{ 0.0f, 0.0f, -(f + n) / (f - n), (-2 * f*n) / (f - n) },
			{ 0.0f, 0.0f, -1.0f, 0.0f },
		};

		/*float projectArr[4][4] = {
					{(2 * n) / (r - l), 0.0f, 0.0f, 0.0f},
					{0.0f, (2*n) / (t - b), 0.0f, 0.0f},
					{(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1.0f},
					{0.0f, 0.0f, (-2*f*n) / (f - n), 0.0f},
					};*/

		return Matrix4x4(projectArr);
	}

	Matrix4x4 OrthographicProjectionMat(float near, float far, float right, float left, float top, float bottom)
	{
		mat4 mat;
		mat[0][0] = 2.0f / (right - left);
		mat[1][1] = 2.0f / (top - bottom);
		mat[2][2] = -2.0f / (far - near);
		mat[0][3] = -(right + left) / (right - left);
		mat[1][3] = -(top + bottom) / (top - bottom);
		mat[2][3] = -(far + near) / (far - near);
		mat[3][3] = 1;

		return mat;
	}

	Matrix4x4 ViewMat(vec4 eye, vec4 target, vec4 up)
	{
		vec4 zaxis = normalize(eye - target);
		vec4 xaxis = normalize(cross(up, zaxis));
		vec4 yaxis = cross(zaxis, xaxis);

		float viewArr[4][4] = {
			{ xaxis[0], xaxis[1], xaxis[2], dot(xaxis, eye) },
			{ yaxis[0], yaxis[1], yaxis[2], dot(yaxis, eye) },
			{ zaxis[0], zaxis[1], zaxis[2], dot(eye, zaxis) },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		/*float viewArr[4][4] = {
					{xaxis[0], yaxis[0], zaxis[0], 0.0f},
					{xaxis[1], yaxis[1], zaxis[1], 0.0f},
					{xaxis[2], yaxis[2], zaxis[2], 0.0f},
					{-dot(xaxis, -eye[1], eye[2], 1.0f}
					};*/

		/*float viewArr[4][4] = {
					{xaxis[0], xaxis[1], xaxis[2], 0.0f},
					{yaxis[0], yaxis[1], yaxis[2], 0.0f},
					{zaxis[0], zaxis[1], zaxis[2], 0.0f},
					{-dot(xaxis, eye), -dot(yaxis, eye), -dot(eye, zaxis), 0.0f}
					};*/


		Matrix4x4 view(viewArr);

		return view;
	}

	Matrix4x4 FPSViewRH(vec4 eye, float pitch, float yaw)
	{
		// If the pitch and yaw angles are in degrees,
		// they need to be converted to radians. Here
		// I assume the values are already converted to radians.
		float cosPitch = cos(pitch);
		float sinPitch = sin(pitch);
		float cosYaw = cos(yaw);
		float sinYaw = sin(yaw);

		vec4 xaxis = { cosYaw, 0, -sinYaw, 0 };
		vec4 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch, 0 };
		vec4 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw, 0 };

		// Create a 4x4 view matrix from the right, up, forward and eye position vectors
		float viewMatrix[4][4] = {
			{ xaxis(0), yaxis(0), zaxis(0), 0 },
			{ xaxis(1), yaxis(1), zaxis(1), 0 },
			{ xaxis(2), yaxis(2), zaxis(2), 0 },
			{ -dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1 }
		};


		return Transpose(mat4(viewMatrix));
	}

	Matrix4x4 Transpose(const Matrix4x4& mat)
	{
		mat4 transposedMat;
		transposedMat[0][1] = mat(1, 0);
		transposedMat[0][2] = mat(2, 0);
		transposedMat[0][3] = mat(3, 0);
		transposedMat[1][0] = mat(0, 1);
		transposedMat[1][2] = mat(2, 1);
		transposedMat[1][3] = mat(3, 1);
		transposedMat[2][0] = mat(0, 2);
		transposedMat[2][1] = mat(1, 2);
		transposedMat[2][3] = mat(3, 2);
		transposedMat[3][0] = mat(0, 3);
		transposedMat[3][1] = mat(1, 3);
		transposedMat[3][2] = mat(2, 3);

		transposedMat[0][0] = mat(0, 0);
		transposedMat[1][1] = mat(1, 1);
		transposedMat[2][2] = mat(2, 2);
		transposedMat[3][3] = mat(3, 3);
		return transposedMat;
	}


	Matrix4x4::~Matrix4x4()
	{
	}

}
