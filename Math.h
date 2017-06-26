#pragma once

#include <glm/glm.hpp>

namespace Math
{
	glm::vec2 SaveNormalize(const glm::vec2& vector);
	float SaveLength(const glm::vec2& vector);
	float SaveDistance(const glm::vec2& vector1, const glm::vec2& vector2);
}