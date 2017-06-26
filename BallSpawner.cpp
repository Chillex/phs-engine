#include "BallSpawner.h"

#include <SFML/Graphics.hpp>

#include "Simulation.h"
#include "Particle.h"
#include "ParticleForceRegistry.h"
#include "ParticleGravity.h"
#include "FanForce.h"

BallSpawner::BallSpawner(ParticleForceRegistry* forceRegistry)
	: BallSpawner(glm::vec2(0.0f), 1.0f, forceRegistry)
{
}

BallSpawner::BallSpawner(glm::vec2 center, float width, ParticleForceRegistry* forceRegistry)
	: m_center(center)
	, m_width(width)
	, m_forceRegistry(forceRegistry)
{
}

BallSpawner::~BallSpawner()
{
	for (size_t i = 0; i < m_particles.size(); ++i)
	{
		delete m_particles[i];
	}
}

void BallSpawner::Update(float duration)
{
	// update all particles
	for (Particles::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		(*it)->Integrate(duration);
	}
}

void BallSpawner::Render(sf::RenderWindow& window)
{
	sf::CircleShape particle;
	particle.setFillColor(sf::Color::Green);
	particle.setOutlineThickness(0.0f);

	sf::CircleShape circleShape;
	circleShape.setFillColor(sf::Color::Transparent);
	circleShape.setOutlineColor(sf::Color::Red);
	circleShape.setOutlineThickness(-4.0f);

	// render all particles
	for (Particles::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		glm::vec2 pos = (*it)->position;
		particle.setPosition(pos.x, pos.y);
		particle.setRadius((*it)->radius);
		particle.setOrigin((*it)->radius, (*it)->radius);

		window.draw(particle);

		bool debugDraw = false;
		if(debugDraw)
		{
			// CircleBound
			circleShape.setPosition((*it)->circleBound.center.x, (*it)->circleBound.center.y);
			circleShape.setRadius((*it)->circleBound.radius);
			circleShape.setOrigin((*it)->circleBound.radius, (*it)->circleBound.radius);
			window.draw(circleShape);
		}
	}
}

void BallSpawner::Reset()
{
	for (size_t i = 0; i < m_particles.size(); ++i)
	{
		m_forceRegistry->Remove(m_particles[i], Simulation::ParticleGravityGenerator);
		m_forceRegistry->Remove(m_particles[i], Simulation::FanForceGenerator);
		delete m_particles[i];
	}
	m_particles.clear();
}

std::vector<Particle*> BallSpawner::GetParticles() const
{
	return m_particles;
}

void BallSpawner::CreateParticles(int amount)
{
	for (int i = 0; i < amount; ++i)
	{
		CreateParticle();
	}
}

void BallSpawner::CreateParticle()
{
	int min = -m_width;
	int max = m_width;
	int randWidth = rand() % (max - min + 1) + min;

	min = 0;
	max = 200.0f;
	int randHeight = rand() % (max - min + 1) + min;

	glm::vec2 velocity(0.0f);
	glm::vec2 acceleration(0.0f);
	float dampening = 0.9f;
	float mass = 1.0f;
	float lifetime = Particle::INFINITE_LIFETIME;
	float size = 20.0f;
	float bounciness = 0.75f;

	Particle* particle = new Particle(glm::vec2(m_center.x + randWidth, m_center.y - randHeight), velocity, acceleration, dampening, mass, lifetime, size, bounciness);
	m_particles.push_back(particle);

	m_forceRegistry->Add(particle, Simulation::ParticleGravityGenerator);
}

