#include "EnemySquishedState.h"
#include "AnimatorComponent.h"
#include "Components/EnemyStateComponent.h"
void dae::EnemySquishedState::OnEnter(StateComponent& ctx, GameObject* gO)
{
    m_FrameCount = 2;
    m_FrameDuration = 0.1f;
    m_StartCol = static_cast<int>(m_CurrentDir) * m_FrameCount;
    m_StartRow += 4;

    static_cast<EnemyStateComponent&>(ctx).GetAnimatorComp()->PlayAnimation(
        m_StartCol, m_StartRow, m_FrameWidth,
        m_FrameHeight, m_FrameCount, m_FrameDuration, false
    );
}

void dae::EnemySquishedState::Update(StateComponent& ctx, GameObject* gO, float deltaTime)
{
    if (static_cast<EnemyStateComponent&>(ctx).GetAnimatorComp()->AnimationFinished())
    {
        static_cast<EnemyStateComponent&>(ctx).GetAnimatorComp()->PlayAnimation(0, 0, 0, 0, 0, 0);
        static_cast<EnemyStateComponent&>(ctx).SetDead();
    }
}
