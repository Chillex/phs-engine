#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

struct CircleBound
{
	glm::vec2 center;
	float radius;
};

struct AABB
{
	glm::vec2 center;
	glm::vec2 halfSize;
};

struct OOBB
{
	glm::vec2 center;
	glm::vec2 halfSize;
	glm::vec2 axis[2];
	glm::vec2 corners[4];
	float rotation;

	glm::mat3 transform;
	glm::mat3 invTransform;

	void CalculateTransform(void)
	{
		glm::mat3 trans(1.0f);
		trans = glm::rotate(trans, rotation);
		trans = glm::translate(trans, center);
		transform = trans;

		invTransform = glm::inverse(transform);
	}
};
