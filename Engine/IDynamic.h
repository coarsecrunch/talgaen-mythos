#pragma once

#include "Cmn.h"

namespace talga
{
	class IDynamic
	{
	public:
		virtual void update(F32 dt) = 0;
	};
}