#include "Math\Vector2.h"
#include <stdexcept>

namespace talga
{
	Vector2::Vector2(float x, float y)
	{
		arr[0] = x;
		arr[1] = y;
	}

	Vector2::~Vector2() {
		// TODO Auto-generated destructor stub
	}

	float& Vector2::operator [](int index)
	{
		if (index != 0 && index != 1)
		{
			throw std::invalid_argument("Vector2 out of bounds");
		}
		return arr[index];
	}

	float Vector2::operator ()(int index)
	{
		if (index != 0 && index != 1)
		{
			throw std::invalid_argument("Vector2 out of bounds");
		}

		return arr[index];
	}
}