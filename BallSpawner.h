#pragma once

#include <glm/glm.hpp>
#include <vector>

class ParticleForceRegistry;
class Particle;

namespace sf {
	class RenderWindow;
}

class BallSpawner
{
public:
	BallSpawner(ParticleForceRegistry* forceRegistry);
	BallSpawner(glm::vec2 center, float width, ParticleForceRegistry* forceRegistry);
	~BallSpawner();

	void Update(float duration);
	void Render(sf::RenderWindow& window);

	void Reset(void);

	std::vector<Particle*> GetParticles(void) const;

	void CreateParticles(int amount);

protected:
	typedef std::vector<Particle*> Particles;
	Particles m_particles;

	glm::vec2 m_center;
	float m_width;

	ParticleForceRegistry* m_forceRegistry;

	void CreateParticle(void);
};

