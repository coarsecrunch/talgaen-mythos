#include "Math\Vector4.h"
#include <cmath>

namespace talga
{
	Vector4::Vector4(float f, float f1, float f2, float f3)
	{
		vec[0] = f;
		vec[1] = f1;
		vec[2] = f2;
		vec[3] = f3;
	}

	Vector4::~Vector4()
	{
	}

	Vector4& Vector4::operator =(const Vector4& copy)
	{
		this->vec[0] = copy(0);
		this->vec[1] = copy(1);
		this->vec[2] = copy(2);
		this->vec[3] = copy(3);

		return *this;
	}


	float& Vector4::operator[](int index)
	{
		return vec[index];
	}

	const float& Vector4::operator[](int index) const
	{
		return vec[index];
	}

	float Vector4::operator ()(int index) const
	{
		return vec[index];
	}

	Vector4 operator *(const Vector4& vec, float scalar)
	{
		vec4 scaledVec = vec;
		scaledVec[0] *= scalar;
		scaledVec[1] *= scalar;
		scaledVec[2] *= scalar;
		return scaledVec;
	}

	Vector4 operator +(const Vector4& vec1, const Vector4& vec2)
	{
		Vector4 newVec;
		newVec[0] = vec1(0) + vec2(0);
		newVec[1] = vec1(1) + vec2(1);
		newVec[2] = vec1(2) + vec2(2);
		newVec[3] = 0;
		return newVec;
	}

	float Vector4::mag() const
	{
		return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	}

	float Vector4::dx() const
	{
		return (*this)(0) / mag();
	}

	float Vector4::dy() const
	{
		return (*this)(1) / mag();
	}

	float Vector4::dz() const
	{
		return (*this)(2) / mag();
	}



	Vector4 normalize(const Vector4& vec)
	{
		Vector4 newVec = vec;
		float mag = vec.mag();
		if (mag == 0.0f)
			mag = 0.00000001f;

		newVec[0] = vec(0) / mag;
		newVec[1] = vec(1) / mag;
		newVec[2] = vec(2) / mag;
		newVec[3] = 0;

		return newVec;
	}

	Vector4 cross(const Vector4& vec1, const Vector4& vec2)
	{
		Vector4 newVec;
		newVec[0] = vec1(1) * vec2(2) - vec1(0) * vec2(1);
		newVec[1] = vec1(2) * vec2(0) - vec1(0) * vec2(2);
		newVec[2] = vec1(0) * vec2(1) - vec1(1) * vec2(0);
		newVec[3] = 0;
		return newVec;
	}

	Vector4 operator-(const Vector4& vec1, const Vector4& vec2)
	{
		Vector4 newVec;
		newVec[0] = vec1(0) - vec2(0);
		newVec[1] = vec1(1) - vec2(1);
		newVec[2] = vec1(2) - vec2(2);
		newVec[3] = 0;
		return newVec;
	}

	Vector4 vecBetween(vec4 point1, vec4 point2)
	{
		float x = point2(0) - point1(0);
		float y = point2(1) - point1(1);
		float z = point2(2) - point1(2);

		return vec4(x, y, z, 0.0f);
	}

	float dot(const Vector4& vec1, const Vector4& vec2)
	{
		return (vec1(0)*vec2(0) + vec1(1) * vec2(1) + vec1(2) * vec2(2));
	}

	Vector4 project(const Vector4& vec, const Vector4& onto)
	{
		Vector4 projection;

		float dotProduct = dot(vec, onto);

		projection[0] = dotProduct * onto.dx();
		projection[1] = dotProduct * onto.dy();
		projection[2] = dotProduct * onto.dz();
		projection[3] = 0.0f;

		return projection;
	}

}
