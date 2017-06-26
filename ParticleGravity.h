#pragma once

#include "ParticleForceGenerator.h"

#include <glm/glm.hpp>

class ParticleGravity : public ParticleForceGenerator
{
public:
	ParticleGravity(const glm::vec2& gravity);
	~ParticleGravity();

	virtual void UpdateForce(Particle* particle, float duration) override;

protected:
	glm::vec2 m_gravity;
};

