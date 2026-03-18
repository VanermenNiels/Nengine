#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "ScoreComponent.h"
#include "Scene.h"
#include "InputManager.h"
#include "Commands/MoveCommand.h"
#include <Xinput.h>


#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto& inputManager { dae::InputManager::GetInstance() };

	inputManager.InitializeControllers();

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

	// Explanation
	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Use the D-Pad to move Pengo, X to inflict damage, A and B to pick up pellets", font2);
	go->SetPosition(0, 140);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Use the D-Pad to move Pengo, X to inflict damage, A and B to pick up pellets", font2);
	go->SetPosition(0, 165);
	scene.Add(std::move(go));

	//Player scores
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::ScoreComponent>(go->AddComponent<dae::TextComponent>(" ", font2));
	go->SetPosition(0, 200);
	scene.Add(std::move(go));

	// Players
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::FPSComponent>(go->AddComponent<dae::TextComponent>(" ", font));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");
	go->SetPosition(275, 300);

	auto right = std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 1.f, 0.f , 0.f }, 200.f);
	auto left  = std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ -1.f, 0.f, 0.f }, 200.f);
	auto down  = std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 0.f, 1.f , 0.f }, 200.f);
	auto up    = std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 0.f, -1.f, 0.f }, 200.f);

	inputManager.BindKeyboardCommand(SDLK_D, std::move(right));
	inputManager.BindKeyboardCommand(SDLK_A, std::move(left));
	inputManager.BindKeyboardCommand(SDLK_S, std::move(down));
	inputManager.BindKeyboardCommand(SDLK_W, std::move(up));

	scene.Add(std::move(go));


	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");
	go->SetPosition(250, 300);

	right = std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 1.f, 0.f , 0.f }, 200.f);
	left  = std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ -1.f, 0.f, 0.f }, 200.f);
	down  = std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 0.f, 1.f , 0.f }, 200.f);
	up    = std::make_unique<dae::MoveCommand>(go.get(), glm::vec3{ 0.f, -1.f, 0.f }, 200.f);

	inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_RIGHT, std::move(right));
	inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_LEFT , std::move(left));
	inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_DOWN , std::move(down));
	inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_UP   , std::move(up));

	scene.Add(std::move(go));
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
