#include "RenderComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "GameObject.h"

dae::RenderComponent::RenderComponent(dae::GameObject* owner):
	BaseComponent(owner)
{
}

void dae::RenderComponent::Render() const
{
	if (m_TextureRPtr)
	{
		const auto& pos { GetOwner()->GetWorldPosition()};
		Renderer::GetInstance().RenderTexture(*m_TextureRPtr, pos.x, pos.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_TextureRPtr = ResourceManager::GetInstance().LoadTexture(filename);
}
