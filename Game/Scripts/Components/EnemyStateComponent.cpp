#include "EnemyStateComponent.h"
#include "AnimatorComponent.h"
#include "GameObject.h"

dae::EnemyStateComponent::EnemyStateComponent(GameObject* owner, PengoGridComponent* grid, std::vector<EventId> eventIds):
	StateComponent(owner, eventIds),
	m_GridRPtr{grid}
{}

dae::AnimatorComponent* dae::EnemyStateComponent::GetAnimatorComp()
{
	return GetOwner()->GetComponent<AnimatorComponent>();
}

void dae::EnemyStateComponent::EventReaction(Event)
{

}
