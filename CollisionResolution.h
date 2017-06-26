#pragma once

#include <glm/glm.hpp>
#include "BoundingVolumes.h"

struct CollisionInfo
{
	glm::vec2 normal;
	float depth;
};

bool CircleCircleCollision(const CircleBound& b1, const CircleBound& b2, CollisionInfo& collisionInfo);
bool CircleAABBCollision(const CircleBound& b1, const AABB& b2, CollisionInfo& collisionInfo);
bool CircleOOBBCollision(const CircleBound& b1, const OOBB& b2, CollisionInfo& collisionInfo);