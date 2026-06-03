#include "AnimatorComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include <iostream>

dae::AnimatorComponent::AnimatorComponent(GameObject* owner)
    : BaseComponent(owner)
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
    std::cout << m_SrcRect.y << " " << m_SrcRect.x << std::endl;
    
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
                m_CurrentFrame = m_FrameCount; // freeze on last frame
        }

        m_SrcRect.x = (m_StartCol + m_CurrentFrame) * m_SrcRect.w;

        auto rect{ m_SrcRect };
        rect.w -= 1;

        m_RenderComponent->SetSrcRect(rect);
    }
}