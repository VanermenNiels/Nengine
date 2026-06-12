#include "AnimatorComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"

dae::AnimatorComponent::AnimatorComponent(GameObject* owner, bool rowAnimation)
    : BaseComponent(owner),
      m_RowAnimation{rowAnimation}
{
    m_RenderComponent = owner->GetComponent<RenderComponent>();
}

void dae::AnimatorComponent::SetSpriteSheet(const std::string& filename, int frameWidth, int frameHeight)
{
    m_SrcRect.w = frameWidth;
    m_SrcRect.h = frameHeight;
    m_RenderComponent->SetTexture(filename);
}

void dae::AnimatorComponent::PlayAnimation(int startCol, int startRow, int frameWidth, int frameHeight, int frameCount, float frameDuration, bool loop, bool freezeLastFrame, int amountOfTimesToPlay)
{
    // Don't restart if already playing this animation
    if (m_StartCol == startCol && m_StartRow == startRow && m_FrameCount == frameCount)
        return;
    m_StartCol = startCol;
    m_StartRow = startRow;
    m_FrameCount = frameCount;
    m_FrameDuration = frameDuration;
    m_Loop = loop;
    m_AmountOfTimesToPlay = amountOfTimesToPlay;
    m_TimesPlayed = 0;
    m_CurrentFrame = 0;
    m_AccumulatedTime = 0.f;
    m_FreezeLastFrame = freezeLastFrame;

    m_SrcRect.w = frameWidth;
    m_SrcRect.h = frameHeight;
    m_SrcRect.x = (m_StartCol + m_CurrentFrame) * m_SrcRect.w;
    m_SrcRect.y = m_StartRow * m_SrcRect.h;
    
    m_RenderComponent->EnableSrcRect(m_SrcRect);
}

bool dae::AnimatorComponent::AnimationFinished() const
{
    if (m_FreezeLastFrame)
        return (!m_Loop && m_CurrentFrame >= m_FrameCount - 1);
    else 
        return (m_CurrentFrame == 0 && m_AmountOfTimesToPlay == m_TimesPlayed);
}

void dae::AnimatorComponent::Update(float deltaTime)
{
    if (m_FrameCount <= 1 || m_TimesPlayed >= m_AmountOfTimesToPlay) return;

    m_AccumulatedTime += deltaTime;

    if (m_AccumulatedTime >= m_FrameDuration)
    {
        m_AccumulatedTime -= m_FrameDuration;
        m_CurrentFrame++;

        if (m_CurrentFrame >= m_FrameCount)
        {
            if (m_Loop)
                m_CurrentFrame = 0;
            else 
            {
                ++m_TimesPlayed;
                if (m_TimesPlayed < m_AmountOfTimesToPlay)
                    m_CurrentFrame = 0;
                else
                    if (m_FreezeLastFrame)
                        m_CurrentFrame = m_FrameCount - 1; // freeze on last frame
                    else m_CurrentFrame = 0;
            }
        }

        if (m_RowAnimation)
            m_SrcRect.y = (m_StartRow + m_CurrentFrame) * m_SrcRect.h;
        else
            m_SrcRect.x = (m_StartCol + m_CurrentFrame) * m_SrcRect.w;

        m_RenderComponent->SetSrcRect(m_SrcRect);
    }
}