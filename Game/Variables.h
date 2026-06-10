#pragma once
#include <vector>
#include <glm/vec2.hpp>

namespace dae
{
    enum class Direction
    {
        Down,
        Left,
        Up,
        Right,
        Static
    };

    enum class CellType { Empty, IceBlock, DiamondBlock, HorizontalWall, VerticalWall, PlayerSpawn, EnemySpawn, EggBlock };
    struct PengoCell
    {
        CellType type{ CellType::Empty };
        glm::vec2 position{};
    };

    using SpawnData = std::vector<PengoCell>;
}