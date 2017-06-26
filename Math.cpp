#include "Math.h"

glm::vec2 Math::SaveNormalize(const glm::vec2& vector)
{
	glm::vec2 normalized = glm::normalize(vector);

	// true if one is NaN
	if (glm::isnan(normalized.x) || glm::isnan(normalized.y))
	{
		normalized.x = 0.0f;
		normalized.y = 0.0f;
	}

	return normalized;
}

float Math::SaveLength(const glm::vec2& vector)
{
	float length = glm::length(vector);

	if (glm::isnan(length))
	{
		length = 0.0f;
	}

	return length;
}

float Math::SaveDistance(const glm::vec2& vector1, const glm::vec2& vector2)
{
	float distance = glm::distance(vector1, vector2);

	if (glm::isnan(distance))
	{
		distance = 0.0f;
	}

	return distance;
}
