#pragma once

namespace talga
{
	class Vector3;

	class Vector2 {
	public:
		Vector2(float x = 0, float y = 0);
		virtual ~Vector2();

		float& operator[](int index);
		float operator[](int index) const { return arr[index]; }
		float operator()(int index);

		const Vector2& operator=(const Vector3& vec3Cpy);
	private:
		float arr[2];
	};

	typedef Vector2 vec2;
}