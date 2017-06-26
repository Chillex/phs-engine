#include "SpringJoint.h"
#include "Particle.h"

SpringJoint::SpringJoint(Particle* p1, Particle* p2, float springConst, float restLength)
	: particle1(p1)
	, particle2(p2)
	, m_springConst(springConst)
	, m_restLength(restLength)
{
}

void SpringJoint::UpdateForce(float deltaTime)
{
	glm::vec2 force = particle1->position - particle2->position;

	float magnitude = glm::length(force);
	magnitude = glm::abs(magnitude - m_restLength) * m_springConst;

	float dampening = 0.5f;
	force = glm::normalize(force) * magnitude - dampening * (particle1->velocity - particle2->velocity);

	particle1->AddForce(force * 0.5f);
	particle1->AddForce(-force * 0.5f);
}

SpringJoint::~SpringJoint()
{
}
