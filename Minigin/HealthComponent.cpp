#include "HealthComponent.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int health, std::vector<EventId> reactsTo):
	BaseComponent(owner),
	m_Health{health},
	m_MaxHealth{health},
	m_ReactsTo{ std::move(reactsTo) }
{
}

void dae::HealthComponent::AlterHealth(int alterAmount)
{
	m_Health += alterAmount;

	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;

	m_Subject.Notify(GetOwner(), Event{ dae::make_sdbm_hash("HealthChanged") });
}

void dae::HealthComponent::onNotify(GameObject*, Event event)
{
	if (m_ReactsTo.empty()) return;
	const int amount { (event.nbArgs > 0) ? event.args[0].intVal : 1 };

	for (const auto& reactEvent : m_ReactsTo)
	{
		if (event.id == reactEvent)
			AlterHealth(amount);
	}

}
