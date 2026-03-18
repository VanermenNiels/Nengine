
#include "BaseComponent.h"
namespace dae
{
	class HealthComponent final : public BaseComponent
	{
	public:
		HealthComponent( GameObject* owner, int health);

		void AlterHealth(int alterAmount); // - for damage , + for healing
		void AlterMaxHealth(int alterAmount) { m_MaxHealth += alterAmount; }

		int GetHealth() const { return m_Health; }
		bool IsDead()   const { return (m_Health <= 0); }
	private:
		int m_Health;
		int m_MaxHealth;

	};
}