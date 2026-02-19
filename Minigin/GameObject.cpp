#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime) 
{
	// Update components
	for (const auto& component : m_ComponentsUPtrVec)
		if (component->IsEnabled()) component->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_ComponentsUPtrVec)
		if (component->IsEnabled()) component->Render();
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);

	for (const auto& component : m_ComponentsUPtrVec)
	{
		if (auto renderComp { dynamic_cast<RenderComponent*>(component.get()) })
			renderComp->SetPosition(x, y);
	}
}

