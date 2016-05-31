#pragma once
#include "Vector2.h"
namespace talga
{
	template<typename T>
	class Vector3 
	{
	public:
		Vector3(T f = 0.0f, T f1 = 0.0f, T f2 = 0.0f) : vec{f,f1,f2} {}

		template < template<typename Vector2> class T2>
		Vector3(const T2<T>& ref)
		{
			vec[0] = ref.x();
			vec[1] = ref.y();
			vec[2] = 1.0f;
		}

		~Vector3() {}

		T& operator[](int index) { return vec[index]; }
		T operator[](int index) const { return vec[index]; }
		T operator()(int index) const { return vec[index]; }
		

		Vector3 operator*(float scaler) const { return Vector3{ x() * scaler, y() * scaler, z() * scaler }; }
		Vector3 operator/(float scaler) const { return Vector3{ x() / scaler, y() / scaler, z() / scaler }; }
		Vector3 operator+(Vector3 vector) const { return Vector3{ x() + vector.x(), y() + vector.y(), z() + vector.z() }; }
		Vector3 operator-(Vector3 vector) const { return Vector3{ x() - vector.x(), y() - vector.y(), z() - vector.z() }; }


		inline T x() const { return vec[0]; }
		inline T y() const { return vec[1]; }
		inline T z() const { return vec[2]; }

		inline T mag() const { return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]); }
		inline T dx() const { return x() / mag(); }
		inline T dy() const { return y() / mag(); }
		inline T dz() const { return z() / mag(); }
	private:
		T vec[3];
	};

	typedef Vector3<int> vec3i;

	typedef Vector3<float> vec3;
	
	vec3 normalize(const vec3& vec);
	vec3 cross(const vec3& vec1, const vec3& vec2);
	vec3 project(const vec3& vec, const vec3& onto);
	float dot(const vec3& vec1, const vec3& vec2);
	vec3 vecBetween(vec3 point1, vec3 point2);

}