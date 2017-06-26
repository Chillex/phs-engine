#pragma once

#include <glm/glm.hpp>

class Particle;

class SpringJoint
{
public:
	SpringJoint(Particle* p1, Particle* p2, float springConst, float restLength);
	~SpringJoint();

	void UpdateForce(float deltaTime);

	Particle* particle1;
	Particle* particle2;

protected:
	float m_springConst;
	float m_restLength;
};

