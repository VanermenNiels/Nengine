#pragma once
#include "BaseComponent.h"
#include <string>
#include <SDL3/SDL_rect.h>

namespace dae
{
    class RenderComponent;

    class AnimatorComponent : public BaseComponent
    {
    public:
        AnimatorComponent(GameObject* owner, bool rowAnimation = false);
        ~AnimatorComponent() override = default;

        void Update(float deltaTime) override;

        void SetSpriteSheet(const std::string& filename, int frameWidth, int frameHeight);
        void PlayAnimation(int startCol, int startRow, int frameWidth, int frameHeight, int frameCount, float frameDuration, 
                            bool loop = true, bool freezeLastFrame = true, int amountOfTimesToPlay = 1);

        void SetRowAnimation(bool newRowAnimation) { m_RowAnimation = newRowAnimation; }

        bool AnimationFinished() const;

    private:
        RenderComponent* m_RenderComponent{};

        SDL_Rect m_SrcRect{};

        int   m_StartCol{};
        int   m_StartRow{};
        int   m_FrameCount{};
        float m_FrameDuration{};
        bool  m_Loop{};
        bool  m_FreezeLastFrame{};

        int   m_TimesPlayed{};
        int   m_AmountOfTimesToPlay{};

        int   m_CurrentFrame{};
        float m_AccumulatedTime{};

        bool m_RowAnimation;
    };
}