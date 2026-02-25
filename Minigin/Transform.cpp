#include "Transform.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::UpdatePosition(float deltaX, float deltaY, float deltaZ)
{
	m_position.x += deltaX;
	m_position.y += deltaY;
	m_position.z += deltaZ;
}

void dae::Transform::SetPosition(const glm::vec3& position) 
{ 
	m_position = position; 
}