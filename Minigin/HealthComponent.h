#pragma once
#include "Components/ObserverComponent.h"

namespace dae
{
	class HealthComponent final : public ObserverComponent
	{
	public:
		HealthComponent(GameObject* owner, int health, std::vector<EventId> reactsTo = {});

		void AlterMaxHealth(int alterAmount) { m_MaxHealth += alterAmount; }

		int  GetHealth() const { return m_Health; }
		bool IsDead()    const { return (m_Health <= 0); }

	protected:
		void EventReaction(Event event) override;

	private:
		int m_Health;
		int m_MaxHealth;

	};
}