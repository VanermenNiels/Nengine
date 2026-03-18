#pragma once
#include "BaseComponent.h"
#include "Events/Subject.h"
#include "Observers/Observer.h"
#include "Events/Event.h"

namespace dae
{
    class ScoreComponent final : public BaseComponent, public Observer
    {
    public:
        ScoreComponent(GameObject* owner, int score = 0);

        void AddScore(int scoreToAdd);
        int GetScore() const { return m_Score; }

        void SetSubject(Subject* subject) { m_SubjectRPtr = subject; }

        // Observer function
        void onNotify(GameObject*, Event event) override;

    private:
        int m_Score{};
        Subject* m_SubjectRPtr{};
    };
}