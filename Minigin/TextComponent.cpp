#include <stdexcept>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
	: RenderComponent(owner), m_needsUpdate(true), m_text(text), m_color(color), m_font(std::move(font))
{ }

void dae::TextComponent::Update(float /*deltaTime*/)
{
	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);
		m_TextTextureUPtr = std::make_unique<Texture2D>(texture);
		m_TextureRPtr = m_TextTextureUPtr.get(); // Now we give the raw pointer to the texture that will actually get rendered
		m_needsUpdate = false;
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color) 
{ 
	m_color = color; 
	m_needsUpdate = true; 
}


