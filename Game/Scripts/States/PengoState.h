#pragma once

namespace dae
{
    class PengoStateComponent;

    class PengoState
    {
    public:
        PengoState(int player = 0);
        virtual ~PengoState() = default;

        virtual void OnEnter(PengoStateComponent& ctx) {};
        virtual void OnExit(PengoStateComponent& ctx) {};
        virtual void Update(PengoStateComponent& ctx, float deltaTime) {};

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