#include "Particle.h"

namespace talga
{
	Particle::Particle(F32 lifetime, F32 x, F32 y, F32 vx, F32 vy)
		: mLifetime(lifetime)
		, mPosition(x, y, 0.0f, 1.0f)
		, mVelocity(vx, vy, 0.0f, 0.0f)
		, mRotation(0.0f)
		, mColor(1.0f, 0.0f, 0.0f, 1.0f)
	{
	}

	Particle::~Particle()
	{
	}
}