#include "PengoWalkingStates.h"
#include "Components/PengoStateComponent.h"
#include "AnimatorComponent.h"
#include "PengoIdleState.h"
#include "PengoGridComponent.h"
#include "../Variables.h"
#include "PushingState.h"

void dae::PengoWalkingState::OnEnter(StateComponent& ctx, GameObject* gO)
{
    m_FrameCount = 2;
    m_FrameDuration = 0.1f;
    m_StartCol = static_cast<int>(m_Direction) * m_FrameCount;

    static_cast<PengoStateComponent&>(ctx).GetAnimatorComp()->PlayAnimation(m_StartCol, m_StartRow, m_FrameWidth,
        m_FrameHeight, m_FrameCount, m_FrameDuration);

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
        dirVectors[static_cast<int>(m_Direction)],
        m_Speed
    );
}

void dae::PengoWalkingState::OnExit(StateComponent& ctx, GameObject*)
{
    m_MoveCommand.reset();
    ctx.SetState(std::make_unique<PengoIdleState>(m_Player), false);
}

void dae::PengoWalkingState::Update(StateComponent& ctx, GameObject* gO, float deltaTime)
{
    if (m_GridRPtr)
    {
        float halfCell = m_GridRPtr->GetCellSize() / 2.f;

        // center of pengo
        glm::vec2 pengoCenter = {
            gO->GetWorldPosition().x + halfCell,
            gO->GetWorldPosition().y + halfCell
        };

        // leading edge in movement direction
       auto leadingEdge { pengoCenter };
        switch (m_Direction)
        {
        case Direction::Up:    leadingEdge.y -= halfCell; break;
        case Direction::Down:  leadingEdge.y += halfCell; break;
        case Direction::Left:  leadingEdge.x -= halfCell; break;
        case Direction::Right: leadingEdge.x += halfCell; break;
        default: break;
        }

       auto currentCell { m_GridRPtr->WorldToCell(pengoCenter) };
       auto targetCell { m_GridRPtr->WorldToCell(leadingEdge) };

        auto ahead { m_GridRPtr->GetCellType(targetCell) };
        bool blocked { (ahead != CellType::Empty &&
                        ahead != CellType::PlayerSpawn &&
                        ahead != CellType::EnemySpawn) };

        if (blocked)
        {
            // snap to current cell on the perpendicular axis
            auto cellTopLeft { m_GridRPtr->CellToWorld(currentCell)};
            auto currentPos  { gO->GetWorldPosition()};

            if (m_Direction == Direction::Left || m_Direction == Direction::Right)
                gO->SetPosition(currentPos.x, cellTopLeft.y);
            else
                gO->SetPosition(cellTopLeft.x, currentPos.y);

            ctx.SetState(std::make_unique<PengoPushingState>(
                m_Player, 
                m_Direction, 
                m_GridRPtr->GetBlockComponentAtCell(targetCell))
            );
            return;
        }

        if (m_MoveCommand)
            m_MoveCommand->Execute(deltaTime);

        // align perpendicular axis to avoid drifting into side blocks
        auto cellTopLeft{ m_GridRPtr->CellToWorld(currentCell) };
        auto currentPos { gO->GetWorldPosition()};

        if (m_Direction == Direction::Left || m_Direction == Direction::Right)
            gO->SetPosition(currentPos.x, cellTopLeft.y);
        else
            gO->SetPosition(cellTopLeft.x, currentPos.y);

        return;
    }

    // fallback if no grid
    if (m_MoveCommand)
        m_MoveCommand->Execute(deltaTime);
}

