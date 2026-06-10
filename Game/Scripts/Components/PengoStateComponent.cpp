#include "PengoStateComponent.h"
#include "PengoState.h"
#include "PengoIdleState.h"
#include "GameObject.h"
#include "AnimatorComponent.h"
#include "../EventIDs.h"
#include "States/PengoWalkingStates.h"
#include "States/PushingState.h"

dae::PengoStateComponent::PengoStateComponent( GameObject* owner, PengoGridComponent* grid, std::vector<EventId> eventIds, int player)
    : StateComponent(owner, eventIds), m_Player{player}, m_GridRPtr{grid}
{
    SetState(std::make_unique<PengoIdleState>());
}

dae::AnimatorComponent* dae::PengoStateComponent::GetAnimatorComp()
{
    return GetOwner()->GetComponent<AnimatorComponent>();
}

void dae::PengoStateComponent::EventReaction(Event event)
{
    if (event.id == dae::EventIDs::IceBlockPushed[m_Player])
    {

		if (static_cast<PengoState*>(m_CurrentState.get())->IsPushing())
			static_cast<PengoPushingState*>(m_CurrentState.get())->Push();
    }
    else if (event.id == dae::EventIDs::PlayerMoved[m_Player])
    {
        m_CurrentDir = static_cast<Direction>(event.args[0].intVal);
        // always show the most recently pressed direction
        SetState(std::make_unique<PengoWalkingState>(m_Player, m_CurrentDir, m_GridRPtr));
    }
    else if (event.id == dae::EventIDs::PlayerStop[m_Player])
    {
        // this makes sure that if multiple buttons are pressed and one goes up that it doesn't switch states
        const auto dir { static_cast<Direction>(event.args[0].intVal) };
        if (dir == m_CurrentDir)
        {
			m_CurrentDir = Direction::Static;
            SetState(std::make_unique<PengoIdleState>());
        }
    }
}