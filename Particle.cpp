#include "Particle.h"
#include <math.h>

Particle::Particle()
	: position(0.0f)
	, velocity(0.0f)
	, acceleration(0.0f)
	, radius(1.0f)
	, lifetime(INFINITE_LIFETIME)
	, inverseMass(1.0f)
	, bounciness(1.0f)
	, m_forceAccumulator(0.0f)
	, m_dampening(0.99f)
{
	InitBounds();
}

Particle::Particle(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, float dampening, float mass, float lifetime, float radius, float bounciness)
	: position(position)
	, velocity(velocity)
	, acceleration(acceleration)
	, radius(radius)
	, lifetime(lifetime)
	, inverseMass(1.0f / mass)
	, bounciness(bounciness)
	, m_forceAccumulator(0.0f)
	, m_dampening(dampening)
{
	InitBounds();
}

Particle::~Particle()
{
}

const float Particle::INFINITE_LIFETIME = -66.6f;

void Particle::AddForce(const glm::vec2& force)
{
	m_forceAccumulator += force;
}

void Particle::Integrate(float duration)
{
	if (inverseMass <= 0.0f)
		return;

	assert(duration > 0.0f);

	// calculate acceleration from force
	glm::vec2 resultingAcceleration = acceleration;
	resultingAcceleration += m_forceAccumulator * inverseMass;

	// update linear velocity
	velocity += resultingAcceleration * duration;

	// calculate drag
	//velocity *= powf(m_dampening, duration);

	// update linear position
	position += velocity *duration;
	circleBound.center = position;

	// clear forces
	ClearAccumulator();
}

bool Particle::UpdateAlive(float duration)
{
	if (lifetime != INFINITE_LIFETIME)
	{
		lifetime -= duration;
		if (lifetime <= 0.0f)
		{
			return false;
		}
	}

	return true;
}

void Particle::SetPosition(glm::vec2 pos)
{
	position = pos;
	circleBound.center = position;
}

float Particle::GetMass() const
{
	return 1.0f / inverseMass;
}

void Particle::InitBounds()
{
	circleBound.center = position;
	circleBound.radius = radius;
}

void Particle::ClearAccumulator()
{
	m_forceAccumulator = glm::vec2(0.0f);
}
