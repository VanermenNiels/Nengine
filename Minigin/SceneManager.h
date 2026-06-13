#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();

		void Update(float deltaTime);
		void Render();

		Scene& GetActiveScene();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() { m_scenes.reserve(16); }
		std::vector<std::unique_ptr<Scene>> m_scenes{};
	};
}
