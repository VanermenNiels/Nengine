#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject* owner, int score):
	BaseComponent(owner),
	m_Score { score }
{
}

void dae::ScoreComponent::AddScore(int scoreToAdd)
{
	m_Score += scoreToAdd;
	m_Subject.Notify(GetOwner(), Event{ make_sdbm_hash("ScoreChanged") });
}

void dae::ScoreComponent::onNotify(GameObject*, Event event)
{
	if (event.id == make_sdbm_hash("AddScore"))
	{
		const int amount{ (event.nbArgs > 0) ? event.args[0].intVal : 10 };
		AddScore(amount);
	}
}
