#pragma once

#include "stdafx.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace Bengine {
	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		// After adding the particleBatch the ParticleEngine becomes
		// responsible for deallocation
		void addParticlebatch(ParticleBatch2D* particleBatch);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

	private:
		std::vector<ParticleBatch2D*> m_batches;
	};
}


