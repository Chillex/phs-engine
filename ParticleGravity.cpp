#include "ParticleGravity.h"

#include "Particle.h"

ParticleGravity::ParticleGravity(const glm::vec2& gravity)
	: m_gravity(gravity)
{
}

void ParticleGravity::UpdateForce(Particle* particle, float duration)
{
	particle->AddForce(m_gravity * particle->GetMass());
}

ParticleGravity::~ParticleGravity()
{
}
