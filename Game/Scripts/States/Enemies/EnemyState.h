#pragma once
#include "State.h"
#include "../Variables.h"
namespace dae
{
    class GameObject;
    class EnemyState : public State
    {
    public:
        EnemyState();
        virtual ~EnemyState() = default;

        virtual void OnEnter(StateComponent& ctx, GameObject* gO) {};
        virtual void OnExit(StateComponent& ctx, GameObject* gO) {};
        virtual void Update(StateComponent& ctx, GameObject* gO, float deltaTime) {};

        // To avoid a dynamic cast later on to check if the current state is pushing.
        virtual bool BeingPushed() const { return false; }
    protected:

        int m_StartCol{};
        int m_StartRow{};

        int m_FrameWidth{ 32 };
        int m_FrameHeight{ 32 };

        int m_FrameCount{};
        float m_FrameDuration{};

        Direction m_CurrentDir{};

    };
}