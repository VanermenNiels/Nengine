#pragma once
#include "Components/StateComponent.h"
#include "../Variables.h"
namespace dae
{
    class AnimatorComponent;
    class PengoGridComponent;
    class EnemyStateComponent final : public StateComponent
    {
    public:
        EnemyStateComponent(GameObject* owner, PengoGridComponent* grid, bool inEgg, std::vector<EventId> eventIds = {});
        ~EnemyStateComponent();

        void SpawnEnemy();
        void StartMoving();
        void RemoveEnemy(EnemyStateComponent* enemy) {ENEMIES.erase(std::remove(ENEMIES.begin(), ENEMIES.end(), enemy), ENEMIES.end());}

        void SetBeingPushed() { m_BeingPushed = true; }
        bool BeingPushed() { return m_BeingPushed; }

        AnimatorComponent* GetAnimatorComp();
        int GetEnemiesAlive() { return ENEMIES_ON_FIELD + ENEMIES_IN_EGG; }
        bool InEgg() { return m_InEgg; }

    protected:
        void EventReaction(Event event) override;
    private:
        Direction m_CurrentDir{ Direction::Right };
        int m_Player{};
        PengoGridComponent* m_GridRPtr;

        bool m_InEgg;
        bool m_BeingPushed{};

        static int ENEMIES_ON_FIELD;
        static int ENEMIES_IN_EGG;

        static std::vector<EnemyStateComponent*> ENEMIES;

    };
}