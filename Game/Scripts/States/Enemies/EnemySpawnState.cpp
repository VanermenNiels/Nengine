#include "EnemySpawnState.h"
#include "EnemyStateComponent.h"
#include "AnimatorComponent.h"

void dae::EnemySpawnState::OnEnter(StateComponent& ctx, GameObject* gO)
{
	m_StartCol = 0;
	m_FrameCount = 8;
	m_FrameDuration = 0.2f;
	m_AnimCompRPtr = static_cast<EnemyStateComponent&>(ctx).GetAnimatorComp();
	if (m_AnimCompRPtr)
		m_AnimCompRPtr->PlayAnimation(m_StartCol, m_StartRow, m_FrameWidth, m_FrameHeight, m_FrameCount, m_FrameDuration, false);
}

void dae::EnemySpawnState::Update(StateComponent& ctx, GameObject* gO, float deltaTime)
{
	if (!m_AnimCompRPtr) return;
	if (m_AnimCompRPtr->AnimationFinished())
	{
		m_AnimCompRPtr = nullptr; // prevent re-triggering every frame
		static_cast<EnemyStateComponent&>(ctx).StartMoving();
	}
}
