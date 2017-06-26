#include "SpringJoint.h"
#include "Particle.h"
#include "Math.h"

SpringJoint::SpringJoint(Particle* p1, Particle* p2, float springConst, float restLength, float damping)
	: particle1(p1)
	, particle2(p2)
	, m_springConst(springConst)
	, m_restLength(restLength)
	, m_damping(damping)
{
}

void SpringJoint::UpdateForce(float deltaTime)
{
	glm::vec2 force = particle1->position - particle2->position;

	float magnitude = Math::SaveLength(force);
	magnitude = magnitude - m_restLength;
	magnitude *= m_springConst;

	force = Math::SaveNormalize(force);
	force *= -magnitude;

	glm::vec2 relVel = particle1->velocity - particle2->velocity;
	force -= relVel * m_damping;

	particle1->AddForce(force * 0.5f);
	particle2->AddForce(-force * 0.5f);
}

SpringJoint::~SpringJoint()
{
}
