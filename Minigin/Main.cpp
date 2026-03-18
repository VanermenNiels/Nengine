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
#include "Commands/EventCommand.h"
#include "Observers/ScoreDisplay.h"
#include "Events/Event.h"


#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene();
    auto& inputManager = dae::InputManager::GetInstance();
    inputManager.InitializeControllers();

    constexpr auto SCORE_EVENT = dae::make_sdbm_hash("AddScore");

    auto bg = std::make_unique<dae::GameObject>();
    bg->AddComponent<dae::RenderComponent>()->SetTexture("background.png");
    scene.Add(std::move(bg));

    auto logo = std::make_unique<dae::GameObject>();
    logo->AddComponent<dae::RenderComponent>()->SetTexture("logo.png");
    logo->SetPosition(358, 180);
    scene.Add(std::move(logo));

    auto titleFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

    auto titleGO = std::make_unique<dae::GameObject>();
    titleGO->AddComponent<dae::TextComponent>("Programming 4 Assignment", titleFont);
    titleGO->SetPosition(292, 20);
    scene.Add(std::move(titleGO));

    auto instr = std::make_unique<dae::GameObject>();
    instr->AddComponent<dae::TextComponent>("Use D-Pad to move Pengo, X to inflict damage, A and B to pick up pellets", fontSmall);
    instr->SetPosition(0, 140);
    scene.Add(std::move(instr));

    instr = std::make_unique<dae::GameObject>();
    instr->AddComponent<dae::TextComponent>("Use WASD to move Pengo, C to inflict damage, Z and X to pick up pellets", fontSmall);
    instr->SetPosition(0, 170);
    scene.Add(std::move(instr));

    // PLAYER 1
    auto subject1 = new dae::Subject();

    auto player1 = std::make_unique<dae::GameObject>();
    player1->SetPosition(275, 300);
    player1->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");

    auto scoreComp1 = player1->AddComponent<dae::ScoreComponent>();
    scoreComp1->SetSubject(subject1);

    subject1->AddObserver(scoreComp1);

    // UI
    auto scoreUI1 = std::make_unique<dae::GameObject>();
    auto text1 = scoreUI1->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
    scoreUI1->SetPosition(0, 250);

    auto display1 = new dae::ScoreDisplay(text1, scoreComp1);
    subject1->AddObserver(display1);

    // Movement
    inputManager.BindKeyboardCommand(SDLK_D, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 1,0,0 }, 200.f));
    inputManager.BindKeyboardCommand(SDLK_A, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ -1,0,0 }, 200.f));
    inputManager.BindKeyboardCommand(SDLK_S, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 0,1,0 }, 200.f));
    inputManager.BindKeyboardCommand(SDLK_W, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 0,-1,0 }, 200.f));

    inputManager.BindKeyboardCommand(
        SDLK_Z,
        std::make_unique<dae::EventCommand>(player1.get(), subject1, SCORE_EVENT)
    );

    inputManager.BindKeyboardCommand(
        SDLK_X,
        std::make_unique<dae::EventCommand>(player1.get(), subject1, SCORE_EVENT)
    );

    scene.Add(std::move(scoreUI1));
    scene.Add(std::move(player1));

    //Player 2
    auto subject2 = new dae::Subject();

    auto player2 = std::make_unique<dae::GameObject>();
    player2->SetPosition(250, 300);
    player2->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");

    auto scoreComp2 = player2->AddComponent<dae::ScoreComponent>();
    scoreComp2->SetSubject(subject2);

    subject2->AddObserver(scoreComp2);

    auto scoreUI2 = std::make_unique<dae::GameObject>();
    auto text2 = scoreUI2->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
    scoreUI2->SetPosition(0, 310);

    auto display2 = new dae::ScoreDisplay(text2, scoreComp2);
    subject2->AddObserver(display2);

    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_RIGHT,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 1,0,0 }, 200.f));
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_LEFT,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ -1,0,0 }, 200.f));
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_DOWN,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 0,1,0 }, 200.f));
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_UP,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 0,-1,0 }, 200.f));

    inputManager.BindControllerCommand(
        0,
        XINPUT_GAMEPAD_A,
        std::make_unique<dae::EventCommand>(player2.get(), subject2, SCORE_EVENT)
    );

    inputManager.BindControllerCommand(
        0,
        XINPUT_GAMEPAD_B,
        std::make_unique<dae::EventCommand>(player2.get(), subject2, SCORE_EVENT)
    );

    scene.Add(std::move(scoreUI2));
    scene.Add(std::move(player2));
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
