#include "PengoGridComponent.h"
#include "GameObject.h"
#include <array>

dae::PengoGridComponent::PengoGridComponent(GameObject* pOwner, float cellSize, std::vector<GameObject*> players, std::vector<GameObject*> enemies):
	GridComponent(pOwner, 17, 15, cellSize), // Pengo arcade game has fixed playing field size of 15 rows and 17 columns
	m_PlayersVecRPtrs{ std::move(players) }, 
	m_EnemiesVecRPtrs{ std::move(enemies) } 
{}

std::vector<dae::GridComponent::Cell> dae::PengoGridComponent::GetPlayerCells()
{
	std::vector<Cell> playerCells{};
	playerCells.reserve(m_PlayersVecRPtrs.size());
	for (const auto& player : m_PlayersVecRPtrs)
	{
		if (player->GetMarkedForDeletion()) continue;
		playerCells.emplace_back(WorldToCell(player->GetWorldPosition()));
	}

	return playerCells;
}

std::vector<dae::GridComponent::Cell> dae::PengoGridComponent::GetEnemyCells()
{
	std::vector<Cell> enemyCells{};
	enemyCells.reserve(m_EnemiesVecRPtrs.size());
	for (const auto& enemy : m_EnemiesVecRPtrs)
	{
		if (enemy->GetMarkedForDeletion()) continue;
		enemyCells.emplace_back(WorldToCell(enemy->GetWorldPosition()));
	}

	return enemyCells;
}

dae::SpawnData dae::PengoGridComponent::GenerateLayout()
{
    m_CarvingSequence.clear();

    // 1. initialize grid with ice blocks
    m_Grid.assign(m_Rows, std::vector<CellType>(m_Cols, CellType::IceBlock));

    // 2. set outer border as walls - vertical check first so corners are vertical
    for (int row = 0; row < m_Rows; ++row)
        for (int col = 0; col < m_Cols; ++col)
        {
            if (col == 0 || col == m_Cols - 1)
                m_Grid[row][col] = CellType::VerticalWall;
            else if (row == 0 || row == m_Rows - 1)
                m_Grid[row][col] = CellType::HorizontalWall;
        }

    // 3. carve open spaces - maze-like carving for tight paths
    std::vector<Cell> candidates;
    std::uniform_int_distribution<int> rowDist(1, m_Rows - 2);
    std::uniform_int_distribution<int> colDist(1, m_Cols - 2);
    candidates.push_back({ rowDist(m_Rng), colDist(m_Rng) });

    int openCount{};
    constexpr int TARGET_OPEN { 111 };

    while (openCount < TARGET_OPEN && !candidates.empty())
    {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(candidates.size()) - 1);
        int idx { dist(m_Rng) };
        Cell cell = candidates[idx];
        candidates.erase(candidates.begin() + idx);

        if (m_Grid[cell.row][cell.col] != CellType::IceBlock) continue;

        // count how many neighbors are already empty
        int emptyNeighbors{};
        for (auto& neighbor : GetNeighbors(cell))
        {
            if (m_Grid[neighbor.row][neighbor.col] == CellType::Empty)
                ++emptyNeighbors;
        }

        // only carve if at most 1 empty neighbor - keeps paths narrow
        if (emptyNeighbors > 1) continue;

        m_Grid[cell.row][cell.col] = CellType::Empty;
        m_CarvingSequence.push_back(CellToWorld(cell));
        ++openCount;

        for (auto& neighbor : GetNeighbors(cell))
            if (m_Grid[neighbor.row][neighbor.col] == CellType::IceBlock)
                candidates.push_back(neighbor);
    }

    // 4. place diamonds and egg blocks on random ice block cells
    std::vector<Cell> iceBlockCells;
    for (int row{1}; row < m_Rows - 1; ++row)
        for (int col{1}; col < m_Cols - 1; ++col)
            if (m_Grid[row][col] == CellType::IceBlock)
                iceBlockCells.push_back({ row, col });

    std::shuffle(iceBlockCells.begin(), iceBlockCells.end(), m_Rng);

    for (int i{}; i < 3 && i < static_cast<int>(iceBlockCells.size()); ++i)
    {
        m_Grid[iceBlockCells[i].row][iceBlockCells[i].col] = CellType::DiamondBlock;
    }

    for (int i {3}; i < 6 && i < static_cast<int>(iceBlockCells.size()); ++i)
    {
        m_Grid[iceBlockCells[i].row][iceBlockCells[i].col] = CellType::EggBlock;
    }

    // 5. place player and enemy spawns on random empty cells
    std::vector<Cell> emptyCells;
    for (int row{ 1 }; row < m_Rows - 1; ++row)
    {
        for (int col{ 1 }; col < m_Cols - 1; ++col)
        {
            if (m_Grid[row][col] == CellType::Empty)
                emptyCells.push_back({ row, col });
        }
    }

    std::shuffle(emptyCells.begin(), emptyCells.end(), m_Rng);

    for (int i{}; i < 2 && !emptyCells.empty(); ++i)
    {
        m_Grid[emptyCells[0].row][emptyCells[0].col] = CellType::PlayerSpawn;
        emptyCells.erase(emptyCells.begin());
    }

    for (int i{}; i < 3 && !emptyCells.empty(); ++i)
    {
        m_Grid[emptyCells[0].row][emptyCells[0].col] = CellType::EnemySpawn;
        emptyCells.erase(emptyCells.begin());
    }

    // 6. build SpawnData from final grid
    SpawnData spawnData;
    for (int row{}; row < m_Rows; ++row)
        for (int col{}; col < m_Cols; ++col)
        {
            CellType type = m_Grid[row][col];
            if (type != CellType::Empty)
                spawnData.push_back({ type, CellToWorld({row, col}) });
        }

    return spawnData;
}

