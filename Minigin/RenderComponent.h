#pragma once
#include <memory>

#include "BaseComponent.h"
#include "Transform.h"
#include <string>

namespace dae
{
	class Texture2D;
	class RenderComponent : public BaseComponent
	{
	public:
		RenderComponent(dae::GameObject* owner);
		virtual ~RenderComponent() = default;

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		void Render() const override;

		void SetPosition(float x, float y);
		void SetTexture(const std::string& filename);
	protected:
		Texture2D* m_TextureRPtr{};
		Transform m_Transform{};
	};
}
