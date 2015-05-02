#include "Math\Operations.h"

namespace talga
{
	F32 abs(F32 num)
	{
		if (num < 0)
			return -num;

		return num;
	}

	vec2 GET_UV(F32 x, F32 xMax, F32 y, F32 yMax)
	{
		return vec2{x / xMax, (y / yMax) };
	}
}