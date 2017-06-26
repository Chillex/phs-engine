#pragma once

#include <glm/glm.hpp>

class Particle;

class AnchorSpring
{
public:
	AnchorSpring(glm::vec2 anchorPoint, Particle* particle, float springConst, float restLength);
	~AnchorSpring();

	void UpdateForce(float deltaTime);

	glm::vec2 anchor;
	Particle* particle;

protected:
	float m_springConst;
	float m_restLength;
};

