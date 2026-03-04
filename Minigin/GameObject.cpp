#include <string>
#include "GameObject.h"
#include "RenderComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime) 
{
	// Update components
	for (const auto& component : m_ComponentsUPtrVec)
	{
		if (component->IsEnabled()) 
			component->Update(deltaTime);
	}

	for (const auto child : m_ChildrenRPtrVec)
	{
		child->Update(deltaTime);
	}
		
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_ComponentsUPtrVec)
	{
		if (component->IsEnabled())
			component->Render();
	}

	for (const auto child : m_ChildrenRPtrVec)
	{
		child->Render();
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

void dae::GameObject::AddChild(GameObject* child)
{
	if (!child || child == this || IsChildOf(child))
		return;

	if (child->m_ParentRPtr)
		child->m_ParentRPtr->RemoveChild(child);

	child->m_ParentRPtr = this;
	m_ChildrenRPtrVec.push_back(child);

	child->SetPositionDirty();
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	if (!child) return;

	auto it { std::remove(m_ChildrenRPtrVec.begin(), m_ChildrenRPtrVec.end(), child) };
	m_ChildrenRPtrVec.erase(it, m_ChildrenRPtrVec.end());

	child->m_ParentRPtr = nullptr;
	child->SetPositionDirty();
}

void dae::GameObject::MarkForDeletion()
{
	m_MarkedForDeletion = true;

	for (const auto child : m_ChildrenRPtrVec)
		child->MarkForDeletion();
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPos)
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

	if (m_ParentRPtr) m_ParentRPtr->RemoveChild(this);
	m_ParentRPtr = parent;
	if (m_ParentRPtr) m_ParentRPtr->AddChild(this);
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
	for (auto child : m_ChildrenRPtrVec)
		child->SetPositionDirty();
}

bool dae::GameObject::IsChildOf(const GameObject* potentialParent) const
{
	if (!potentialParent) return false;

	for (auto child : m_ChildrenRPtrVec)
	{
		if (child == potentialParent || child->IsChildOf(potentialParent))
			return true;
	}
	return false;
}

