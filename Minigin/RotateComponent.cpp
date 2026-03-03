#include "RotateComponent.h"
#include "GameObject.h"
dae::RotateComponent::RotateComponent(GameObject* owner, float speed, float radius):
	BaseComponent(owner), m_Speed{ speed }, m_Radius{ radius }
{
}

void dae::RotateComponent::Update(float deltaTime)
{
	m_Angle += m_Speed * deltaTime;
	
	if (m_Angle > (2 * m_PI))
		m_Angle -= (2 * m_PI);

	float x{ GetOwner()->GetLocalPosition().x + m_Radius * std::cosf(m_Angle)};
	float y{ GetOwner()->GetLocalPosition().y + m_Radius * std::sinf(m_Angle) };

	GetOwner()->SetLocalPosition({ x, y, 0 });
}
