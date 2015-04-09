#include "Math\Operations.h"

namespace talga
{
	F32 abs(F32 num)
	{
		if (num < 0)
			return -num;

		return num;
	}
}