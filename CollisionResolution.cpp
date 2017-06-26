#include "CollisionResolution.h"

#include <glm/gtx/matrix_transform_2d.hpp>

bool CircleCircleCollision(const CircleBound & circle1, const CircleBound & circle2, CollisionInfo& collisionInfo)
{
	float distance = glm::distance(circle1.center, circle2.center);
	
	if(distance > (circle1.radius + circle2.radius))
	{
		return false;
	}

	collisionInfo.normal = glm::normalize(circle1.center - circle2.center);
	collisionInfo.depth = (circle1.radius + circle2.radius) - distance;

	return true;
}

bool CircleAABBCollision(const CircleBound & circle, const AABB & aabb, CollisionInfo& collisionInfo)
{
	glm::vec2 distanceVector = circle.center - aabb.center;
	distanceVector.x = glm::clamp(distanceVector.x, -aabb.halfSize.x, aabb.halfSize.x);
	distanceVector.y = glm::clamp(distanceVector.y, -aabb.halfSize.y, aabb.halfSize.y);

	glm::vec2 closest = aabb.center + distanceVector;
	float distance = glm::length(closest - circle.center);

	// no collision
	if(distance > circle.radius)
	{
		return false;
	}

	collisionInfo.normal = glm::normalize(closest - circle.center);
	collisionInfo.depth = circle.radius - distance;

	return true;
}

bool CircleOOBBCollision(const CircleBound& circle, const OOBB& oobb, CollisionInfo& collisionInfo)
{
	// transform circle and oobb to oobb box space
	glm::vec2 transCircleCenter(oobb.invTransform * glm::vec3(circle.center, 1.0f));
	glm::vec2 transOOBBCenter(oobb.invTransform * glm::vec3(oobb.center, 1.0f));

	glm::vec2 distanceVector = transCircleCenter - transOOBBCenter;
	distanceVector.x = glm::clamp(distanceVector.x, -oobb.halfSize.x, oobb.halfSize.x);
	distanceVector.y = glm::clamp(distanceVector.y, -oobb.halfSize.y, oobb.halfSize.y);

	glm::vec2 closest = transOOBBCenter + distanceVector;
	float distance = glm::length(closest - transCircleCenter);

	// no collision
	if (distance > circle.radius)
	{
		return false;
	}

	collisionInfo.normal = glm::vec2(oobb.transform * glm::vec3(glm::normalize(closest - transCircleCenter), 0.0f));
	collisionInfo.depth = circle.radius - distance;

	return true;
}
