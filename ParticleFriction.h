#pragma once

#include "ParticleForceGenerator.h"

class ParticleFriction : public ParticleForceGenerator
{
public:
	ParticleFriction();
	~ParticleFriction();

	virtual void UpdateForce(Particle* particle, float duration) override;
};

