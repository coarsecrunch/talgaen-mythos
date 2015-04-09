#pragma once

namespace talga
{
	class Vector2 {
	public:
		Vector2(float x = 0, float y = 0);
		virtual ~Vector2();

		float& operator[](int index);
		float operator()(int index);
	private:
		float arr[2];
	};

	typedef Vector2 vec2;
}