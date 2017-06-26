#pragma once

#include "ParticleForceGenerator.h"

#include <glm/glm.hpp>

class FanForce : public ParticleForceGenerator
{
public:
	FanForce(const glm::vec2& force, const glm::vec2& bounds);
	~FanForce();

	virtual void UpdateForce(Particle* particle, float duration) override;

	const glm::vec2& GetBounds(void) const;

protected:
	glm::vec2 m_force;
	glm::vec2 m_bounds;
};

