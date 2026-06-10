#pragma once
#include "Components/GridComponent.h"
#include <vector>
#include <random>
#include "../Variables.h"
#include <utility>

namespace dae
{
	class BlockComponent;
    class PengoGridComponent final : public GridComponent
    {
    public:
        PengoGridComponent(GameObject* pOwner, float cellSize, std::vector<GameObject*> players, std::vector<GameObject*> enemies);

        PengoGridComponent(const PengoGridComponent& other) = delete;
        PengoGridComponent(PengoGridComponent&& other) noexcept = delete;

        ~PengoGridComponent() = default;

        PengoGridComponent& operator=(const PengoGridComponent& other) = delete;
        PengoGridComponent& operator=(PengoGridComponent&& other) noexcept = delete;

        std::vector<Cell> GetPlayerCells();
        std::vector<Cell> GetEnemyCells();

        SpawnData GenerateLayout();

        std::vector<Cell> GetNeighbors(Cell cell) const;
        const std::vector<std::vector<CellType>>& GetGrid() const { return m_Grid; }
        const std::vector<glm::vec2>& GetCarvingSequence() const { return m_CarvingSequence; }
		CellType GetCellType(Cell cell) const { return IsInBounds(cell) ? m_Grid[cell.row][cell.col] : CellType::HorizontalWall; }

        void SetGrid(const std::vector<std::vector<CellType>>& grid) { m_Grid = grid; }
        void SetCellType(Cell oldCell,Cell newCell, CellType type);
        void AddBlockComponent(Cell cell, BlockComponent* component) { m_BlockComponents.push_back({cell, component}); }

        BlockComponent* GetBlockComponentAtCell(Cell cell) const;

        void RemoveBlockComponent(GridComponent::Cell cell);

    private:
        std::vector<GameObject*> m_PlayersVecRPtrs;
        std::vector<GameObject*> m_EnemiesVecRPtrs;

        std::vector<std::vector<CellType>> m_Grid;

		// C++ standard way better than srand() and rand() for generating random numbers, 
        // and it doesn't have the same issues with randomness quality and global state
		// also thread-safe, so if we ever want to do multithreaded layout generation it won't cause problems
        //https://www.geeksforgeeks.org/cpp/stdmt19937-class-in-cpp/
		std::mt19937 m_Rng{ std::random_device{}() }; // random number generator for layout generation

        std::vector<glm::vec2> m_CarvingSequence;

        std::vector<std::pair<Cell, BlockComponent*>> m_BlockComponents{};
    };
}