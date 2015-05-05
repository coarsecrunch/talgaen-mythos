#pragma once

#include "Particle.h"
#include "Cmn.h"

/* ON HOLD
namespace talga
{
	class Game;
	class Camera;

	class ParticleEmitter : public PhysSprite
	{
	public:
		ParticleEmitter(Game* game, const I32 maxNumParticles, F32 particleLifetime, F32 spawnRadius, F32 spawnSpeed);
		~ParticleEmitter();
		const ParticleEmitter& operator=(const ParticleEmitter& cpy) {}
		ParticleEmitter(const ParticleEmitter& cpy);



		virtual void Update(talga::F32 dt) override;
		virtual void Render(const Camera&) override;
		void AddParticle();

		I32 getNumParticles() const {return mNumParticles; }
	private:


		Particle* mParticles;

		const I32 mMaxNumParticles;
		I32 mNumParticles;
		F32 mParticleTotalLife;
		F32 mSpawnRadius;
		F32 mSpawnSpeed; // in ms
		F32 mLaunchSpeed;

		F32 mAddAt;

		void RemoveParticle(I32 index);
		void UpdateGLBuffer();
	};

}*/