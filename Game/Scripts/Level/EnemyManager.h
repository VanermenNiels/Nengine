#pragma once
#include <vector>
#include <queue>
#include "Components/ObserverComponent.h"

namespace dae
{
    class EnemyStateComponent;
    class BlockComponent;
    class PengoGridComponent;

    class EnemyManager : public ObserverComponent
    {
    public:
        using ObserverComponent::ObserverComponent;

        void Update(float deltaTime) override;

        void AddEnemyOnField(EnemyStateComponent* enemy);
        void AddEnemyInEgg(EnemyStateComponent* enemy, BlockComponent* block);
        void AddGrid(PengoGridComponent* grid) { m_GridRPtr = grid; }

        bool AllEnemiesDead() { return m_EnemiesOnFieldVec.size() <= 0; }

    protected:
        void EventReaction(Event event) override;

    private:
        std::vector<EnemyStateComponent*> m_EnemiesOnFieldVec{};
        std::vector<std::pair<EnemyStateComponent*, BlockComponent*>> m_EnemiesInEggVec{};

        PengoGridComponent* m_GridRPtr{};
        int m_EnemiesOnField{};
        int m_EnemiesInEgg{};

        struct PendingHatch
        {
            EnemyStateComponent* enemy;
            BlockComponent* block;
        };
        std::queue<PendingHatch> m_PendingHatches{};

        static constexpr int MAX_ENEMIES_ON_FIELD{ 3 };
    };
}