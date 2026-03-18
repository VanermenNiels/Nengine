#pragma once
#include <vector>
#include "Observers/Observer.h"

namespace dae
{
    class Subject
    {
    public:
        void AddObserver(Observer* observer);

        void RemoveObserver(Observer* observer);

        void Notify(GameObject* entity, Event event);

    private:
        std::vector<Observer*> m_Observers{};
    };
}