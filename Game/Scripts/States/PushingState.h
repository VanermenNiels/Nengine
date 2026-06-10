#pragma once
#include "PengoState.h"
#include "../Variables.h"

namespace dae
{
	class PengoGridComponent;
	class BlockComponent;
    class PengoPushingState : public PengoState
    {
    public:
        PengoPushingState(int player, Direction direction, BlockComponent* blockComp) : 
            PengoState{ player }, m_Direction{direction}, m_BlockComp{blockComp} {}

        void OnEnter(StateComponent& ctx, GameObject* gO) override;
        void OnExit(StateComponent& ctx, GameObject* gO)  override;

        void Push();

        bool IsPushing() const override { return true; }
    private:
        Direction m_Direction{};
		BlockComponent* m_BlockComp;
    };
}