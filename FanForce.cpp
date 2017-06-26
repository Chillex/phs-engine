#include "FanForce.h"

#include "Particle.h"

FanForce::FanForce(const glm::vec2& force, const glm::vec2& bounds)
	: m_force(force)
	, m_bounds(bounds)
{
}

void FanForce::UpdateForce(Particle* particle, float duration)
{
	particle->AddForce(m_force * particle->GetMass());
}

const glm::vec2& FanForce::GetBounds() const
{
	return m_bounds;
}

FanForce::~FanForce()
{
}
