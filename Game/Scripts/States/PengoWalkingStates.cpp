#include "PengoWalkingStates.h"
#include "PengoStateComponent.h"
#include "AnimatorComponent.h"
#include "PengoIdleState.h"

void dae::PengoWalkingState::OnEnter(PengoStateComponent& ctx)
{
	m_FrameCount = 2;
	m_FrameDuration = 0.1f;
	m_StartCol = static_cast<int>(m_Direction) * m_FrameCount;
	m_StartRow += 0;

    ctx.GetAnimatorComp()
		->PlayAnimation(m_StartCol, m_StartRow, m_FrameWidth, 
						m_FrameHeight, m_FrameCount, m_FrameDuration);
}

void dae::PengoWalkingState::OnExit(PengoStateComponent& ctx) 
{
	ctx.SetState(std::make_unique<PengoIdleState>(m_Player), false);
}