std::vector<dae::GridComponent::Cell> dae::PengoGridComponent::GetNeighbors(Cell cell) const
{
    std::vector<Cell> neighbors{};
    //Made sure they match the "Direction" enum for easy indexing
    constexpr std::array<Cell, 4> directions{
        Cell{ 1, 0},   // Down  (row+1)
        Cell{ 0,-1},   // Left  (col-1)
        Cell{-1, 0},   // Up    (row-1)
        Cell{ 0, 1},   // Right (col+1)
    };

    for (auto& dir : directions)
    {
        Cell neighbor { cell.row + dir.row, cell.col + dir.col };

        if (IsInBounds(neighbor))
            neighbors.push_back(neighbor);
    }
    return neighbors;
}

void dae::PengoGridComponent::SetCellType(Cell oldCell, Cell newCell, CellType type)
{
    if (IsInBounds(newCell))
    {
		m_Grid[oldCell.row][oldCell.col] = CellType::Empty;
        m_Grid[newCell.row][newCell.col] = type;
        for (auto& [compCell, component] : m_BlockComponents)
        {
            if (compCell.row == oldCell.row && compCell.col == oldCell.col)
                compCell = newCell;

        }
    }
}

void dae::PengoGridComponent::UpdatePlayerCell(StateComponent* player, Cell newCell)
{
	for (auto& [compCell, component] : m_PlayerStateComponents)
	{
		if (component == player)
		{
			compCell = newCell;
			break;
		}
	}
}

dae::BlockComponent* dae::PengoGridComponent::GetBlockComponentAtCell(Cell cell) const
{
    for (const auto& [compCell, component] : m_BlockComponents)
    {
        if (compCell.row == cell.row && compCell.col == cell.col)
            return component;
    }
    return nullptr;
}

dae::StateComponent* dae::PengoGridComponent::GetStateComponentAtCell(Cell cell) const
{
    for (const auto& [compCell, component] : m_PlayerStateComponents)
    {
        if (compCell.row == cell.row && compCell.col == cell.col)
            return component;
    }
    return nullptr;
}

void dae::PengoGridComponent::RemoveBlockComponent(GridComponent::Cell cell)
{
    m_Grid[cell.row][cell.col] = CellType::Empty;
    std::erase_if(m_BlockComponents, [&cell](const auto& pair)
        {
            return pair.first.row == cell.row && pair.first.col == cell.col;
        });
}
