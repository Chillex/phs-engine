#pragma once

class Particle;

class ParticleForceGenerator
{
public:
	ParticleForceGenerator();
	virtual ~ParticleForceGenerator();

	virtual void UpdateForce(Particle* particle, float duration) = 0;
};

