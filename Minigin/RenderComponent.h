#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include <string>
#include <SDL3/SDL_rect.h>

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

		void SetTexture(const std::string& filename);
		void EnableSrcRect(SDL_Rect newSrcRect);
		void SetSrcRect(SDL_Rect newSrcRect) { m_SourceRect = newSrcRect; }
		void DisableSrcRect() { m_UsingSrcRect = false; }
	protected:
		Texture2D* m_TextureRPtr{};
		Transform m_Transform{};

		bool m_UsingSrcRect{};
		SDL_Rect m_SourceRect{};
	};
}
