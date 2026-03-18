#include "HealthComponent.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int health):
	BaseComponent(owner),
	m_Health{health},
	m_MaxHealth{health}
{
}

void dae::HealthComponent::AlterHealth(int alterAmount)
{
	m_Health += alterAmount;

	if (m_Health <= 0)
		m_Health = m_MaxHealth;
	else if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;
}
