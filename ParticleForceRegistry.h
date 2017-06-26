#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"
#include <vector>

class ParticleForceRegistry
{
public:
	ParticleForceRegistry();
	~ParticleForceRegistry();

	void Add(Particle* particle, ParticleForceGenerator* forceGenerator);
	void Remove(Particle* particle, ParticleForceGenerator* forceGenerator);
	void Clear();

	void UpdateForces(float duration);

protected:
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* forceGenerator;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry m_registrations;
};

