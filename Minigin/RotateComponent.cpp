#include "RotateComponent.h"
#include "GameObject.h"
dae::RotateComponent::RotateComponent(GameObject* owner, float speed, float radius):
	BaseComponent(owner), m_Speed{ speed }, m_Radius{ radius }
{
}

void dae::RotateComponent::Update(float deltaTime)
{
	auto owner { GetOwner() };

	m_Angle += m_Speed * deltaTime;
	
	if (m_Angle > (2 * m_PI))
		m_Angle -= (2 * m_PI);

	float x{ m_RotationPoint.x + m_Radius * std::cosf(m_Angle) };
	float y{ m_RotationPoint.y + m_Radius * std::sinf(m_Angle) };

	owner->SetLocalPosition({ x, y, 0 });
}
