#pragma once

#include "Cmn.h"

namespace talga
{
	class IState
	{
	public:
		virtual void begin() = 0;
		virtual void exit() = 0;
		virtual void update(I32 dt) = 0;
	};
}