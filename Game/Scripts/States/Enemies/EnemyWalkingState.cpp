#include "EnemyWalkingState.h"
#include "EnemyStateComponent.h"
#include "PengoGridComponent.h"
#include "Commands/MoveCommand.h"
#include <queue>
#include <unordered_map>
#include "Components/EnemyStateComponent.h"
#include "AnimatorComponent.h"
#include "Components/BlockComponent.h"

dae::EnemyWalkingState::EnemyWalkingState(Direction dir, PengoGridComponent* grid, bool isPlayer) :
    EnemyState(), m_GridRPtr{ grid }, m_IsPlayer{ isPlayer }
{
    m_CurrentDir = dir;
}

void dae::EnemyWalkingState::OnEnter(StateComponent& ctx, GameObject* gO)
{
    m_FrameCount = 2;
    m_FrameDuration = 0.1f;
    m_StartCol = static_cast<int>(m_CurrentDir) * m_FrameCount;
    m_StartRow += 1;

    static_cast<EnemyStateComponent&>(ctx).GetAnimatorComp()->PlayAnimation(
        m_StartCol, m_StartRow, m_FrameWidth,
        m_FrameHeight, m_FrameCount, m_FrameDuration
    );

    if (!m_IsPlayer) AIMoving(ctx, gO);
}

void dae::EnemyWalkingState::Update(StateComponent& ctx, GameObject* gO, float deltaTime)
{
    if (!m_MoveCommand) return;
    glm::vec2 enemyCenter = {
        gO->GetWorldPosition().x + m_GridRPtr->GetCellSize() / 2.f,
        gO->GetWorldPosition().y + m_GridRPtr->GetCellSize() / 2.f
    };

    auto leadingEdge{ enemyCenter };
    float fullCell{ m_GridRPtr->GetCellSize() };
    switch (m_CurrentDir)
    {
    case Direction::Down:  leadingEdge.y += fullCell; break;
    case Direction::Left:  leadingEdge.x -= fullCell; break;
    case Direction::Up:    leadingEdge.y -= fullCell; break;
    case Direction::Right: leadingEdge.x += fullCell; break;
    default: break;
    }

    auto currentCell = m_GridRPtr->WorldToCell(enemyCenter);
    auto targetCell = m_GridRPtr->WorldToCell(leadingEdge);
    auto ahead = m_GridRPtr->GetCellType(targetCell);

    bool blocked = (ahead == CellType::HorizontalWall ||
        ahead == CellType::VerticalWall ||
        ahead == CellType::DiamondBlock ||
        ahead == CellType::EggBlock);

    if (ahead == CellType::IceBlock && !m_HasLineOfSight)
        blocked = true;

    if (blocked)
    {
        auto snap = m_GridRPtr->CellToWorld(currentCell);
        gO->SetPosition(snap.x, snap.y);
        m_MoveCommand = nullptr;
        AIMoving(ctx, gO);
        return;
    }

    if (ahead == CellType::IceBlock)
    {
        if (m_BlocksDestroyed < m_MaxBlocksToDestroy)
        {
            auto* block = m_GridRPtr->GetBlockComponentAtCell(targetCell);
            if (block)
            {
                if (block->IsMoving() == Direction::Static)
                {
                    block->Destroy();
                    ++m_BlocksDestroyed;
                }
            }
        }
        else
        {
            // budget exhausted, treat ice as a wall
            auto snap = m_GridRPtr->CellToWorld(currentCell);
            gO->SetPosition(snap.x, snap.y);
            m_MoveCommand = nullptr;
            AIMoving(ctx, gO);
            return;
        }
    }

    m_MoveCommand->Execute(deltaTime);
}

void dae::EnemyWalkingState::AIMoving(StateComponent& ctx, GameObject* gO)
{
    auto playerCells = m_GridRPtr->GetPlayerCells();
    if (playerCells.empty()) return;

    auto enemyCell = m_GridRPtr->WorldToCell(gO->GetWorldPosition());
    auto playerCell = playerCells[0];

    m_HasLineOfSight = HasLineOfSight(enemyCell, playerCell);

    Direction newDir;
    if (m_HasLineOfSight)
    {
        newDir = GetChaseDirection(enemyCell, playerCell);
    }
    else
    {
        int exits = CountExits(enemyCell);
        std::uniform_real_distribution<float> chance(0.f, 1.f);

        if (exits > 2 && chance(m_Rng) < TURN_CHANCE)
            newDir = GetRandomDirection(enemyCell);
        else if (chance(m_Rng) < RANDOM_MOVE_CHANCE)
            newDir = GetRandomDirection(enemyCell);
        else
            newDir = GetPathfindDirection(enemyCell, playerCell);
    }

    if (newDir != m_CurrentDir)
    {
        m_CurrentDir = newDir;
        m_StartCol = static_cast<int>(m_CurrentDir) * m_FrameCount;
        static_cast<EnemyStateComponent&>(ctx).GetAnimatorComp()->PlayAnimation(
            m_StartCol, m_StartRow, m_FrameWidth,
            m_FrameHeight, m_FrameCount, m_FrameDuration
        );
    }

    constexpr glm::vec3 dirVectors[] = {
        { 0,  1, 0 },  // Down
        {-1,  0, 0 },  // Left
        { 0, -1, 0 },  // Up
        { 1,  0, 0 },  // Right
        { 0,  0, 0 },  // Static
    };

    m_MoveCommand = std::make_unique<MoveCommand>(
        gO,
        dirVectors[static_cast<int>(m_CurrentDir)],
        SPEED
    );
}

