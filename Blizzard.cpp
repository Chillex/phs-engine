#include "Blizzard.h"

#include <glm/gtx/rotate_vector.hpp>
#include <SFML/Graphics.hpp>

#include "Simulation.h"
#include "Particle.h"
#include "ParticleForceRegistry.h"
#include "ParticleGravity.h"
#include "FanForce.h"

Blizzard::Blizzard(ParticleForceRegistry* forceRegistry)
	: Blizzard(glm::vec2(0.0f), 1.0f, forceRegistry)
{
}

Blizzard::Blizzard(glm::vec2 center, float rate, ParticleForceRegistry* forceRegistry)
	: Blizzard(center, rate, Direction::CLOCKWISE, forceRegistry)
{
}

Blizzard::Blizzard(glm::vec2 center, float rate, Direction::Enum direction, ParticleForceRegistry* forceRegistry)
	: m_currentVelocityIndex(0)
	, m_center(center)
	, m_direction(direction)
	, m_rate(rate)
	, m_timer(0.0f)
	, m_forceRegistry(forceRegistry)
{
	SetupStartingVelocities();
}


Blizzard::~Blizzard()
{
	for (size_t i = 0; i < m_particles.size(); ++i)
	{
		delete m_particles[i];
	}
}

void Blizzard::Update(float duration, bool spawnNew)
{
	if(spawnNew)
	{
		m_timer += duration;
		if (m_timer >= m_rate)
		{
			CreateParticle();
			m_timer = 0.0f;
		}
	}

	// update all particles
	for (Particles::iterator it = m_particles.begin(); it != m_particles.end();)
	{
		Particle* particle = *it;
		// check if particle is still alive
		if (particle->UpdateAlive(duration))
		{
			particle->Integrate(duration);
			++it;
		}
		else
		{
			m_forceRegistry->Remove(particle, Simulation::ParticleGravityGenerator);
			m_forceRegistry->Remove(particle, Simulation::FanForceGenerator);
			it = m_particles.erase(it);
			delete particle;
		}
	}
}

void Blizzard::Render(sf::RenderWindow& window)
{
	sf::CircleShape particle;
	particle.setFillColor(sf::Color::White);
	particle.setOutlineThickness(0.0f);

	// render all particles
	for (Particles::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		glm::vec2 pos = (*it)->position;
		particle.setPosition(pos.x, pos.y);
		particle.setRadius((*it)->radius);
		particle.setOrigin((*it)->radius, (*it)->radius);

		window.draw(particle);
	}
}

std::vector<Particle*> Blizzard::GetParticles(void) const
{
	return m_particles;
}

void Blizzard::SetupStartingVelocities(void)
{
	float steps = 25.0f;
	glm::vec2 v(150.0f, 0.0f);

	for (int i = 0; i < steps; ++i)
	{
		v = glm::rotate(v, 360.0f / steps);
		m_startingVelocities.push_back(v);
	}
}

void Blizzard::CreateParticle(void)
{
	glm::vec2 acceleration(0.0f);
	float dampening = 0.9f;
	float mass = 1.0f;
	float lifetime = 10.0f;
	float size = 1.0f;
	float bounciness = 0.2f;

	Particle* particle = new Particle(m_center, m_startingVelocities[m_currentVelocityIndex], acceleration, dampening, mass, lifetime, size, bounciness);
	m_particles.push_back(particle);

	m_forceRegistry->Add(particle, Simulation::ParticleGravityGenerator);

	// change velocity index
	if(m_direction == Direction::CLOCKWISE)
	{
		if (m_currentVelocityIndex < m_startingVelocities.size() - 1)
			++m_currentVelocityIndex;
		else
			m_currentVelocityIndex = 0;
	}
	else
	{
		if (m_currentVelocityIndex > 0)
			--m_currentVelocityIndex;
		else
			m_currentVelocityIndex = m_startingVelocities.size() - 1;

	}
	
}
