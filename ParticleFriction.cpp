#include "ParticleFriction.h"

#include "Particle.h";

ParticleFriction::ParticleFriction()
{
}

ParticleFriction::~ParticleFriction()
{
}

void ParticleFriction::UpdateForce(Particle* particle, float duration)
{
	float friction = 1.0f;
	particle->AddForce(-particle->velocity * friction);
}
