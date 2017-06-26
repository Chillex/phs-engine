#include "Simulation.h"

#include <SFML/Graphics.hpp>
#include <glm/gtx/projection.hpp>

#include "Blizzard.h"
#include "Constants.h"
#include "StaticObject.h"
#include "CollisionResolution.h"
#include "ParticleFriction.h"
#include "ParticleGravity.h"
#include "FanForce.h"
#include "ParticleFriction.h"
#include "ParticleForceRegistry.h"
#include "AnchorSpring.h"
#include "SpringJoint.h"

ParticleGravity* Simulation::ParticleGravityGenerator = new ParticleGravity(CONSTANT::GRAVITY);
FanForce* Simulation::FanForceGenerator = new FanForce(glm::vec2(-200.0f, 0.0f), glm::vec2(150.0f, 850.0f));
ParticleFriction* Simulation::ParticleFrictionGenerator = new ParticleFriction();

Simulation::Simulation()
	: m_forceRegistry(new ParticleForceRegistry())
	, m_ballSpawner(glm::vec2(1920.0f * 0.5f, 0.0f), 450.0f, m_forceRegistry)
	, m_fanActive(false)
{
	InitScene();
}

Simulation::~Simulation()
{
	delete m_forceRegistry;
}

void Simulation::Update(float deltaTime)
{
	m_forceRegistry->UpdateForces(deltaTime);

	// update blizzards
	for (std::vector<Blizzard>::iterator it = m_blizzards.begin(); it != m_blizzards.end(); ++it)
	{
		// manually add fan force
		if (m_fanActive)
		{
			// for each particle in the blizzard
			std::vector<Particle*> blizzardParticles = (*it).GetParticles();
			for (std::vector<Particle*>::iterator particleIt = blizzardParticles.begin(); particleIt != blizzardParticles.end(); ++particleIt)
			{
				Particle* particle = (*particleIt);

				// check if it is in the fan area
				if (particle->position.y >= FanForceGenerator->GetBounds().x && particle->position.y <= FanForceGenerator->GetBounds().y)
				{
					FanForceGenerator->UpdateForce(particle, deltaTime);
				}
			}
		}

		(*it).Update(deltaTime);
	}

	// update balls
	if (m_fanActive)
	{
		// for each particle in the ballspawner
		std::vector<Particle*> ballParticles = m_ballSpawner.GetParticles();
		for (std::vector<Particle*>::iterator particleIt = ballParticles.begin(); particleIt != ballParticles.end(); ++particleIt)
		{
			Particle* particle = (*particleIt);

			// check if it is in the fan area
			if (particle->position.y >= FanForceGenerator->GetBounds().x && particle->position.y <= FanForceGenerator->GetBounds().y)
			{
				FanForceGenerator->UpdateForce(particle, deltaTime);
			}
		}
	}
	m_ballSpawner.Update(deltaTime);

	// update springs
	for (std::vector<AnchorSpring>::iterator it = m_anchorSprings.begin(); it != m_anchorSprings.end(); ++it)
	{
		(*it).UpdateForce(deltaTime);
	}
	for (std::vector<SpringJoint>::iterator it = m_springJoints.begin(); it != m_springJoints.end(); ++it)
	{
		(*it).UpdateForce(deltaTime);
	}

	// update particles
	for (std::vector<Particle*>::iterator particleIt = m_particles.begin(); particleIt != m_particles.end(); ++particleIt)
	{
		Particle* particle = (*particleIt);

		if (m_fanActive)
		{
			// check if it is in the fan area
			if (particle->position.y >= FanForceGenerator->GetBounds().x && particle->position.y <= FanForceGenerator->GetBounds().y)
			{
				FanForceGenerator->UpdateForce(particle, deltaTime);
			}
		}

		particle->Integrate(deltaTime);
	}

	for (int i = 0; i < 1; ++i)
	{
		ResolveCollisions();
	}
}

