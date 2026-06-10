#pragma once
#include "PengoState.h"

namespace dae
{
    class PengoIdleState : public PengoState
    {
    public:
        using PengoState::PengoState;
        void OnEnter(StateComponent& ctx, GameObject* gO) override;
    };
}