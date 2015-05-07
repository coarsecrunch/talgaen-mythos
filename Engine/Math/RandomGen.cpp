#include "RandomGen.h"


namespace talga
{

	static std::minstd_rand0  generator;
	static std::uniform_real_distribution<F32> distribution01f;

	void seedRand(F64 sd)
	{
		generator = std::minstd_rand0(sd);
	}

	F32 get01f()
	{
		return distribution01f(generator);
	}
}
