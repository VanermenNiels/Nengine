#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject* owner, int score):
	BaseComponent(owner),
	m_Score { score }
{
}

void dae::ScoreComponent::AddScore(int scoreToAdd)
{
	m_Score += scoreToAdd;

	if (m_SubjectRPtr)
		m_SubjectRPtr->Notify(GetOwner(), Event{ make_sdbm_hash("ScoreChanged") });
}

void dae::ScoreComponent::onNotify(GameObject*, Event event)
{
	if (event.id == make_sdbm_hash("AddScore"))
	{
		AddScore(10);
	}
}
