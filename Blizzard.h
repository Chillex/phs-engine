#pragma once

#include <glm/glm.hpp>
#include <vector>

class ParticleForceRegistry;
class Particle;

namespace sf {
	class RenderWindow;
}

class Blizzard
{
public:

	struct Direction
	{
		enum Enum
		{
			CLOCKWISE,
			COUNTERCLOCKWISE
		};
	};

	Blizzard(ParticleForceRegistry* forceRegistry);
	Blizzard(glm::vec2 center, float rate, ParticleForceRegistry* forceRegistry);
	Blizzard(glm::vec2 center, float rate, Direction::Enum direction, ParticleForceRegistry* forceRegistry);
	~Blizzard();

	void Update(float duration, bool spawnNew);
	void Render(sf::RenderWindow& window);

	std::vector<Particle*> GetParticles(void) const;

protected:
	typedef std::vector<Particle*> Particles;
	Particles m_particles;

	std::vector<glm::vec2> m_startingVelocities;
	size_t m_currentVelocityIndex;

	glm::vec2 m_center;
	
	Direction::Enum m_direction;

	float m_rate;
	float m_timer;

	ParticleForceRegistry* m_forceRegistry;

	void SetupStartingVelocities(void);
	void CreateParticle(void);
};