void Simulation::Render(sf::RenderWindow& window)
{
	// render static objects#
	sf::RectangleShape objectShape;
	objectShape.setFillColor(sf::Color::Blue);

	// debug draw objects
	sf::CircleShape circleShape;
	circleShape.setFillColor(sf::Color::Transparent);
	circleShape.setOutlineColor(sf::Color::Red);
	circleShape.setOutlineThickness(-4.0f);

	sf::RectangleShape rectShape;
	rectShape.setFillColor(sf::Color::Transparent);
	rectShape.setOutlineColor(sf::Color::Red);
	rectShape.setOutlineThickness(-4.0f);

	sf::ConvexShape oobb;
	oobb.setPointCount(4);
	oobb.setFillColor(sf::Color::Transparent);
	oobb.setOutlineColor(sf::Color::Red);
	oobb.setOutlineThickness(-4.0f);

	for (std::vector<StaticObject>::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); ++it)
	{
		StaticObject obj = (*it);

		objectShape.setPosition(obj.position.x, obj.position.y);
		objectShape.setSize(sf::Vector2f(obj.size.x, obj.size.y));
		objectShape.setOrigin(obj.size.x * 0.5f, obj.size.y * 0.5f);
		objectShape.setRotation(obj.rotation);

		window.draw(objectShape);

		if (m_debugDraw)
		{
			// CircleBound
			circleShape.setPosition(obj.circleBound.center.x, obj.circleBound.center.y);
			circleShape.setRadius(obj.circleBound.radius);
			circleShape.setOrigin(obj.circleBound.radius, obj.circleBound.radius);
			window.draw(circleShape);

			// AABB
			rectShape.setPosition(obj.aabb.center.x, obj.aabb.center.y);
			rectShape.setSize(sf::Vector2f(obj.aabb.halfSize.x * 2.0f, obj.aabb.halfSize.y * 2.0f));
			rectShape.setOrigin(obj.aabb.halfSize.x, obj.aabb.halfSize.y);
			window.draw(rectShape);

			// OOBB
			oobb.setPoint(0, { obj.oobb.corners[0].x, obj.oobb.corners[0].y });
			oobb.setPoint(1, { obj.oobb.corners[1].x, obj.oobb.corners[1].y });
			oobb.setPoint(2, { obj.oobb.corners[2].x, obj.oobb.corners[2].y });
			oobb.setPoint(3, { obj.oobb.corners[3].x, obj.oobb.corners[3].y });
			window.draw(oobb);
		}
	}

	// render fan
	sf::RectangleShape fanShape(sf::Vector2f(20.0f, (FanForceGenerator->GetBounds().y - FanForceGenerator->GetBounds().x)));
	fanShape.setPosition(window.getView().getSize().x - fanShape.getSize().x, FanForceGenerator->GetBounds().x);
	fanShape.setFillColor((m_fanActive) ? sf::Color::Green : sf::Color::Red);
	window.draw(fanShape);

	// render blizzards
	for (std::vector<Blizzard>::iterator it = m_blizzards.begin(); it != m_blizzards.end(); ++it)
	{
		(*it).Render(window);
	}

	// render balls
	m_ballSpawner.Render(window);

	// render cloth
	sf::CircleShape clothShape;
	clothShape.setFillColor(sf::Color::Cyan);
	for (std::vector<Particle*>::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		Particle* p = (*it);

		clothShape.setPosition(p->position.x, p->position.y);
		clothShape.setRadius(p->circleBound.radius);
		clothShape.setOrigin(p->circleBound.radius, p->circleBound.radius);

		window.draw(clothShape);
	}

	sf::VertexArray line(sf::LineStrip, 2);
	for (std::vector<AnchorSpring>::iterator it = m_anchorSprings.begin(); it != m_anchorSprings.end(); ++it)
	{
		line[0].position = sf::Vector2f((*it).anchor.x, (*it).anchor.y);
		line[0].color = sf::Color::Red;
		line[1].position = sf::Vector2f((*it).particle->position.x, (*it).particle->position.y);
		line[1].color = sf::Color::Red;
		window.draw(line);
	}
	for (std::vector<SpringJoint>::iterator it = m_springJoints.begin(); it != m_springJoints.end(); ++it)
	{
		line[0].position = sf::Vector2f((*it).particle1->position.x, (*it).particle1->position.y);
		line[0].color = sf::Color::Red;
		line[1].position = sf::Vector2f((*it).particle2->position.x, (*it).particle2->position.y);
		line[1].color = sf::Color::Red;
		window.draw(line);
	}
}

