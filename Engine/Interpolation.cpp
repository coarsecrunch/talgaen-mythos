#include "Math/Interpolation.h"

namespace talga
{
	F32 ApproachLinearInterp(F32 current, F32 goal, F32 dt)
	{
		F32 difference = goal - current;

		if (difference > dt)
			return current + dt;
		if (difference < -dt)
			return current - dt;

		return goal;
	}
}
