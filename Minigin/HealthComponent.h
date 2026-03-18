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
		HealthComponent( GameObject* owner, int health);

		void AlterHealth(int alterAmount); // - for damage , + for healing
		void AlterMaxHealth(int alterAmount) { m_MaxHealth += alterAmount; }

		int GetHealth() const { return m_Health; }
		bool IsDead()   const { return (m_Health <= 0); }

		void onNotify(GameObject*, Event event) override;
		void SetSubject(Subject* subject) { m_SubjectRPtr = subject; }
	private:
		int m_Health;
		int m_MaxHealth;

		Subject* m_SubjectRPtr{};
	};
}