void Simulation::ToggleFan()
{
	m_fanActive = !m_fanActive;
}

void Simulation::ToggleDebugDraw()
{
	m_debugDraw = !m_debugDraw;
}

void Simulation::SpawnBalls(int amount)
{
	m_ballSpawner.CreateParticles(amount);
}

void Simulation::InitScene()
{
	Blizzard b1(glm::vec2(300.0f, 100.0f), 0.05f, Blizzard::Direction::COUNTERCLOCKWISE, m_forceRegistry);
	Blizzard b2(glm::vec2(1520.0f, 200.0f), 0.05f, Blizzard::Direction::CLOCKWISE, m_forceRegistry);

	//m_blizzards.push_back(b1);
	//m_blizzards.push_back(b2);

	StaticObject floor(glm::vec2(1920.0f * 0.5f, 1080.0f - 25.0f), glm::vec2(1920.0f, 50.0f), 0.0f);
	StaticObject roof1(glm::vec2(1200.0f, 400.0f), glm::vec2(500.0f, 20.0f), 10.0f);
	StaticObject roof2(glm::vec2(710.0f, 379.0f), glm::vec2(500.0f, 20.0f), -5.0f);
	StaticObject leftWall(glm::vec2(10.0f, 1080.0f * 0.5f), glm::vec2(20.0f, 1080.0f), 0.0f);
	StaticObject rightWall(glm::vec2(1920.0f - 10.0f, 1080.0f * 0.5f), glm::vec2(20.0f, 1080.0f), 0.0f);

	m_staticObjects.push_back(floor);
	m_staticObjects.push_back(roof1);
	m_staticObjects.push_back(roof2);
	m_staticObjects.push_back(leftWall);
	m_staticObjects.push_back(rightWall);

	// cloth
	glm::vec2 vel(0.0f);
	glm::vec2 acc(0.0f);
	float dampening = 0.9f;
	float mass = 1.0f;
	float lifetime = Particle::INFINITE_LIFETIME;
	float size = 15.0f;
	float bounciness = 1.0f;
	Particle* c1 = new Particle(glm::vec2(110.0f, 80.f), vel, acc, dampening, mass, lifetime, size, bounciness);
	Particle* c2 = new Particle(glm::vec2(130.0f, 80.f), vel, acc, dampening, mass, lifetime, size, bounciness);
	Particle* c3 = new Particle(glm::vec2(190.0f, 80.f), vel, acc, dampening, mass, lifetime, size, bounciness);
	Particle* c4 = new Particle(glm::vec2(210.0f, 80.f), vel, acc, dampening, mass, lifetime, size, bounciness);

	m_forceRegistry->Add(c1, ParticleGravityGenerator);
	m_forceRegistry->Add(c2, ParticleGravityGenerator);
	m_forceRegistry->Add(c3, ParticleGravityGenerator);
	m_forceRegistry->Add(c4, ParticleGravityGenerator);

	m_particles.push_back(c1);
	m_particles.push_back(c2);
	m_particles.push_back(c3);
	m_particles.push_back(c4);

	float springConstant = 10.0f;
	float restLength = 20.0f;
	AnchorSpring anch1(glm::vec2(200.0f, 150.0f), c1, springConstant, restLength);
	AnchorSpring anch2(glm::vec2(240.0f, 150.0f), c2, springConstant, restLength);
	AnchorSpring anch3(glm::vec2(280.0f, 150.0f), c3, springConstant, restLength);
	AnchorSpring anch4(glm::vec2(320.0f, 150.0f), c4, springConstant, restLength);

	m_anchorSprings.push_back(anch1);
	m_anchorSprings.push_back(anch2);
	m_anchorSprings.push_back(anch3);
	m_anchorSprings.push_back(anch4);

	SpringJoint spring1(c1, c2, springConstant, restLength);
	SpringJoint spring2(c2, c3, springConstant, restLength);
	SpringJoint spring3(c3, c4, springConstant, restLength);

	m_springJoints.push_back(spring1);
	m_springJoints.push_back(spring2);
	m_springJoints.push_back(spring3);
}

