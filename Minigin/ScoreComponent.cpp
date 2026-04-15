#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject* owner, std::vector<EventId> reactsTo, int score):
	ObserverComponent(owner, std::move(reactsTo)),
	m_Score { score }
{
}     

void dae::ScoreComponent::AddScore(int scoreToAdd)
{
	m_Score += scoreToAdd;
	m_Subject.Notify(GetOwner(), Event{ make_sdbm_hash("ScoreChanged") });
}

void dae::ScoreComponent::EventReaction(Event event)
{
	const int scoreToAdd{ (event.nbArgs > 0) ? event.args[0].intVal : 1 };
	m_Score += scoreToAdd;
	m_Subject.Notify(GetOwner(), Event{ make_sdbm_hash("ScoreChanged") });
}
