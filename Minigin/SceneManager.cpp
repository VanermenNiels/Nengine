#include "SceneManager.h"
#include "Scene.h"
#include <stdexcept>

void dae::SceneManager::Update(float deltaTime)
{
	const size_t count = m_scenes.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (m_scenes[i] && m_scenes[i]->IsActive())
			m_scenes[i]->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		if (scene && scene->IsActive())
			scene->Render();
	}
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
	for (auto& scene : m_scenes)
	{
		if (scene && scene->IsActive())
			return *scene;
	}
	throw std::runtime_error("No active scene found");
}

dae::Scene& dae::SceneManager::CreateScene()
{
	for (const auto& scene : m_scenes)
	{
		if (scene && scene->IsActive() && !scene->IsPersistent())
			scene->SetActive(false);
	}
	m_scenes.emplace_back(new Scene());
	m_scenes.back()->SetActive(true);
	return *m_scenes.back();
}