void Simulation::ResolveCollisions()
{
	std::vector<Particle*> ballParticles = m_ballSpawner.GetParticles();

	// check all blizzard particles with the static objects and ball particles
	for (std::vector<Blizzard>::iterator it = m_blizzards.begin(); it != m_blizzards.end(); ++it)
	{
		// for each particle in the blizzard
		std::vector<Particle*> blizzardParticles = (*it).GetParticles();
		for (std::vector<Particle*>::iterator particleIt = blizzardParticles.begin(); particleIt != blizzardParticles.end(); ++particleIt)
		{
			// check static objects
			for (std::vector<StaticObject>::iterator staticIt = m_staticObjects.begin(); staticIt != m_staticObjects.end(); ++staticIt)
			{
				ResolveSingleCollision(*particleIt, *staticIt);
			}

			// check ball particles
			for (std::vector<Particle*>::iterator ballIt = ballParticles.begin(); ballIt != ballParticles.end(); ++ballIt)
			{
				ResolveSingleCollision(*particleIt, *ballIt, CollisionType::DeleteFirst);
			}
		}
	}

	// check all ball spawner with static and other balls
	for (std::vector<Particle*>::iterator particleIt = ballParticles.begin(); particleIt != ballParticles.end(); ++particleIt)
	{
		// check static objects
		for (std::vector<StaticObject>::iterator staticIt = m_staticObjects.begin(); staticIt != m_staticObjects.end(); ++staticIt)
		{
			ResolveSingleCollision(*particleIt, *staticIt);
		}

		// check ball particles
		for (std::vector<Particle*>::iterator ballIt = ballParticles.begin(); ballIt != ballParticles.end(); ++ballIt)
		{
			if(*particleIt != *ballIt)
				ResolveSingleCollision(*particleIt, *ballIt, CollisionType::Reflect);
		}

		// check cloth particles
		for (std::vector<Particle*>::iterator clothIt = m_particles.begin(); clothIt != m_particles.end(); ++clothIt)
		{
			ResolveSingleCollision(*particleIt, *clothIt, CollisionType::Reflect);
		}
	}

	// check cloth particles
	for (std::vector<Particle*>::iterator particleIt = m_particles.begin(); particleIt != m_particles.end(); ++particleIt)
	{
		// check static objects
		for (std::vector<StaticObject>::iterator staticIt = m_staticObjects.begin(); staticIt != m_staticObjects.end(); ++staticIt)
		{
			ResolveSingleCollision(*particleIt, *staticIt);
		}

		// check cloth particles
		for (std::vector<Particle*>::iterator clothIt = m_particles.begin(); clothIt != m_particles.end(); ++clothIt)
		{
			if (*particleIt != *clothIt)
				ResolveSingleCollision(*particleIt, *clothIt, CollisionType::Reflect);
		}
	}
}

