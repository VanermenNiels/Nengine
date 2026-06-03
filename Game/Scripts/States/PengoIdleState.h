#pragma once
#include "PengoState.h"

namespace dae
{
    class PengoIdleState : public PengoState
    {
    public:
        using PengoState::PengoState;
        void OnEnter(PengoStateComponent& ctx) override;
    };
}