#pragma once
#include "BlockComponent.h"
#include "GameObject.h"
#include "PengoGridComponent.h"
#include "AnimatorComponent.h"

const float dae::BlockComponent::SPEED { 400.f };

void dae::BlockComponent::Update(float deltaTime)
{
	if (m_AnimatorRPtr)
	{
		m_AnimatorRPtr->Update(deltaTime);
		if (m_AnimatorRPtr->AnimationFinished())
		{
			GetOwner()->MarkForDeletion();
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

        m_MoveCommand.reset();
		m_CurrentDir = Direction::Static;
        return;
    }
    else
    {
        m_MoveCommand->Execute(deltaTime);

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
        m_AnimatorRPtr = GetOwner()->GetComponent<AnimatorComponent>();
        m_AnimatorRPtr->PlayAnimation(0, 3, 32, 32, 8, 0.2f, false);
        m_GridRPtr->RemoveBlockComponent(m_CurrentCell);
        return;
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
