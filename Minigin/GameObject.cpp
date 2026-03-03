#include "GameObject.h"
#include "RenderComponent.h"
#include <string>

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime) 
{
	// Update components
	for (const auto& component : m_ComponentsUPtrVec)
	{
		if (component->IsEnabled()) 
			component->Update(deltaTime);
	}
		
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_ComponentsUPtrVec)
	{
		if (component->IsEnabled())
			component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	SetLocalPosition({ x, y, 0.f });
}

void dae::GameObject::UpdatePosition(float deltaX, float deltaY)
{
	auto pos { m_Transform.GetLocalPosition() };
	pos.x += deltaX;
	pos.y += deltaY;

	m_Transform.SetLocalPosition(pos);
	SetPositionDirty();
}

void dae::GameObject::AddChild(std::unique_ptr<GameObject> child)
{
	if (!child || child.get() == this || IsChildOf(child.get()))
		return;

	if (child->m_ParentRPtr)
		child->m_ParentRPtr->RemoveChild(child.get());

	child->m_ParentRPtr = this;
	m_ChildrenUPtrVec.push_back(std::move(child));

	child->SetPositionDirty();
}

std::unique_ptr<dae::GameObject> dae::GameObject::ReleaseChild(GameObject* child)
{
	auto it = std::find_if(m_ChildrenUPtrVec.begin(),
						   m_ChildrenUPtrVec.end(),
						   [child](const std::unique_ptr<GameObject>& ptr)
						   { return ptr.get() == child;});

	if (it == m_ChildrenUPtrVec.end())
		return nullptr;

	std::unique_ptr<GameObject> extracted{ std::move(*it) };
	m_ChildrenUPtrVec.erase(it);
	extracted->m_ParentRPtr = nullptr;
	return extracted;
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	if (!child) return;

	auto it { std::remove(m_ChildrenUPtrVec.begin(), m_ChildrenUPtrVec.end(), child) };
	m_ChildrenUPtrVec.erase(it, m_ChildrenUPtrVec.end());

	child->m_ParentRPtr = nullptr;
	child->SetPositionDirty();
}

void dae::GameObject::SetParent(std::unique_ptr<GameObject> self, GameObject* parent, bool keepWorldPos)
{
	if (IsChildOf(parent) || parent == this || parent == m_ParentRPtr)
		return;
	if (!parent)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPos)
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}

	if (m_ParentRPtr)
	{
		parent->AddChild(std::move(self));
	}
	m_ParentRPtr = parent;
	if (m_ParentRPtr) m_ParentRPtr->AddChild(std::move(self));
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_Transform.SetLocalPosition(pos);
	SetPositionDirty();
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionDirty)
		UpdateWorldPosition();
	return m_Transform.GetWorldPosition();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionDirty)
	{
		if(!m_ParentRPtr)
			m_Transform.SetWorldPosition(m_Transform.GetLocalPosition());
		else
			m_Transform.SetWorldPosition(m_Transform.GetLocalPosition() + m_ParentRPtr->GetWorldPosition());
	}
	m_PositionDirty = false;
}

void dae::GameObject::SetPositionDirty()
{
	m_PositionDirty = true;
	for (const auto& child : m_ChildrenUPtrVec)
	{
		child->SetPositionDirty();
	}
}

bool dae::GameObject::IsChildOf(const GameObject* potentialParent) const
{
	if (!potentialParent) return false;

	for (const auto& child : m_ChildrenUPtrVec)
	{
		if (child.get() == potentialParent || child->IsChildOf(potentialParent))
			return true;
	}
	return false;
}

