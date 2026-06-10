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

void dae::AnimatorComponent::PlayAnimation(int startCol, int startRow, int frameWidth, int frameHeight, int frameCount, float frameDuration, bool loop)
{
    // Don't restart if already playing this animation
    if (m_StartCol == startCol && m_StartRow == startRow && m_FrameCount == frameCount)
        return;
    m_StartCol = startCol;
    m_StartRow = startRow;
    m_FrameCount = frameCount;
    m_FrameDuration = frameDuration;
    m_Loop = loop;
    m_CurrentFrame = 0;
    m_AccumulatedTime = 0.f;

    m_SrcRect.w = frameWidth;
    m_SrcRect.h = frameHeight;
    m_SrcRect.x = (m_StartCol + m_CurrentFrame) * m_SrcRect.w;
    m_SrcRect.y = m_StartRow * m_SrcRect.h;
    
    m_RenderComponent->EnableSrcRect(m_SrcRect);
}

void dae::AnimatorComponent::Update(float deltaTime)
{
    if (m_FrameCount <= 1) return;

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
                m_CurrentFrame = m_FrameCount - 1; // freeze on last frame
        }

        if (m_RowAnimation)
            m_SrcRect.y = (m_StartRow + m_CurrentFrame) * m_SrcRect.h;
        else
            m_SrcRect.x = (m_StartCol + m_CurrentFrame) * m_SrcRect.w;

        m_RenderComponent->SetSrcRect(m_SrcRect);
    }
}