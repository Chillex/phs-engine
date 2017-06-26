#pragma once

#include <glm/glm.hpp>
#include "BoundingVolumes.h"

class Particle
{
public:
	Particle();
	Particle(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, float dampening, float mass, float lifetime, float radius, float bounciness);
	~Particle();

	void AddForce(const glm::vec2& force);
	void Integrate(float duration);
	bool UpdateAlive(float duration);

	void SetPosition(glm::vec2 pos);
	float GetMass(void) const;

	static const float INFINITE_LIFETIME;

	CircleBound circleBound;

	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;

	float radius;
	float lifetime;

	float inverseMass;

	float bounciness;

protected:

	glm::vec2 m_forceAccumulator;

	float m_dampening;

	void InitBounds();
	void ClearAccumulator();
};

