#pragma once
#include "BaseComponent.h"
#include "Events/Subject.h"
#include "Observers/Observer.h"
#include "Events/Event.h"

namespace dae
{
	class HealthComponent final : public BaseComponent, public Observer
	{
	public:
		HealthComponent(GameObject* owner, int health, std::vector<EventId> reactsTo = {});

		void AlterHealth(int alterAmount);
		void AlterMaxHealth(int alterAmount) { m_MaxHealth += alterAmount; }

		int  GetHealth() const { return m_Health; }
		bool IsDead()    const { return (m_Health <= 0); }

		void onNotify(GameObject*, Event event) override;

		void AddObserver(Observer* o) { m_Subject.AddObserver(o); }
		void RemoveObserver(Observer* o) { m_Subject.RemoveObserver(o); }

	private:
		int m_Health;
		int m_MaxHealth;

		Subject m_Subject{};
		std::vector<EventId> m_ReactsTo;
	};
}