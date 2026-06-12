#include "EnemyManager.h"
#include "Components/EnemyStateComponent.h"
#include "../EventIDs.h"

void dae::EnemyManager::AddEnemy(EnemyStateComponent* enemy)
{
	if (enemy->InEgg())
		++m_EnemiesInEgg;
	else
		++m_EnemiesOnField;

	m_Enemies.push_back(enemy);
}

void dae::EnemyManager::EventReaction(Event event)
{
	if (event.id == EventIDs::EnemyKilled)
	{
		if (m_EnemiesInEgg > 0)
		{
			--m_EnemiesInEgg;
			for (auto enemy : m_Enemies)
			{
				if (enemy->InEgg())
				{
					enemy->SpawnEnemy();
					break;
				}
			}
		}
		else --m_EnemiesOnField;
	}

	if (event.id == EventIDs::EggDestroyed)
	{
		--m_EnemiesInEgg;

	}
}