int dae::EnemyWalkingState::CountExits(GridComponent::Cell cell)
{
    constexpr GridComponent::Cell offsets[] = {
        { 1,  0}, { 0, -1}, {-1,  0}, { 0,  1}
    };
    int count = 0;
    for (auto& o : offsets)
    {
        auto type = m_GridRPtr->GetCellType({ cell.row + o.row, cell.col + o.col });
        if (type == CellType::Empty) ++count;
    }
    return count;
}

bool dae::EnemyWalkingState::HasLineOfSight(GridComponent::Cell from, GridComponent::Cell to)
{
    if (from.row == to.row)
    {
        int step = (to.col > from.col) ? 1 : -1;
        for (int col = from.col + step; col != to.col; col += step)
            if (m_GridRPtr->GetCellType({ from.row, col }) != CellType::Empty)
                return false;
        return true;
    }
    if (from.col == to.col)
    {
        int step = (to.row > from.row) ? 1 : -1;
        for (int row = from.row + step; row != to.row; row += step)
            if (m_GridRPtr->GetCellType({ row, from.col }) != CellType::Empty)
                return false;
        return true;
    }
    return false;
}

dae::Direction dae::EnemyWalkingState::GetChaseDirection(GridComponent::Cell enemyCell, GridComponent::Cell playerCell)
{
    int rowDiff = playerCell.row - enemyCell.row;
    int colDiff = playerCell.col - enemyCell.col;

    if (std::abs(rowDiff) >= std::abs(colDiff))
        return rowDiff > 0 ? Direction::Down : Direction::Up;
    else
        return colDiff > 0 ? Direction::Right : Direction::Left;
}

dae::Direction dae::EnemyWalkingState::GetPathfindDirection(GridComponent::Cell enemyCell, GridComponent::Cell playerCell)
{
    auto key = [](GridComponent::Cell c) { return c.row * 100 + c.col; };

    std::queue< GridComponent::Cell> open{};
    std::unordered_map<int, GridComponent::Cell> cameFrom{};

    open.push(enemyCell);
    cameFrom[key(enemyCell)] = enemyCell;

    while (!open.empty())
    {
        auto current = open.front(); open.pop();

        if (current.row == playerCell.row && current.col == playerCell.col)
        {
            GridComponent::Cell step = current;
            while (true)
            {
                GridComponent::Cell parent = cameFrom[key(step)];
                if (parent.row == enemyCell.row && parent.col == enemyCell.col) break;
                step = parent;
            }
            int dr = step.row - enemyCell.row;
            int dc = step.col - enemyCell.col;
            if (dr == 1)  return Direction::Down;
            if (dr == -1) return Direction::Up;
            if (dc == 1)  return Direction::Right;
            return Direction::Left;
        }

        for (auto& neighbor : m_GridRPtr->GetNeighbors(current))
        {
            if (cameFrom.count(key(neighbor))) continue;
            auto type = m_GridRPtr->GetCellType(neighbor);
            if (type == CellType::HorizontalWall ||
                type == CellType::VerticalWall ||
                type == CellType::DiamondBlock ||
                type == CellType::EggBlock ||
                type == CellType::IceBlock)   continue;

            cameFrom[key(neighbor)] = current;
            open.push(neighbor);
        }
    }

    return GetRandomDirection(enemyCell);
}

dae::Direction dae::EnemyWalkingState::GetRandomDirection(GridComponent::Cell enemyCell)
{
    constexpr GridComponent::Cell offsets[] = {
        { 1,  0},  // Down
        { 0, -1},  // Left
        {-1,  0},  // Up
        { 0,  1},  // Right
    };

    std::vector<Direction> valid{};
    for (int i = 0; i < 4; ++i)
    {
        GridComponent::Cell neighbor{ enemyCell.row + offsets[i].row, enemyCell.col + offsets[i].col };
        auto type = m_GridRPtr->GetCellType(neighbor);
        if (type == CellType::HorizontalWall ||
            type == CellType::VerticalWall ||
            type == CellType::DiamondBlock ||
            type == CellType::EggBlock ||
            type == CellType::IceBlock)
            continue;

        valid.push_back(static_cast<Direction>(i));
    }

    if (valid.empty())
        return m_CurrentDir;

    std::uniform_int_distribution<size_t> dist(0, valid.size() - 1);
    return valid[dist(m_Rng)];
}