#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "FPSComponent.h"
#include "RotateComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto renderComp { go->AddComponent<dae::RenderComponent>() };
	renderComp->SetTexture("logo.png");
	go->SetPosition(358, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<dae::GameObject>();
	to->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	to->SetPosition(292, 20);
	scene.Add(std::move(to));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::FPSComponent>(go->AddComponent<dae::TextComponent>(" ", font));
	scene.Add(std::move(go));

	auto child{ std::make_unique<dae::GameObject>() };
	child->AddComponent<dae::RotateComponent>(10.f, 25.f);
	child->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");
	

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");
	go->AddComponent<dae::RotateComponent>(-10.f, 50.f)->SetRotationPoint({ 400.f, 300.f, 0.f });
	child->SetParent(go.get(), false);
	scene.Add(std::move(go));
	scene.Add(std::move(child));

	
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
