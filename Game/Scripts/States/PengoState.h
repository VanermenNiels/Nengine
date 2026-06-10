#pragma once
#include "State.h"
namespace dae
{
	class GameObject;
	class PengoState : public State
    {
    public:
        PengoState(int player = 0);
        virtual ~PengoState() = default;

        virtual void OnEnter(StateComponent& ctx, GameObject* gO) {};
        virtual void OnExit(StateComponent& ctx, GameObject* gO) {};
        virtual void Update(StateComponent& ctx, GameObject* gO, float deltaTime) {};

        // To avoid a dynamic cast later on to check if the current state is pushing.
        virtual bool IsPushing() const { return false; }
    protected:

        int m_StartCol{};
		int m_StartRow{};

		int m_FrameWidth{32};
		int m_FrameHeight{32};

		int m_FrameCount{};
		float m_FrameDuration{};

		// will be used to determine which animation to play for the player, since player 1 and player 2 have different sprites
        int m_Player{};

        static const int TOTAL_ROWS_PER_PLAYER { 4 };
    };
}