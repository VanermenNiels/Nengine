#pragma once
#include "Components/ObserverComponent.h"

namespace dae
{
    class PengoGridComponent;
    class HealthComponent;
    class EnemyManager;
    class LevelManager final : public ObserverComponent
    {
    public:
        enum class Modes
        {
            SinglePlayer, Multiplayer
        };
        using ObserverComponent::ObserverComponent;
        virtual ~LevelManager() = default;

        void StartSinglePlayerLevel();
        void StartMultiplayerLevel();
        void Update(float deltaTime) override;

    protected:
        void EventReaction(Event event) override;

    private:
        bool m_PendingNextLevel{};
        int m_CurrentLevel{};
        PengoGridComponent* m_GridCompRPtr{};
        std::vector<HealthComponent*> m_PlayerHealths{};
        EnemyManager* m_EnemyManager{};
        Modes m_Mode{};
        bool m_Started{ false };
    };
}