#include "StaticObject.h"
#include "Math.h"

StaticObject::StaticObject()
	: StaticObject(glm::vec2(0.0f), glm::vec2(0.0f))
{
}

StaticObject::StaticObject(glm::vec2 position, glm::vec2 size)
	: StaticObject(position, size, 0.0f)
{
}

StaticObject::StaticObject(glm::vec2 position, glm::vec2 size, float rotation)
	: position(position)
	, size(size)
	, rotation(rotation)
{
	CalculateBoundingVolumes();
}


StaticObject::~StaticObject()
{
}

void StaticObject::CalculateBoundingVolumes()
{
	glm::vec2 halfSize = size * 0.5f;

	// circle bounds
	circleBound.center = position;
	circleBound.radius = Math::SaveLength(halfSize);

	// OOBB
	oobb.center = position;
	oobb.halfSize = halfSize;
	oobb.rotation = rotation;

	oobb.corners[0] = position - halfSize;
	oobb.corners[1] = glm::vec2(position.x + halfSize.x, position.y - halfSize.y);
	oobb.corners[2] = position + halfSize;
	oobb.corners[3] = glm::vec2(position.x - halfSize.x, position.y + halfSize.y);

	float rotationRadians = glm::radians(rotation);

	for (int i = 0; i < 4; ++i)
	{
		// translate to origin
		float tempX = oobb.corners[i].x - position.x;
		float tempY = oobb.corners[i].y - position.y;

		// apply rotation
		float rotatedX = tempX * glm::cos(rotationRadians) - tempY * glm::sin(rotationRadians);
		float rotatedY = tempX * glm::sin(rotationRadians) + tempY * glm::cos(rotationRadians);

		// translate back
		oobb.corners[i].x = rotatedX + position.x;
		oobb.corners[i].y = rotatedY + position.y;
	}

	oobb.axis[0] = Math::SaveNormalize(oobb.corners[1] + oobb.corners[0]);
	oobb.axis[1] = Math::SaveNormalize(oobb.corners[3] + oobb.corners[0]);

	oobb.CalculateTransform();

	// AABB
	glm::vec2 maxSeperation(0.0f);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			glm::vec2 seperation = glm::abs(oobb.corners[i] - oobb.corners[j]);

			if (seperation.x > maxSeperation.x)
				maxSeperation.x = seperation.x;
			if (seperation.y > maxSeperation.y)
				maxSeperation.y = seperation.y;
		}
	}

	aabb.center = position;
	aabb.halfSize = maxSeperation * 0.5f;
}
