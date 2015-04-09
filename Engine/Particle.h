#pragma once

#include "Cmn.h"
#include "Math/Vector4.h"

/*	Particle would be much cleaner if it inherited from PhysSprite
	but that would add a lot of overhead, instead Particle is just
	made with the bare basics
*/

namespace talga
{
	class Particle
	{
	public:
		~Particle();

	protected:
		friend class ParticleEmitter;

		vec4 mPosition;
		vec4 mColor;
		vec4 mVelocity;

		talga::F32 mLifetime; // in ms
		talga::F32 mRotation;

		Particle(F32 lifetime = 1.0f, F32 x = 0.0f, F32 y = 0.0f, F32 vx = 0.0f, F32 vy = 0.0f); // only to be constructed by Particle Emitter
	};
}
