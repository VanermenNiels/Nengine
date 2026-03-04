#include "Transform.h"

void dae::Transform::SetWorldPosition(const float x, const float y, const float z)
{
	m_WorldPos.x = x;
	m_WorldPos.y = y;
	m_WorldPos.z = z;
}

void dae::Transform::SetLocalPosition(float x, float y, float z)
{
	m_LocalPos.x = x;
	m_LocalPos.y = y;
	m_LocalPos.z = z;
}

void dae::Transform::UpdateWorldPosition(float deltaX, float deltaY, float deltaZ)
{
	m_WorldPos.x += deltaX;
	m_WorldPos.y += deltaY;
	m_WorldPos.z += deltaZ;
}

void dae::Transform::UpdateLocalPosition(float deltaX, float deltaY, float deltaZ)
{
	m_LocalPos.x += deltaX;
	m_LocalPos.y += deltaY;
	m_LocalPos.z += deltaZ;
}

void dae::Transform::SetWorldPosition(const glm::vec3& position)
{ 
	m_WorldPos = position;
}

void dae::Transform::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPos = position;
}
