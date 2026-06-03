#pragma once
#include "PengoState.h"
#include "../Variables.h"

namespace dae
{
    class PengoPushingState : public PengoState
    {
    public:
        PengoPushingState(int player, Direction direction) : PengoState{ player }, m_Direction{direction} {}
        void OnEnter(PengoStateComponent& ctx) override;
        void OnExit(PengoStateComponent& ctx)  override;
    private:
        Direction m_Direction{};
    };
}