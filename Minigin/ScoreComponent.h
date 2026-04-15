#pragma once
#include "Components/ObserverComponent.h"
#include "Events/Event.h"

namespace dae
{
	class ScoreComponent final : public ObserverComponent
	{
	public:
		ScoreComponent(GameObject* owner, std::vector<EventId> reactsTo = {}, int score = 0);

		void AddScore(int scoreToAdd);
		int  GetScore() const { return m_Score; }

	protected:
		void EventReaction(Event event) override;

	private:
		int m_Score{};
	};
}