#include "Operations.h"

namespace talga
{

	vec2 GET_UV(F32 x, F32 xMax, F32 y, F32 yMax)
	{
		return vec2{x / xMax, (y / yMax) };
	}
}
