#include "PengoStateComponent.h"
#include "PengoState.h"
#include "PengoIdleState.h"
#include "GameObject.h"
#include "AnimatorComponent.h"
#include "../EventIDs.h"
#include "States/PengoWalkingStates.h"

dae::PengoStateComponent::PengoStateComponent(GameObject* owner, std::vector<EventId> eventIds, int player)
    : ObserverComponent(owner, eventIds), m_Player(player)
{
    SetState(std::make_unique<PengoIdleState>());
}

void dae::PengoStateComponent::Update(float deltaTime)
{
    if (m_CurrentState)
        m_CurrentState->Update(*this, deltaTime);
}

void dae::PengoStateComponent::SetState(std::unique_ptr<PengoState> newState, bool fireOnExit)
{
    if (m_CurrentState && fireOnExit)
        m_CurrentState->OnExit(*this);

    m_CurrentState = std::move(newState);

    if (m_CurrentState)
        m_CurrentState->OnEnter(*this);
}

dae::AnimatorComponent* dae::PengoStateComponent::GetAnimatorComp()
{
    return GetOwner()->GetComponent<AnimatorComponent>();
}

void dae::PengoStateComponent::EventReaction(Event event)
{
    if (event.id == dae::EventIDs::ReadyPushIceBlock[m_Player])
    {
        // push logic here
    }
    else if (event.id == dae::EventIDs::PlayerMoved[m_Player])
    {
        const auto dir = static_cast<Direction>(event.args[0].intVal);
        m_ActiveDirections.insert(dir);
        SetState(std::make_unique<PengoWalkingState>(m_Player, dir));
    }
    else if (event.id == dae::EventIDs::PlayerStop[m_Player])
    {
        const auto dir = static_cast<Direction>(event.args[0].intVal);
        m_ActiveDirections.erase(dir);

        if (!m_ActiveDirections.empty())
        {
            // still holding another key — use the most recently inserted direction
            const auto remainingDir = *m_ActiveDirections.rbegin();
            SetState(std::make_unique<PengoWalkingState>(m_Player, remainingDir));
        }
        else
        {
            SetState(std::make_unique<PengoIdleState>());
        }
    }
}