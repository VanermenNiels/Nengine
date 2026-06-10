#include "PushingState.h"
#include "Components/PengoStateComponent.h"
#include "AnimatorCOmponent.h"
#include "PengoIdleState.h"
#include "Components/BlockComponent.h"

void dae::PengoPushingState::OnEnter(StateComponent& ctx, GameObject*)
{
	m_FrameCount = 2;
	m_FrameDuration = 0.3f;
	m_StartCol = static_cast<int>(m_Direction) * m_FrameCount;
	m_StartRow += 1;

	static_cast<PengoStateComponent&>(ctx).GetAnimatorComp()
		->PlayAnimation(m_StartCol, m_StartRow, m_FrameWidth,
						m_FrameHeight, m_FrameCount, m_FrameDuration, false);
}

void dae::PengoPushingState::OnExit(StateComponent& ctx, GameObject*)
{
	ctx.SetState(std::make_unique<PengoIdleState>(m_Player), false);
}

void dae::PengoPushingState::Push()
{
	m_BlockComp->Push(m_Direction);
}
