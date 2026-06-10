#include "ObserverComponent.h"

namespace dae
{
    class GameObject;
    class State;
    class StateComponent : public ObserverComponent
    {
    public:
        using ObserverComponent::ObserverComponent;
        ~StateComponent() override = default;

        void Update(float deltaTime) override;
        void SetState(std::unique_ptr<State> newState, bool fireOnExit = true);

    protected:
        std::unique_ptr<State> m_CurrentState{};
    };
}