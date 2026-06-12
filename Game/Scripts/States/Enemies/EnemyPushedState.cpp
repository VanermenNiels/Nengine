#include "EnemyPushedState.h"
#include "GameObject.h"
#include "Components/PengoGridComponent.h"
#include "Components/EnemyStateComponent.h"
#include "AnimatorComponent.h"

void dae::EnemyPushedState::OnEnter(StateComponent& ctx, GameObject* gO)
{
	
	gO->SetPosition(m_StartPos.x, m_StartPos.y);

    Direction animationDir{};

    switch (m_CurrentDir)
    {
    case Direction::Down:
        animationDir = Direction::Up;
        break;
    case Direction::Left:
        animationDir = Direction::Right;
        break;
    case Direction::Up:
        animationDir = Direction::Down;
        break;
    case Direction::Right:
        animationDir = Direction::Left;
        break;
    }

    // direction vectors per enum value
    constexpr glm::vec3 dirVectors[] = {
        { 0,  1, 0 },  // Down
        { -1, 0, 0 },  // Left
        { 0, -1, 0 },  // Up
        { 1,  0, 0 },  // Right
        { 0,  0, 0 },  // Static
    };

    m_MoveCommand = std::make_unique<MoveCommand>(
        gO,
        dirVectors[static_cast<int>(m_CurrentDir)],
        m_Speed
    );

    m_FrameCount = 1;
    m_FrameDuration = 0.1f;
    m_StartCol = static_cast<int>(animationDir) * m_FrameCount;
    m_StartRow += 1;

    static_cast<EnemyStateComponent&>(ctx).GetAnimatorComp()->PlayAnimation(
        m_StartCol, m_StartRow, m_FrameWidth,
        m_FrameHeight, m_FrameCount, m_FrameDuration
    );
}

void dae::EnemyPushedState::Update(StateComponent& ctx, GameObject* gO, float deltaTime)
{
	if (!m_MoveCommand) return;
		
	m_MoveCommand->Execute(deltaTime);
}
