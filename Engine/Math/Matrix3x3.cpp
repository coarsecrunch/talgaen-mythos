#include "Matrix3x3.h"
#include "Vector3.h"

namespace talga
{

	Matrix3x3::Matrix3x3()
	{
	}

	float* Matrix3x3::operator[](int index)
	{
		return matrix[index];
	}

	Matrix3x3::Matrix3x3(const Matrix3x3& cpy)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				(*this)[i][j] = cpy(i, j);

	}

	float Matrix3x3::operator()(int indexRow, int indexCol) const
	{
		return matrix[indexRow][indexCol];
	}

	Matrix3x3::Matrix3x3(float v00, float v01, float v02,
		float v10, float v11, float v12,
		float v20, float v21, float v22)
	{
		matrix[0][0] = v00;
		matrix[0][1] = v01;
		matrix[0][2] = v02;

		matrix[1][0] = v10;
		matrix[1][1] = v11;
		matrix[1][2] = v12;

		matrix[2][0] = v20;
		matrix[2][1] = v21;
		matrix[2][2] = v22;
	}
	Matrix3x3 operator*(const Matrix3x3& mat1, const Matrix3x3& mat2)
	{
		Matrix3x3 product;
		product[0][0] = mat1(0, 0) * mat2(0, 0) + mat1(0, 1) * mat2(1, 0) + mat1(0, 2) * mat2(2, 0);
		product[0][1] = mat1(0, 0) * mat2(0, 1) + mat1(0, 1) * mat2(1, 1) + mat1(0, 2) * mat2(2, 1);
		product[0][2] = mat1(0, 0) * mat2(0, 2) + mat1(0, 1) * mat2(1, 2) + mat1(0, 2) * mat2(2, 2);

		product[1][0] = mat1(1, 0) * mat2(0, 0) + mat1(1, 1) * mat2(1, 0) + mat1(1, 2) * mat2(2, 0);
		product[1][1] = mat1(1, 0) * mat2(0, 1) + mat1(1, 1) * mat2(1, 1) + mat1(1, 2) * mat2(2, 1);
		product[1][2] = mat1(1, 0) * mat2(0, 2) + mat1(1, 1) * mat2(1, 2) + mat1(1, 2) * mat2(2, 2);

		product[2][0] = mat1(2, 0) * mat2(0, 0) + mat1(2, 1) * mat2(1, 0) + mat1(2, 2) * mat2(2, 0);
		product[2][1] = mat1(2, 0) * mat2(0, 1) + mat1(2, 1) * mat2(1, 1) + mat1(2, 2) * mat2(2, 1);
		product[2][2] = mat1(2, 0) * mat2(0, 2) + mat1(2, 1) * mat2(1, 2) + mat1(2, 2) * mat2(2, 2);

		return product;
	}

	Vector3 operator*(const Matrix3x3& mat, const Vector3& vec)
	{
		Vector3 copy;

		for (int i = 0; i < 3; i++)
		{
			float dotProduct = 0.0f;

			for (int j = 0; j < 3; j++)
			{
				dotProduct += mat(i, j) * vec(j);
			}

			copy[i] = dotProduct;
		}

		return copy;
	}

	Matrix3x3 Transpose(const Matrix3x3& mat)
	{
		Matrix3x3 transposed;

		float temp;

		transposed = mat;

		temp = transposed(1, 0);
		transposed[1][0] = transposed[0][1];
		transposed[0][1] = temp;

		temp = transposed(2, 0);
		transposed[2][0] = transposed[0][2];
		transposed[0][2] = temp;

		temp = transposed(2, 1);
		transposed[2][1] = transposed[1][2];
		transposed[1][2] = temp;
		return transposed;
	}

	Matrix3x3 GetOrtho(float r, float l, float t, float b, float n, float f)
	{
		return mat3();
	}

	Matrix3x3::~Matrix3x3()
	{

	}
}