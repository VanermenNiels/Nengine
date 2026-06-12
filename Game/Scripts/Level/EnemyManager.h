#include <vector>
#include "Components/ObserverComponent.h"
namespace dae
{
	class EnemyStateComponent;
	class EnemyManager : public ObserverComponent
	{
	public:
		using ObserverComponent::ObserverComponent;

		void AddEnemy(EnemyStateComponent* enemy);

	protected:
		void EventReaction(Event event) override;
	private:
		std::vector<EnemyStateComponent*> m_Enemies{};

		int m_EnemiesOnField{};
		int m_EnemiesInEgg{};

	};
}