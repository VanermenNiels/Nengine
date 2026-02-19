#pragma once
#include <string>
#include "RenderComponent.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public RenderComponent
	{
	public:
		void Update(float deltaTime) override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

		TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};
		// We need to manage the texture ourselves since we need to update it when the text or color changes, and we don't want to mess with the ResourceManager for that
		std::unique_ptr<Texture2D> m_TextTextureUPtr{};
	};
}
