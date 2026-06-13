#include "EnemyStateComponent.h"
#include "AnimatorComponent.h"
#include "GameObject.h"
#include "PengoGridComponent.h"
#include "States/Enemies/EnemySpawnState.h"
#include "States/Enemies/EnemyWalkingState.h"
#include <memory>
#include "../EventIDs.h"

int dae::EnemyStateComponent::ENEMIES_ON_FIELD { 3 };
int dae::EnemyStateComponent::ENEMIES_IN_EGG { 3 };


dae::EnemyStateComponent::EnemyStateComponent(GameObject* owner, PengoGridComponent* grid, bool inEgg, std::vector<EventId> eventIds) :
	StateComponent(owner, eventIds),
	m_GridRPtr { grid },
	m_InEgg { inEgg }
{

	if (inEgg)
		GetAnimatorComp()->PlayAnimation(0, 0, 1, 1, 4, 0.1f); // Enemy stays dormant
	else SpawnEnemy();
}

dae::EnemyStateComponent::~EnemyStateComponent()
{
	if (!m_InEgg)
	{
		--ENEMIES_IN_EGG;
		if (ENEMIES_IN_EGG > 0) ++ENEMIES_ON_FIELD;
	}
	else --ENEMIES_IN_EGG;
}

void dae::EnemyStateComponent::SpawnEnemy()
{
	SetState(std::make_unique<EnemySpawnState>());
	m_InEgg = false;
}

void dae::EnemyStateComponent::StartMoving()
{
	SetState(std::make_unique<EnemyWalkingState>(m_CurrentDir, m_GridRPtr));
}

void dae::EnemyStateComponent::SetDead()
{
	m_Dead = true;
	m_GridRPtr->RemoveEnemyObject(GetOwner());
}

dae::AnimatorComponent* dae::EnemyStateComponent::GetAnimatorComp()
{
	return GetOwner()->GetComponent<AnimatorComponent>();
}

void dae::EnemyStateComponent::EventReaction(Event)
{
	
}
