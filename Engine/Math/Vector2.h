#pragma once
#include "Vector3.h"

namespace talga
{
	template<typename T>
	class Vector2 
	{
	public:
		Vector2(T x = 0.0f, T y = 0.0f) : vec{x, y} {}

		template < template<typename Vector3> class T2>
		Vector2(const T2<T>& ref)
		{
			vec[0] = ref.x();
			vec[1] = ref.y();
		}


		~Vector2() {}

		inline T& operator[](int index) { return vec[index]; }
		inline T operator[](int index) const { return vec[index]; }
		inline T operator()(int index) { return vec[index]; }
		inline Vector2 operator+(const Vector2& vec) const { return Vector2(x() + vec.x(), y() + vec.y()); }
		inline Vector2 operator-(const Vector2& vec) const { return Vector2(x() - vec.x(), y() - vec.y()); }

		inline Vector2 operator*(float scaler) { return Vector2(x() * scaler, y() * scaler); }
		inline Vector2 operator/(float scaler) { return vec2(x() / scaler, y() / scaler); }

		inline T x() const { return vec[0]; }
		inline T y() const { return vec[1]; }
	private:
		T vec[2];
	};

	typedef Vector2<float> vec2;
	typedef Vector2<int> vec2i;
}