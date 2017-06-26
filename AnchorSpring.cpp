#include "AnchorSpring.h"

#include "Particle.h"

AnchorSpring::AnchorSpring(glm::vec2 anchorPoint, Particle* particle, float springConst, float restLength)
	: anchor(anchorPoint)
	, particle(particle)
	, m_springConst(springConst)
	, m_restLength(restLength)
{
}

void AnchorSpring::UpdateForce(float deltaTime)
{
	glm::vec2 force = particle->position;
	force -= anchor;

	float magnitude = glm::length(force);
	magnitude = (m_restLength - magnitude) * m_springConst;

	float dampening = 0.5f;
	force = glm::normalize(force) * magnitude - dampening * particle->velocity;

	particle->AddForce(force);
}

AnchorSpring::~AnchorSpring()
{
}
