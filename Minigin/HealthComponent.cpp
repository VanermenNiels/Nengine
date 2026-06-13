#include "HealthComponent.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int health, std::vector<EventId> reactsTo):
	ObserverComponent(owner, std::move(reactsTo)),
	m_Health{health},
	m_MaxHealth{health}
{
}

void dae::HealthComponent::EventReaction(Event event)
{
	int amount{ (event.nbArgs > 0) ? event.args[0].intVal : -1 };
	m_Health += amount;

	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;

	m_Subject.Notify(GetOwner(), Event{ dae::make_sdbm_hash("HealthChanged") });
}

