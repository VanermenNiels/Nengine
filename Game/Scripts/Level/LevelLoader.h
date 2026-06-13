#pragma once
#include <string>
#include "../Variables.h"

namespace dae
{
    class PengoGridComponent;
    class LevelLoader
    {
    public:
        static void GenerateAndSaveLevels(const std::string& filename, int levelCount, PengoGridComponent* grid);
        static SpawnData LoadLevel(const std::string& filename, int levelIndex, PengoGridComponent* grid);

    };
}