#pragma once
#include <vector>
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

    protected:
        void EventReaction(Event event) override;

    private:
        std::vector<EnemyStateComponent*> m_EnemiesOnFieldVec{};
        std::vector<std::pair<EnemyStateComponent*, BlockComponent*>> m_EnemiesInEggVec{};

        PengoGridComponent* m_GridRPtr{};
        int m_EnemiesOnField{};
        int m_EnemiesInEgg{};

        EnemyStateComponent* m_PendingSpawnEnemy{ nullptr };
        BlockComponent* m_PendingDestroyBlock{ nullptr };
    };
}