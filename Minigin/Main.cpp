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
#include "TrashTheCasheComponent.h"
#include "InputManager.h"
#include "Commands/MoveCommand.h"

#include <filesystem>
#include <SDL3/SDL_oldnames.h>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto& inputManager { dae::InputManager::GetInstance() };

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

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");

	inputManager.BindKeyboardCommand(SDLK_D, std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 1.f, 0.f, 0.f }, 5.f));
	inputManager.BindKeyboardCommand(SDLK_A, std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ -1.f, 0.f, 0.f }, 5.f));
	inputManager.BindKeyboardCommand(SDLK_S, std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 0.f, 1.f, 0.f }, 5.f));
	inputManager.BindKeyboardCommand(SDLK_W, std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 0.f, -1.f, 0.f }, 5.f));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");


	scene.Add(std::move(go));

	/*go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TrashTheCasheComponent>();
	scene.Add(std::move(go))*/;
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
