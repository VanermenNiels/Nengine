#include "PushingState.h"
#include "PengoStateComponent.h"
#include "AnimatorCOmponent.h"
#include "PengoIdleState.h"

void dae::PengoPushingState::OnEnter(PengoStateComponent& ctx)
{
	m_FrameCount = 2;
	m_FrameDuration = 1.f;
	m_StartCol = static_cast<int>(m_Direction) * m_FrameCount;
	m_StartRow += 1;

	ctx.GetAnimatorComp()
		->PlayAnimation(m_StartCol, m_StartRow, m_FrameWidth,
						m_FrameHeight, m_FrameCount, m_FrameDuration, false);
}

void dae::PengoPushingState::OnExit(PengoStateComponent& ctx)
{
	ctx.SetState(std::make_unique<PengoIdleState>(m_Player), false);
}
