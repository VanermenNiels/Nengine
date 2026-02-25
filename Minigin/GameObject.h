#pragma once
#include <string>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Transform.h"
#include "BaseComponent.h"

namespace dae
{
	class GameObject final
	{
	public:

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float deltaTime);
		void Render() const;

		void SetPosition(float x, float y);
		void UpdatePosition(float deltaX, float deltaY);

		Transform& GetWorldTransForm() { return m_WorldTransform; }

		void MarkForDeletion() { m_MarkedForDeletion = true; }
		bool GetMarkedForDeletion() const { return m_MarkedForDeletion; }

		void SetParent(GameObject* newParent, bool keepWorldPosition = true);
		void SetLocalPosition(const glm::vec3& pos);
		void SetPositionDirty();

		GameObject* GetParent() const { return m_ParentRPtr; }
		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const { return m_LocalTransform.GetPosition(); }

		void UpdateWorldPosition();


		template<typename ComponentType, typename... Args>
		ComponentType* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<BaseComponent, ComponentType>, "ComponentType must derive from BaseComponent");

			auto newComponent  { std::make_unique<ComponentType>(this, std::forward<Args>(args)...) };
			auto pnewComponent { newComponent.get() };

			m_ComponentRPtrUMap[typeid(ComponentType)].push_back(pnewComponent);
			m_ComponentsUPtrVec.push_back(std::move(newComponent));

			return pnewComponent;
		}

		template<typename ComponentType>
		ComponentType* GetComponent() const
		{
			auto component { m_ComponentRPtrUMap.find(typeid(ComponentType)) };

			if (component != m_ComponentRPtrUMap.end() && !component->second.empty())
				return static_cast<ComponentType*>(component->second.front());

			return nullptr;
		}

		template<typename ComponentType>
		std::vector<ComponentType*> GetComponents() const
		{
			std::vector<ComponentType*> matches{};

			auto components{ m_ComponentRPtrUMap.find(typeid(ComponentType)) };
			if (components != m_ComponentRPtrUMap.end()) // If we found components of the requested type
			{
				for (const auto& component : components->second)
					matches.push_back(static_cast<ComponentType*>(component));
			}

			return matches;
		}

		template<typename ComponentType>
		bool FindComponent() const
		{
			auto component{ m_ComponentRPtrUMap.find(typeid(ComponentType)) };

			if (component != m_ComponentRPtrUMap.end() && !component->second.empty())
				return true;

			return false;
		}

		template<typename ComponentType>
		bool RemoveComponent()
		{
			static_assert(std::is_base_of_v<BaseComponent, ComponentType>, "ComponentType must derive from BaseComponent");

			auto targetVec { m_ComponentRPtrUMap.find(typeid(ComponentType)) };
			if (targetVec == m_ComponentRPtrUMap.end() || targetVec->second.empty())
				return false; 

			BaseComponent* componentToRemove { targetVec->second.front() };

			targetVec->second.erase(targetVec->second.begin());
			if (targetVec->second.empty())
				m_ComponentRPtrUMap.erase(targetVec);


			auto target = std::find_if( m_ComponentsUPtrVec.begin(), 
										m_ComponentsUPtrVec.end(),
										[componentToRemove](const std::unique_ptr<BaseComponent>& ptr) { return ptr.get() == componentToRemove; }
			);

			if (target != m_ComponentsUPtrVec.end())
				m_ComponentsUPtrVec.erase(target);

			return true;
		}

		template<typename ComponentType>
		bool RemoveAllComponents()
		{
			auto target { m_ComponentRPtrUMap.find(typeid(ComponentType)) };
			if (target == m_ComponentRPtrUMap.end())
				return false;

			auto isSameType = [typeIndex = typeid(ComponentType)](const std::unique_ptr<BaseComponent>& ptr) 
			{
				return typeid(*ptr) == typeIndex;
			};
			m_ComponentsUPtrVec.erase(std::remove_if( m_ComponentsUPtrVec.begin(), 
													  m_ComponentsUPtrVec.end(),
													  isSameType ),
									  m_ComponentsUPtrVec.end());

			m_ComponentRPtrUMap.erase(target);

			return true;
		}

	private:
		std::vector<std::unique_ptr<BaseComponent>> m_ComponentsUPtrVec{};
		std::unordered_map<std::type_index, std::vector<BaseComponent*>> m_ComponentRPtrUMap{};

		GameObject* m_ParentRPtr{};
		std::vector<GameObject*> m_ChildrenRPtrVec{};

		Transform m_WorldTransform{};
		Transform m_LocalTransform{};

		bool m_PositionDirty{};

		bool m_MarkedForDeletion{};

		bool IsChildOf(const GameObject* potentialParent) const;
		void RemoveChild(GameObject* child);
		void AddChild(GameObject* child);

	};
}
