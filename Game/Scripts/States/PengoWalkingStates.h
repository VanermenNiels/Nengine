#pragma once
#include "PengoState.h"
#include "../Variables.h"

namespace dae
{
    class PengoWalkingState : public PengoState
    {
    public:
        PengoWalkingState(int player, Direction direction) : PengoState(player), m_Direction(direction) {}
        void OnEnter(PengoStateComponent& ctx) override;
        void OnExit(PengoStateComponent& ctx)  override;
    private:
		Direction m_Direction{};
    };
}