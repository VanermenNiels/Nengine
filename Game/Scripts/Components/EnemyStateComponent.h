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

        void SetBeingPushed() { m_BeingPushed = true; }
        bool BeingPushed() { return m_BeingPushed; }
        void SetDead();
        bool IsDead() { return m_Dead; }
        AnimatorComponent* GetAnimatorComp();
        int GetEnemiesAlive() { return ENEMIES_ON_FIELD + ENEMIES_IN_EGG; }
        bool InEgg() { return m_InEgg; }

        void RemoveGO() { GetOwner()->MarkForDeletion(); }
        GameObject* GetOwnerGO() { return GetOwner(); }
    protected:
        void EventReaction(Event event) override;
    private:
        bool m_Dead{};
        Direction m_CurrentDir{ Direction::Right };
        int m_Player{};
        PengoGridComponent* m_GridRPtr;

        bool m_InEgg;
        bool m_BeingPushed{};

        static int ENEMIES_ON_FIELD;
        static int ENEMIES_IN_EGG;


    };
}