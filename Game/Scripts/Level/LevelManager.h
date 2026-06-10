#include "Components/ObserverComponent.h"

namespace dae
{
	class PengoGridComponent;
	class LevelManager final : public ObserverComponent
	{
	public:
		using ObserverComponent::ObserverComponent;
		virtual ~LevelManager() = default;

		void StartLevel();


	protected:
		void EventReaction(Event) override;

	private:
		int m_CurrentLevel{};
		PengoGridComponent* m_GridCompRPtr{};
	};
}