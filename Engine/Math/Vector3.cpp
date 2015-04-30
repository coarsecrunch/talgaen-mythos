#include "Vector3.h"
#include <cmath>

namespace talga
{
	Vector3::Vector3(float f, float f1, float f2)
	{
		vec[0] = f;
		vec[1] = f1;
		vec[2] = f2;
	}

	Vector3::~Vector3()
	{
	}

	Vector3& Vector3::operator =(const Vector3& copy)
	{
		this->vec[0] = copy(0);
		this->vec[1] = copy(1);
		this->vec[2] = copy(2);

		return *this;
	}

	float& Vector3::operator[](int index)
	{
		return vec[index];
	}

	const float& Vector3::operator[](int index) const
	{
		return vec[index];
	}

	float Vector3::operator ()(int index) const
	{
		return vec[index];
	}

	Vector3 operator *(const Vector3& vec, float scalar)
	{
		Vector3 scaledVec = vec;
		scaledVec[0] *= scalar;
		scaledVec[1] *= scalar;
		scaledVec[2] *= scalar;
		return scaledVec;
	}

	Vector3 operator +(const Vector3& vec1, const Vector3& vec2)
	{
		return Vector3{ vec1(0) + vec2(0), vec1(1) + vec2(1), vec1(2) + vec2(2) };
	}

	float Vector3::mag() const
	{
		return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	}

	float Vector3::dx() const
	{
		return (*this)(0) / mag();
	}

	float Vector3::dy() const
	{
		return (*this)(1) / mag();
	}



	Vector3 normalize(const Vector3& vec)
	{
		Vector3 newVec = vec;
		float mag = vec.mag();
		if (mag == 0.0f)
			mag = 0.00000001f;

		newVec[0] = vec(0) / mag;
		newVec[1] = vec(1) / mag;
		newVec[2] = 0;

		return newVec;
	}

	Vector3 cross(const Vector3& vec1, const Vector3& vec2)
	{
		Vector3 newVec;
		newVec[0] = vec1(1) * vec2(2) - vec1(0) * vec2(1);
		newVec[1] = vec1(2) * vec2(0) - vec1(0) * vec2(2);
		newVec[2] = 0;
		return newVec;
	}

	Vector3 operator-(const Vector3& vec1, const Vector3& vec2)
	{
		Vector3 newVec;
		newVec[0] = vec1(0) - vec2(0);
		newVec[1] = vec1(1) - vec2(1);
		newVec[2] = 0;
		return newVec;
	}

	Vector3 vecBetween(Vector3 point1, Vector3 point2)
	{
		float x = point2(0) - point1(0);
		float y = point2(1) - point1(1);

		return vec3(x, y, 0.0f);
	}

	float dot(const Vector3& vec1, const Vector3& vec2)
	{
		return (vec1(0)*vec2(0) + vec1(1) * vec2(1));
	}

	Vector3 project(const Vector3& vec, const Vector3& onto)
	{
		Vector3 projection;

		float dotProduct = dot(vec, onto);

		projection[0] = dotProduct * onto.dx();
		projection[1] = dotProduct * onto.dy();
		projection[2] = 0.0f;

		return projection;
	}
}