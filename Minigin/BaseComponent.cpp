#include "BaseComponent.h"
#include "GameObject.h"

dae::BaseComponent::BaseComponent(GameObject* pOwner):
	m_OwnerRPtr{ pOwner }
{
}

void dae::BaseComponent::Update(float)
{
}

void dae::BaseComponent::Render() const
{
}