#pragma once

#include <list>
#include "ParticleEmitter.h"
#include "Cmn.h"

namespace talga
{
	class ParticleEngine
	{
	public:
		ParticleEngine();
		~ParticleEngine();

		void Update(F32 dt);
		void Render();
	private:
		std::list<ParticleEmitter> mEmitters;
	};
}

