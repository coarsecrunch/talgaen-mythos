#include "Vector2.h"
#include "Vector3.h"

namespace talga
{
	Vector2::Vector2(float x, float y)
	{
		arr[0] = x;
		arr[1] = y;
	}

	Vector2::Vector2(const Vector3& cpy)
	{
		arr[0] = cpy[0];
		arr[1] = cpy[1];
	}

	Vector2::~Vector2() {
		// TODO Auto-generated destructor stub
	}

	const Vector2& Vector2::operator = (const Vector3& vec3Cpy)
	{
		arr[0] = vec3Cpy.x();
		arr[1] = vec3Cpy.y();

		return *this;
	}

	float& Vector2::operator [](int index)
	{
		return arr[index];
	}

	float Vector2::operator ()(int index)
	{
		return arr[index];
	}
}
