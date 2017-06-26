#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "BallSpawner.h"

class Particle;
class ParticleForceRegistry;
class ParticleGravity;
class FanForce;
class ParticleFriction;
class Blizzard;
class StaticObject;
class AnchorSpring;
class SpringJoint;

namespace sf {
	class RenderWindow;
}

class Simulation
{
public:
	Simulation();
	~Simulation();

	static ParticleGravity* ParticleGravityGenerator;
	static FanForce* FanForceGenerator;
	static ParticleFriction* ParticleFrictionGenerator;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void ToggleFan(void);
	void ToggleDebugDraw(void);
	void ToggleBlizzards(void);

	void SpawnBalls(int amount);
	void ResetBalls(void);

protected:
	struct CollisionType
	{
		enum Enum
		{
			Reflect,
			DeleteFirst,
			DeleteSecond
		};
	};

	void InitScene(void);

	void SpawnCloth(glm::vec2 position, glm::vec2 distance, glm::ivec2 amount);

	void ResolveCollisions(void);
	void ResolveSingleCollision(Particle* particle, StaticObject staticObject);
	void ResolveSingleCollision(Particle* particle1, Particle* particle2, CollisionType::Enum collisionType);

	ParticleForceRegistry* m_forceRegistry;

	std::vector<Blizzard> m_blizzards;
	std::vector<StaticObject> m_staticObjects;
	BallSpawner m_ballSpawner;

	// cloth
	std::vector<AnchorSpring> m_anchorSprings;
	std::vector<SpringJoint> m_springJoints;
	std::vector<Particle*> m_particles;

	bool m_fanActive;
	bool m_blizzardsActive;
	bool m_debugDraw;
};

