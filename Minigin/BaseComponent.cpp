#include "BaseComponent.h"

dae::Component::Component(GameObject* pOwner):
	m_pOwner{ pOwner }
{
}

void dae::Component::Update(float deltaTime)
{
}

void dae::Component::Render() const
{
}
