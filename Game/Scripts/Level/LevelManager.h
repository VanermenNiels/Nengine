#include "Components/ObserverComponent.h"

namespace dae
{
	class PengoGridComponent;
	class HealthComponent;

	class LevelManager final : public ObserverComponent
	{
	public:
		using ObserverComponent::ObserverComponent;
		virtual ~LevelManager() = default;

		void StartLevel();
		void Update(float deltaTime) override;


	protected:
		void EventReaction(Event) override;

	private:
		int m_CurrentLevel{};
		PengoGridComponent* m_GridCompRPtr{};
		HealthComponent* m_Player1Health{};

	};
}