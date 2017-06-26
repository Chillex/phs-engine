#include "AnchorSpring.h"

#include "Particle.h"
#include "Math.h"

AnchorSpring::AnchorSpring(glm::vec2 anchorPoint, Particle* particle, float springConst, float restLength, float damping)
	: anchor(anchorPoint)
	, particle(particle)
	, m_springConst(springConst)
	, m_restLength(restLength)
	, m_damping(damping)
{
}

void AnchorSpring::UpdateForce(float deltaTime)
{
	glm::vec2 force = particle->position - anchor;

	float magnitude = Math::SaveLength(force);

	magnitude = magnitude - m_restLength;
	magnitude *= m_springConst;

	force = Math::SaveNormalize(force);
	force *= -magnitude;

	float damping = 0.9f;
	glm::vec2 relVel = particle->velocity;
	force -= relVel * m_damping;

	particle->AddForce(force);
}

AnchorSpring::~AnchorSpring()
{
}
