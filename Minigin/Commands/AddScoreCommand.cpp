#include "AddScoreCommand.h"
#include "ScoreComponent.h"
#include "GameObject.h"

dae::AddScoreCommand::AddScoreCommand(GameObject* target, int scoreIncrement):
	Command(target),
	m_ScoreIncrement { scoreIncrement }
{
}

void dae::AddScoreCommand::Execute(float)
{
	if (auto scoreComponent = m_TargetGameObjectRPtr->GetComponent<ScoreComponent>())
		scoreComponent->AddScore(m_ScoreIncrement);
}