void Simulation::ResolveSingleCollision(Particle* particle, StaticObject staticObject)
{
	CollisionInfo collisionInfo;

	// check circle circle
	if (!CircleCircleCollision(particle->circleBound, staticObject.circleBound, collisionInfo))
	{
		return;
	}

	// check circle aabb
	if (!CircleAABBCollision(particle->circleBound, staticObject.aabb, collisionInfo))
	{
		return;
	}
	
	// check for "real" collision
	if (!CircleOOBBCollision(particle->circleBound, staticObject.oobb, collisionInfo))
	{
		return;
	}

	// we have a collision, calculate reflection
	glm::vec2 normal = collisionInfo.normal;

	// reset position
	particle->position -= normal * collisionInfo.depth;

	//float normalStrength = glm::dot(normal, -particle->velocity);
	//particle->velocity -= normal * (normalStrength * particle->bounciness) * 2.0f;

	// calculate new velocity
	// https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
	glm::vec2 relativeVelocity = glm::vec2(0.0f) - particle->velocity;
	float velocityAlongNormal = glm::dot(relativeVelocity, normal);

	float j = -(1 + particle->bounciness) * velocityAlongNormal;
	j /= particle->inverseMass;
	particle->velocity -= particle->inverseMass * (j * normal);

	// check if force is strong enough to break static friction
	// https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-friction-scene-and-jump-table--gamedev-7756
	glm::vec2 normalVelocity = glm::proj(particle->velocity, -normal);
	if (glm::length(normalVelocity) > 20.0f)
	{
		// no friction
		return;
	}

	relativeVelocity = glm::vec2(0.0f) - particle->velocity;
	
	glm::vec2 tangent;
	if (relativeVelocity.x * normal.y - relativeVelocity.y * normal.x < 0.0f)
	{
		tangent = glm::vec2(-normal.y, normal.x);
	}
	else
	{
		tangent = glm::vec2(normal.y, -normal.x);
	}

	float jt = -glm::dot(relativeVelocity, tangent);
	jt /= particle->inverseMass;

	// TODO: static friction of objects
	float staticFriction = 0.25f;

	// clamp friction and differentiate between static and kinetic friction
	glm::vec2 frictionImpulse;
	if (abs(jt) < j * staticFriction)
	{
		// static friction
		frictionImpulse = jt * tangent;
		printf("=> static!\n");
	}
	else
	{
		// dynamic friction
		float dynamicFriction = 0.1f;
		frictionImpulse = -j * tangent * dynamicFriction;
		printf("=> dynamic!\n");
	}

	particle->velocity -= particle->inverseMass * frictionImpulse;
}

void Simulation::ResolveSingleCollision(Particle* particle1, Particle* particle2, CollisionType::Enum collisionType)
{
	CollisionInfo collisionInfo;

	// check circle circle
	if (!CircleCircleCollision(particle1->circleBound, particle2->circleBound, collisionInfo))
	{
		return;
	}

	if(collisionType == CollisionType::DeleteFirst)
	{
		particle1->lifetime = 0.0f;
		return;
	}
	else if(collisionType == CollisionType::DeleteSecond)
	{
		particle2->lifetime = 0.0f;
		return;
	}
	else
	{
		// we have a collision, calculate reflection
		glm::vec2 normal = collisionInfo.normal;

		// resolve interpenetration
		particle1->SetPosition(particle1->position + normal * collisionInfo.depth);
		particle2->SetPosition(particle2->position - normal * collisionInfo.depth);

		//float normalStrength = (glm::dot(collisionInfo.normal, particle1->velocity) + glm::dot(-collisionInfo.normal, particle2->velocity)) / 2.0f;
		//particle1->velocity -= collisionInfo.normal * (normalStrength * particle1->bounciness) * 2.0f;
		//particle2->velocity += collisionInfo.normal * (normalStrength * particle2->bounciness) * 2.0f;

		// https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
		// calculate new velocities
		glm::vec2 relativeVelocity = particle2->velocity - particle1->velocity;
		float velocityAlongNormal = glm::dot(relativeVelocity, normal);

		float e = std::min(particle1->bounciness, particle2->bounciness);
		float j = -(1 + e) * velocityAlongNormal;
		j /= particle1->inverseMass + particle2->inverseMass;

		glm::vec2 impulse = j * normal;
		particle1->velocity -= particle1->inverseMass * impulse;
		particle2->velocity += particle2->inverseMass * impulse;
	}
}
