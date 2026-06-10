#include "PengoIdleState.h"
#include "Components/PengoStateComponent.h"
#include "AnimatorComponent.h"

void dae::PengoIdleState::OnEnter(StateComponent& ctx, GameObject* gO)
{
	m_FrameCount = 4;
	m_FrameDuration = 0.5f;
	m_StartCol = 4;
	m_StartRow += 2;

	static_cast<PengoStateComponent&>(ctx).GetAnimatorComp()
		->PlayAnimation(m_StartCol, m_StartRow, m_FrameWidth,
						m_FrameHeight, m_FrameCount, m_FrameDuration);
}