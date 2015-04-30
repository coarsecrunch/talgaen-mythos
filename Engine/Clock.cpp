#include "Clock.h"
#include <chrono>

namespace talga
{
	Clock::Clock()
	{
		Init();
	}

	void Clock::Init()
	{
		mZeroPoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	unsigned int Clock::TimePassed()
	{
		auto past = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		return past - mZeroPoint;
	}

	Clock::~Clock()
	{
	}
}