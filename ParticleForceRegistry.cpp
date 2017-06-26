#include "ParticleForceRegistry.h"

#include <algorithm>

ParticleForceRegistry::ParticleForceRegistry()
{
}


ParticleForceRegistry::~ParticleForceRegistry()
{
}

void ParticleForceRegistry::Add(Particle* particle, ParticleForceGenerator* forceGenerator)
{
	m_registrations.push_back({ particle, forceGenerator });
}

void ParticleForceRegistry::Remove(Particle* particle, ParticleForceGenerator* forceGenerator)
{
	m_registrations.erase(
		std::remove_if(m_registrations.begin(), m_registrations.end(), [&](const ParticleForceRegistration& registration) {
			return registration.particle == particle && registration.forceGenerator == forceGenerator;
		}),
		m_registrations.end());
}

void ParticleForceRegistry::Clear()
{
	m_registrations.clear();
}

void ParticleForceRegistry::UpdateForces(float duration)
{
	for(Registry::iterator it = m_registrations.begin(); it != m_registrations.end(); ++it)
	{
		it->forceGenerator->UpdateForce(it->particle, duration);
	}
}
