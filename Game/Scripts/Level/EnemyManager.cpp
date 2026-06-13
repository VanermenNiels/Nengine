#include "EnemyManager.h"
#include "Components/EnemyStateComponent.h"
#include "../EventIDs.h"
#include "Components/BlockComponent.h"
#include "Components/PengoGridComponent.h"
#include <algorithm>
#include "HitboxComponent.h"

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
    if (m_PendingHatches.empty()) return;

    auto& pending = m_PendingHatches.front();
    auto* enemy = pending.enemy;
    auto* block = pending.block;

    // block destroyed by collision/player — cancel this hatch
    if (block && block->IsDestroyed())
    {
        m_PendingHatches.pop();
        return;
    }

    // wait until block is stationary
    if (block && block->IsMoving() != Direction::Static)
        return;

    m_PendingHatches.pop();

    // remove from egg vec BEFORE calling Destroy
    m_EnemiesInEggVec.erase(
        std::remove_if(m_EnemiesInEggVec.begin(), m_EnemiesInEggVec.end(),
            [enemy](const auto& pair) { return pair.first == enemy; }),
        m_EnemiesInEggVec.end()
    );
    --m_EnemiesInEgg;

    block->SetHatchDestroy();
    block->Destroy(false);

    enemy->SpawnEnemy();
    enemy->GetOwnerGO()->GetComponent<HitboxComponent>()->SetEnabled(true);
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

        // only hatch if under the cap and there are eggs
        if (m_EnemiesOnField < MAX_ENEMIES_ON_FIELD && !m_EnemiesInEggVec.empty())
        {
            for (auto& [enemy, block] : m_EnemiesInEggVec)
            {
                bool alreadyQueued = false;
                auto tempQueue = m_PendingHatches;
                while (!tempQueue.empty())
                {
                    if (tempQueue.front().enemy == enemy) { alreadyQueued = true; break; }
                    tempQueue.pop();
                }
                if (!alreadyQueued)
                {
                    m_PendingHatches.push({ enemy, block });
                    break;
                }
            }
        }
    }

    if (event.id == EventIDs::EggDestroyed)
    {
        auto it = std::find_if(m_EnemiesInEggVec.begin(), m_EnemiesInEggVec.end(),
            [](const auto& pair) { return pair.second->IsDestroyed(); });

        if (it == m_EnemiesInEggVec.end()) return;

        --m_EnemiesInEgg;
        auto [enemy, block] = *it;
        m_EnemiesInEggVec.erase(it);

        // cancel any pending hatch for this enemy
        // rebuild queue without this enemy
        std::queue<PendingHatch> filtered{};
        auto tempQueue = m_PendingHatches;
        while (!tempQueue.empty())
        {
            if (tempQueue.front().enemy != enemy)
                filtered.push(tempQueue.front());
            tempQueue.pop();
        }
        m_PendingHatches = filtered;

        enemy->RemoveGO();
    }
}