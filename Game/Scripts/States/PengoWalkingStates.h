#pragma once
#include "PengoState.h"
#include "../Variables.h"
#include "Commands/MoveCommand.h"
#include <memory>
#include "Components/GridComponent.h"

namespace dae
{
    class PengoGridComponent;
    class PengoWalkingState : public PengoState
    {
    public:
        PengoWalkingState(int player, Direction direction, PengoGridComponent* grid, float speed = 200.f)
            : PengoState(player), m_Direction(direction), m_Speed(speed), m_GridRPtr{ grid } {}

        void OnEnter(StateComponent& ctx, GameObject* gO) override;
        void OnExit(StateComponent& ctx, GameObject* gO) override;
        void Update(StateComponent& ctx, GameObject* gO, float deltaTime) override;

    private:
        Direction m_Direction{};
        float m_Speed{ 200.f };
        std::unique_ptr<MoveCommand> m_MoveCommand{};
		PengoGridComponent* m_GridRPtr{};
		GridComponent::Cell m_CurrentCell{};
    };
}