#pragma once
#include "Components/ObserverComponent.h"
#include "PengoState.h"
#include "../Variables.h"
#include <memory>
#include <set>
namespace dae
{
    class AnimatorComponent;

    class PengoStateComponent final : public ObserverComponent
    {
    public:
        PengoStateComponent(GameObject* owner, std::vector<EventId> eventIds = {}, int player = 0);
        ~PengoStateComponent() override = default;

        void Update(float deltaTime) override;
        void SetState(std::unique_ptr<PengoState> newState, bool fireOnExit = true);

        AnimatorComponent* GetAnimatorComp();
    protected:
        void EventReaction(Event event) override;
    private:
        std::set<dae::Direction> m_ActiveDirections;
        std::unique_ptr<PengoState> m_CurrentState{};
        int m_Player{};
    };
}