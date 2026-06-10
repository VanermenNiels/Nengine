#include "StateComponent.h"
#include "State.h"

void dae::StateComponent::Update(float deltaTime)
{
    if (m_CurrentState)
        m_CurrentState->Update(*this, GetOwner(), deltaTime);
}

void dae::StateComponent::SetState(std::unique_ptr<State> newState, bool fireOnExit)
{
    if (m_CurrentState && fireOnExit)
        m_CurrentState->OnExit(*this, GetOwner());

    m_CurrentState = std::move(newState);

    if (m_CurrentState)
        m_CurrentState->OnEnter(*this, GetOwner());
}
