#pragma once
#include "Components/StateComponent.h"
#include "PengoState.h"
#include "../Variables.h"
#include <memory>
namespace dae
{
    class AnimatorComponent;
	class PengoGridComponent;
    class PengoStateComponent final : public StateComponent
    {
    public:
        PengoStateComponent(GameObject* owner, PengoGridComponent* grid, std::vector<EventId> eventIds = {}, int player = 0);
        ~PengoStateComponent() override = default;

        AnimatorComponent* GetAnimatorComp();
    protected:
        void EventReaction(Event event) override;
    private:
        Direction m_CurrentDir {Direction::Static};
        int m_Player{};
        PengoGridComponent* m_GridRPtr;
    };
}