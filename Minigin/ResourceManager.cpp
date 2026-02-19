#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

namespace fs = std::filesystem;

void dae::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (!TTF_Init())
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

dae::Texture2D* dae::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath { m_dataPath / file };
	const auto filename { fs::path(fullPath).filename().string() };

	const auto& target { m_loadedTextures.find(filename) };

	if (target == m_loadedTextures.end()) // If we haven't loaded this texture before
	{
		auto texture { std::make_unique<Texture2D>(fullPath.string()) };
		auto tRPtr   { texture.get() };

		m_loadedTextures.insert({ filename, std::move(texture) });

		return tRPtr;
	}
	return target->second.get();
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if(m_loadedFonts.find(key) == m_loadedFonts.end())
		m_loadedFonts.insert(std::pair(key,std::make_shared<Font>(fullPath.string(), size)));
	return m_loadedFonts.at(key);
}

dae::ResourceManager::~ResourceManager() = default;

void dae::ResourceManager::UnloadUnusedResources()
{
	for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedFonts.erase(it);
		else
			++it;
	}
}
