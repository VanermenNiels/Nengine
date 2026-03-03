#include "Transform.h"

void dae::Transform::SetWorldPosition(const float x, const float y, const float z)
{
	m_WorldPosition.x = x;
	m_WorldPosition.y = y;
	m_WorldPosition.z = z;
}

void dae::Transform::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
}

void dae::Transform::UpdatePosition(float deltaX, float deltaY, float deltaZ)
{
	m_WorldPosition.x += deltaX;
	m_WorldPosition.y += deltaY;
	m_WorldPosition.z += deltaZ;
}

void dae::Transform::SetWorldPosition(const glm::vec3& position)
{ 
	m_WorldPosition = position; 
}

void dae::Transform::SetLocalPosition(const glm::vec3& localPosition)
{
	m_LocalPosition = localPosition;
}
