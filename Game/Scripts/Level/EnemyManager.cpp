#include "EnemyManager.h"
#include "Components/EnemyStateComponent.h"
#include "../EventIDs.h"
#include "Components/BlockComponent.h"
#include "Components/PengoGridComponent.h"
#include <algorithm>

void dae::EnemyManager::AddEnemyOnField(EnemyStateComponent* enemy)
{
    ++m_EnemiesOnField;
    m_EnemiesOnFieldVec.push_back(enemy);
}

void dae::EnemyManager::AddEnemyInEgg(EnemyStateComponent* enemy, BlockComponent* block)
{
    ++m_EnemiesInEgg;
    m_EnemiesInEggVec.push_back(std::pair{ enemy, block });
}

void dae::EnemyManager::Update(float)
{
    if (!m_PendingSpawnEnemy) return;

    auto* enemy = m_PendingSpawnEnemy;
    auto* block = m_PendingDestroyBlock;
    m_PendingSpawnEnemy = nullptr;
    m_PendingDestroyBlock = nullptr;

    // remove from egg vec BEFORE calling Destroy so EggDestroyed handler is a no-op
    m_EnemiesInEggVec.erase(
        std::remove_if(m_EnemiesInEggVec.begin(), m_EnemiesInEggVec.end(),
            [enemy](const auto& pair) { return pair.first == enemy; }),
        m_EnemiesInEggVec.end()
    );
    --m_EnemiesInEgg;

    // play break animation on the block (fires EggDestroyed but vec is empty so handler skips)
    if (block) block->Destroy(false);

    // spawn the enemy
    enemy->SpawnEnemy();
    if (m_GridRPtr) m_GridRPtr->AddEnemyObject(enemy->GetOwnerGO());
    ++m_EnemiesOnField;
    m_EnemiesOnFieldVec.push_back(enemy);
}

void dae::EnemyManager::EventReaction(Event event)
{
    if (event.id == EventIDs::EnemyKilled)
    {
        --m_EnemiesOnField;

        m_EnemiesOnFieldVec.erase(
            std::remove_if(m_EnemiesOnFieldVec.begin(), m_EnemiesOnFieldVec.end(),
                [](EnemyStateComponent* e) { return e->IsDead(); }),
            m_EnemiesOnFieldVec.end()
        );

        if (!m_EnemiesInEggVec.empty())
        {
            m_PendingSpawnEnemy = m_EnemiesInEggVec[0].first;
            m_PendingDestroyBlock = m_EnemiesInEggVec[0].second;
        }
    }

    if (event.id == EventIDs::EggDestroyed)
    {
        // only reached when player directly destroys an egg block (not via hatching)
        // because hatching removes the entry before calling Destroy
        if (m_EnemiesInEggVec.empty()) return;

        --m_EnemiesInEgg;
        for (auto it = m_EnemiesInEggVec.begin(); it != m_EnemiesInEggVec.end(); ++it)
        {
            auto [enemy, block] = *it;
            if (enemy->IsDead())
            {
                enemy->RemoveGO();
            }
            else
            {
                enemy->SpawnEnemy();
                if (m_GridRPtr) m_GridRPtr->AddEnemyObject(enemy->GetOwnerGO());
                ++m_EnemiesOnField;
                m_EnemiesOnFieldVec.push_back(enemy);
            }
            m_EnemiesInEggVec.erase(it);
            break;
        }
    }
}