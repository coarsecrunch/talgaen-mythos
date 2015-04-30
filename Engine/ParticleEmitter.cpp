#include "ParticleEmitter.h"
#include "Camera.h"
#include <cstdlib>
#include <ctime>
#include <iostream>


namespace talga
{
	ParticleEmitter::ParticleEmitter(Game* game, const I32 maxNumParticles, F32 particleLifetime, F32 spawnRadius, F32 spawnSpeed)
		: PhysSprite(game)
		, mMaxNumParticles(maxNumParticles)
		, mNumParticles(0)
		, mParticleTotalLife(particleLifetime)
		, mSpawnRadius(spawnRadius)
		, mSpawnSpeed(spawnSpeed)
		, mParticles(new Particle[maxNumParticles])
		, mAddAt(0.0f)
		, mLaunchSpeed(0.5f)
	{
		setVisible(true);
		setCheckCollisions(false);
	}
	
	ParticleEmitter::ParticleEmitter(const ParticleEmitter& cpy)
		: PhysSprite(cpy.mGame)
		, mMaxNumParticles(cpy.mMaxNumParticles)
		, mParticleTotalLife(cpy.mParticleTotalLife)
		, mSpawnRadius(cpy.mSpawnRadius)
		, mSpawnSpeed(cpy.mSpawnSpeed)
		, mLaunchSpeed(cpy.mLaunchSpeed)
		, mAddAt(cpy.mAddAt)
		, mNumParticles(cpy.mNumParticles)
		, mParticles(new Particle[cpy.mMaxNumParticles])
	{
	}


	int numParticlesToSpawnPerCall = 20;
	void ParticleEmitter::Update(F32 dt)
	{
		mAddAt += dt;

		if (mAddAt >= mSpawnSpeed)
		{
			for (int i = 0; i < numParticlesToSpawnPerCall; ++i)
				AddParticle();
			mAddAt = 0.0f;
		}

		for (I32 i = 0; i < mNumParticles; ++i)
		{
			Particle& particle = mParticles[i];
			
			particle.mVelocity[1] += GRAVITY * 0.1f;
			particle.mPosition = particle.mPosition + particle.mVelocity;
			
			particle.mLifetime -= dt;
			
			if (particle.mLifetime <= 0)
				RemoveParticle(i);
		}
	}
	
	//Render needs to be rewritten to not use RenderQuad, which is super slow
	void ParticleEmitter::Render(const Camera& camera)
	{
		glBindVertexArray(Camera::mVAOs[Camera::COLORED_QUAD]);
		glUseProgram(Camera::mColliderProgram);
		
		mat4 trans;
		mat4 scale;
		mat4 fix;
		static F32 sizey = 4.0f;
		
		for (int i = 0; i < mNumParticles; ++i)
		{
			Particle& particle = mParticles[i];
			trans[0][3] = (mPosition[0] + particle.mPosition[0]) + sizey * 0.5f;
			trans[1][3] = (mPosition[1] + particle.mPosition[1]) + sizey * 0.5f;

			scale[0][0] = sizey;
			scale[1][1] = sizey;

			fix = trans * scale;
			
			glUniform1fv(Camera::mColorUnifromLoc, 4, &particle.mColor[0]);
			glUniformMatrix4fv(Camera::mColliderModelLoc, 1, GL_TRUE, &fix[0][0]);
			glUniformMatrix4fv(Camera::mColliderViewLoc, 1, GL_TRUE, &camera.getCameraMat()[0][0]);
			glUniformMatrix4fv(Camera::mColliderProjectionLoc, 1, GL_TRUE, &camera.getProjectionMatrix()[0][0]);


			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)0);
		}
		
		glBindVertexArray(0);
	}

	void ParticleEmitter::AddParticle()
	{
		if ( !(mNumParticles < mMaxNumParticles) ) 
			return;

		mParticles[mNumParticles] = Particle();
		mParticles[mNumParticles].mLifetime = mParticleTotalLife;
		
		
		
		/*

		F32 r = (F32)distributionColor(generator);
		F32 g = (F32)distributionColor(generator);
		F32 b = (F32)distributionColor(generator);

		F32 xSet = cos(angle) * radius;
		F32 ySet = sin(angle) * radius;

		vec4 launchSpeed = normalize(vec4(xSet, ySet, 0, 0)) * mLaunchSpeed;

		mParticles[mNumParticles].mColor = vec4(r, g, b, 1.0f);
		mParticles[mNumParticles].mPosition = vec4(xSet, ySet);
		mParticles[mNumParticles].mVelocity = mVelocity + launchSpeed;
		*/
		++mNumParticles;
	}

	void ParticleEmitter::RemoveParticle(I32 index)
	{
		mParticles[index] = mParticles[mNumParticles - 1];
		--mNumParticles;
	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete[] mParticles;
	}
}