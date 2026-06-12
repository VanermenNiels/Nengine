#pragma once
#include "BlockComponent.h"
#include "GameObject.h"
#include "PengoGridComponent.h"
#include "AnimatorComponent.h"
#include "HitboxComponent.h"
#include "../EventIDs.h"
#include "EnemyStateComponent.h"
#include "States/Enemies/EnemyPushedState.h"
#include "States/Enemies/EnemySquishedState.h"

const float dae::BlockComponent::SPEED { 400.f };

void dae::BlockComponent::Update(float deltaTime)
{
	if (m_AnimatorRPtr)
	{
		if (m_AnimatorRPtr->AnimationFinished())
		{
            if (m_Type == CellType::EggBlock)
            {
                m_Subject.Notify(GetOwner(), Event(EventIDs::EggDestroyed));
                m_AnimatorRPtr->PlayAnimation(0, 5, 32, 32, 3, 0.2f, false);
                m_Type = CellType::EnemySpawn; //make sure we don't enter this if statement again.
            }
            else m_AnimatorRPtr->SetEnabled(false);
			if (!m_AnimatorRPtr->IsEnabled()) GetOwner()->MarkForDeletion();
		}
		return;
	}
    if (!m_MoveCommand) return;

    float halfCell { m_GridRPtr->GetCellSize() / 2.f };
    glm::vec2 blockCenter = {
        GetOwner()->GetWorldPosition().x + halfCell,
        GetOwner()->GetWorldPosition().y + halfCell
    };

    // leading edge in movement direction
    auto leadingEdge{ blockCenter };
    float fullCell{ m_GridRPtr->GetCellSize() };
    switch (m_CurrentDir)
    {
    case Direction::Down:  leadingEdge.y += fullCell; break;
    case Direction::Left:  leadingEdge.x -= fullCell; break;
    case Direction::Up:    leadingEdge.y -= fullCell; break;
    case Direction::Right: leadingEdge.x += fullCell; break;
    default: break;
    }

    GridComponent::Cell currentCell { m_GridRPtr->WorldToCell(blockCenter) };
    GridComponent::Cell targetCell  { m_GridRPtr->WorldToCell(leadingEdge) };

    auto ahead {m_GridRPtr->GetCellType(targetCell)};

    bool blocked { (ahead == CellType::IceBlock ||
                    ahead == CellType::DiamondBlock ||
                    ahead == CellType::EggBlock ||
                    ahead == CellType::HorizontalWall ||
                    ahead == CellType::VerticalWall) };

    if (blocked)
    {
        // snap to current cell and stop
        auto cellTopLeft { m_GridRPtr->CellToWorld(currentCell) };
        GetOwner()->SetPosition(cellTopLeft.x, cellTopLeft.y);

        // make sure grid reflects final position
        m_GridRPtr->SetCellType(m_CurrentCell, currentCell, m_Type);
        m_CurrentCell = currentCell;

        if (m_CurrentGrabbedEnemy)
            m_CurrentGrabbedEnemy->SetState(std::make_unique<EnemySquishedState>(m_CurrentDir));
        m_MoveCommand.reset();
        m_EnemyStateComponents.clear();
        m_EnemyHitboxes.clear();
		m_CurrentDir = Direction::Static;
        return;
    }
    else
    {
        m_MoveCommand->Execute(deltaTime);

        for (int h{}; h < m_EnemyHitboxes.size(); ++h)
        {
            if (m_EnemyHitboxes[h]->Overlaps(m_Hitbox))
            {
                m_EnemyStateComponents[h]->SetBeingPushed();

                auto pos{ m_GridRPtr->CellToWorld(currentCell) };
                switch (m_CurrentDir)
                {
                case Direction::Down:
                    pos.y += fullCell;
                    break;
                case Direction::Left:
                    pos.x -= fullCell;
                    break;
                case Direction::Up:
                    pos.y -= fullCell;
                    break;
                case Direction::Right:
                    pos.x += fullCell;
                    break;
                default: break;

                }
                m_CurrentGrabbedEnemy = m_EnemyStateComponents[h];
                m_CurrentGrabbedEnemy->SetState(std::make_unique<EnemyPushedState>(pos, m_CurrentDir, SPEED, m_GridRPtr));
            }

        }
        // keep grid in sync as block crosses cell boundaries
        if (currentCell.row != m_CurrentCell.row || currentCell.col != m_CurrentCell.col)
        {
            m_GridRPtr->SetCellType(m_CurrentCell, currentCell, m_Type);
            m_CurrentCell = currentCell;
        }
    }
}

void dae::BlockComponent::Push(Direction dir)
{
	if (m_AnimatorRPtr) return; // can't push if currently breaking

    m_CurrentDir = dir;

    constexpr GridComponent::Cell dirOffsets[] = {
        { 1,  0},  // Down
        { 0, -1},  // Left
        {-1,  0},  // Up
        { 0,  1},  // Right
        { 0,  0},  // Static
    };

    auto offset = dirOffsets[static_cast<int>(dir)];
    GridComponent::Cell targetCell{ m_CurrentCell.row + offset.row, m_CurrentCell.col + offset.col };

    auto blockAhead = m_GridRPtr->GetCellType(targetCell);
    if ((blockAhead == CellType::IceBlock ||
         blockAhead == CellType::DiamondBlock ||
         blockAhead == CellType::EggBlock ||
         blockAhead == CellType::HorizontalWall ||
         blockAhead == CellType::VerticalWall) &&
         m_Type != CellType::DiamondBlock)
    {
        Destroy();
        return;
    }

    m_Hitbox = GetOwner()->GetComponent<HitboxComponent>();
    for (auto enemyGO : m_GridRPtr->GetEnemies())
    {
        auto stateComp{ enemyGO->GetComponent<EnemyStateComponent>() };
        if (stateComp->BeingPushed()) continue;

        m_EnemyStateComponents.push_back(stateComp);
        m_EnemyHitboxes.push_back(enemyGO->GetComponent<HitboxComponent>());
    }
    constexpr glm::vec3 dirVectors[] = {
        { 0,  1, 0 },  // Down
        { -1, 0, 0 },  // Left
        { 0, -1, 0 },  // Up
        { 1,  0, 0 },  // Right
        { 0,  0, 0 },  // Static
    };

    m_MoveCommand = std::make_unique<MoveCommand>(
        GetOwner(),
        dirVectors[static_cast<int>(dir)],
        SPEED
    );
}

void dae::BlockComponent::Destroy()
{
    m_AnimatorRPtr = GetOwner()->GetComponent<AnimatorComponent>();
    m_AnimatorRPtr->PlayAnimation(0, 3, 32, 32, 8, 0.1f, false);
    m_GridRPtr->RemoveBlockComponent(m_CurrentCell);
}
