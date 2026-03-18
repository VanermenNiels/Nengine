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

	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;

	if (m_SubjectRPtr)
		m_SubjectRPtr->Notify(GetOwner(), Event{ dae::make_sdbm_hash("HealthChanged") });
}

void dae::HealthComponent::onNotify(GameObject*, Event event)
{
	if (event.id == make_sdbm_hash("Damage"))
	{
		AlterHealth(-1);
	}
	else if (event.id == make_sdbm_hash("Heal"))
	{
		AlterHealth(1);
	}
}
