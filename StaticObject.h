#pragma once

#include <glm/glm.hpp>
#include "BoundingVolumes.h"

class StaticObject
{
public:
	StaticObject();
	StaticObject(glm::vec2 position, glm::vec2 size);
	StaticObject(glm::vec2 position, glm::vec2 size, float rotation);
	~StaticObject();

	glm::vec2 position;
	glm::vec2 size;

	CircleBound circleBound;
	AABB aabb;
	OOBB oobb;

	float rotation;

protected:
	void CalculateBoundingVolumes();
};

