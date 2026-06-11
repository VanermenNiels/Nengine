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
        EnemyStateComponent(GameObject* owner, PengoGridComponent* grid, std::vector<EventId> eventIds = {});
        ~EnemyStateComponent() override = default;

        AnimatorComponent* GetAnimatorComp();
    protected:
        void EventReaction(Event event) override;
    private:
        Direction m_CurrentDir{ Direction::Static };
        int m_Player{};
        PengoGridComponent* m_GridRPtr;
    };
}