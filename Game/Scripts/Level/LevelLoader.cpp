#include "LevelLoader.h"
#include "PengoGridComponent.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

void dae::LevelLoader::GenerateAndSaveLevels(const std::string& filename, int levelCount, PengoGridComponent* grid)
{
#ifdef __EMSCRIPTEN__
    (void)filename;
    (void)levelCount;
    (void)grid;
    return; // Can't write to filesystem in WASM
#else
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("LevelLoader: could not open file for writing: " + filename);

    for (int i{}; i < levelCount; ++i)
    {
        grid->GenerateLayout();

        file << "LEVEL " << i << "\n";

        const auto& gridData = grid->GetGrid();
        for (const auto& row : gridData)
        {
            for (int col = 0; col < static_cast<int>(row.size()); ++col)
            {
                if (col > 0) file << ",";
                file << static_cast<int>(row[col]);
            }
            file << "\n";
        }
    }
#endif
}

dae::SpawnData dae::LevelLoader::LoadLevel(const std::string& filename, int levelIndex, PengoGridComponent* grid)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("LevelLoader: could not open file for reading: " + filename);

    std::string line;
    int currentLevel = -1;
    std::vector<std::vector<CellType>> loadedGrid;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        if (line.rfind("LEVEL ", 0) == 0)
        {
            currentLevel = std::stoi(line.substr(6));
            if (currentLevel == levelIndex)
                loadedGrid.clear();
            continue;
        }

        if (currentLevel != levelIndex) continue;

        std::vector<CellType> row;
        std::stringstream ss(line);
        std::string val;
        while (std::getline(ss, val, ','))
            if (!val.empty())
                row.push_back(static_cast<CellType>(std::stoi(val)));

        loadedGrid.push_back(row);
    }

    if (loadedGrid.empty())
        throw std::runtime_error("LevelLoader: level not found: " + std::to_string(levelIndex));

    grid->SetGrid(loadedGrid);

    const float halfCell{ grid->GetCellSize() / 2.f };
    const int rows{ static_cast<int>(loadedGrid.size()) };

    SpawnData spawnData;
    for (int row = 0; row < rows; ++row)
    {
        const int cols{ static_cast<int>(loadedGrid[row].size()) };
        for (int col = 0; col < cols; ++col)
        {
            CellType type = loadedGrid[row][col];
            if (type == CellType::Empty) continue;

            glm::vec2 pos{ grid->CellToWorld({row, col}) };

            if (type == CellType::HorizontalWall)
            {
                if (row == 0)        pos.y += halfCell;
                if (row == rows - 1) pos.y -= halfCell;
            }
            else if (type == CellType::VerticalWall)
            {
                if (col == 0)        pos.x += halfCell;
                if (col == cols - 1) pos.x -= halfCell;
            }

            spawnData.push_back({ type, pos });
        }
    }

    return spawnData;
}