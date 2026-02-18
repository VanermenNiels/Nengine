#pragma once
#include <string>
#include <memory>
#include "Transform.h"

class BaseComponent;
namespace dae
{
	class Texture2D;
	class GameObject 
	{
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename ComponentType, typename... Args>
		ComponentType* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<BaseComponent, ComponentType>, "ComponentType must derive from BaseComponent");

			auto newComponent { std::make_unique<ComponentType>(this, std::forward<Args>(args)...) };
			auto newComponentRawPtr { newComponent.get() };
			m_pComponents.push_back(std::move(newComponent));

			return newComponentRawPtr;
		}

		template<typename ComponentType>
		ComponentType* GetComponent() const
		{
			static_assert(std::is_base_of_v<BaseComponent, ComponentType>, "ComponentType must derive from BaseComponent");

			for (const auto& component : m_Components)
			{
				if (auto castedComponent { dynamic_cast<T*>(component.get()) } )
					return casted;
			}

			return nullptr;


		}

	private:
		std::vector<std::unique_ptr<BaseComponent*>> m_pComponents{};
	};
}
