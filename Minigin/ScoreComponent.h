#pragma once
#include "BaseComponent.h"
#include "Events/Subject.h"
#include "Observers/Observer.h"
#include "Events/Event.h"

namespace dae
{
	class ScoreComponent final : public BaseComponent, public Observer
	{
	public:
		ScoreComponent(GameObject* owner, int score = 0);

		void AddScore(int scoreToAdd);
		int  GetScore() const { return m_Score; }

		void onNotify(GameObject*, Event event) override;

		void AddObserver(Observer* o) { m_Subject.AddObserver(o); }
		void RemoveObserver(Observer* o) { m_Subject.RemoveObserver(o); }

	private:
		int m_Score{};

		Subject m_Subject{};
	};
}