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
#include "Observers/HealthDisplay.h"
#include "HealthComponent.h"
#include "HitboxComponent.h"
#include "Events/Event.h"
#include "Tags.h"


#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene();
    auto& inputManager = dae::InputManager::GetInstance();
    inputManager.InitializeControllers();

    constexpr auto SCORE_EVENT = dae::make_sdbm_hash("AddScore");
    auto DAMAGE_EVENT = dae::make_sdbm_hash("Damage");
    constexpr auto OVERLAP_EVENT = dae::make_sdbm_hash("OnOverlapBegin");

	dae::Event overlapE{ OVERLAP_EVENT };
    overlapE.args[0].intVal = -1;
    overlapE.nbArgs = 1;

    // --- BACKGROUND ---
    auto bg = std::make_unique<dae::GameObject>();
    bg->AddComponent<dae::RenderComponent>()->SetTexture("background.png");
    scene.Add(std::move(bg));

    // --- LOGO ---
    auto logo = std::make_unique<dae::GameObject>();
    logo->AddComponent<dae::RenderComponent>()->SetTexture("logo.png");
    logo->SetPosition(358, 180);
    scene.Add(std::move(logo));

    // --- FONTS ---
    auto titleFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

    // --- TITLE ---
    auto titleGO = std::make_unique<dae::GameObject>();
    titleGO->AddComponent<dae::TextComponent>("Programming 4 Assignment", titleFont);
    titleGO->SetPosition(292, 20);
    scene.Add(std::move(titleGO));

    // --- FPS ---
    auto FPS = std::make_unique<dae::GameObject>();
    FPS->AddComponent<dae::FPSComponent>(FPS->AddComponent<dae::TextComponent>(" ", titleFont));
    scene.Add(std::move(FPS));

    // --- INSTRUCTIONS ---
    auto instr = std::make_unique<dae::GameObject>();
    instr->AddComponent<dae::TextComponent>("Use D-Pad to move Pengo, X to inflict damage, A and B to pick up pellets", fontSmall);
    instr->SetPosition(0, 120);
    scene.Add(std::move(instr));

    instr = std::make_unique<dae::GameObject>();
    instr->AddComponent<dae::TextComponent>("Use WASD to move Pengo, C to inflict damage, Z and X to pick up pellets", fontSmall);
    instr->SetPosition(0, 150);
    scene.Add(std::move(instr));

    // ======================
    // --- PLAYER 1 SETUP ---
    // ======================
    auto player1 = std::make_unique<dae::GameObject>();
    player1->SetPosition(275, 300);
    player1->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");
    player1->SetTag(dae::Tags::Player);

    // Score
    auto scoreComp1 = player1->AddComponent<dae::ScoreComponent>();

    auto scoreUI1 = std::make_unique<dae::GameObject>();
    auto text1 = scoreUI1->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
    scoreUI1->SetPosition(0, 250);
    scene.Add(std::move(scoreUI1));

    auto scoreDisplay1 = new dae::ScoreDisplay(text1, scoreComp1);
    scoreComp1->AddObserver(scoreDisplay1);

    // Health
    auto healthComp1 = player1->AddComponent<dae::HealthComponent>(3, std::vector<dae::EventId>
    {
        dae::make_sdbm_hash("Damage"),
        dae::make_sdbm_hash("OnOverlapBegin")
    });

    auto healthUI1 = std::make_unique<dae::GameObject>();
    auto hText1 = healthUI1->AddComponent<dae::TextComponent>("Lives: 3", fontSmall);
    healthUI1->SetPosition(0, 220);
    scene.Add(std::move(healthUI1));

    auto healthDisplay1 = new dae::HealthDisplay(hText1, healthComp1);
    healthComp1->AddObserver(healthDisplay1);

    // Hitbox — reacts to Enemy tag, notifies healthComp1 on overlap
    auto hitbox1 = player1->AddComponent<dae::HitboxComponent>(overlapE, 18.f, 18.f);
    hitbox1->SetTargetTags(dae::Tags::Enemy);
    hitbox1->AddObserver(healthComp1);

    // Movement bindings
    inputManager.BindKeyboardCommand(SDLK_D, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 1,0,0 }, 200.f), dae::InputManager::InputType::Down);
    inputManager.BindKeyboardCommand(SDLK_A, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ -1,0,0 }, 200.f), dae::InputManager::InputType::Down);
    inputManager.BindKeyboardCommand(SDLK_S, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 0,1,0 }, 200.f), dae::InputManager::InputType::Down);
    inputManager.BindKeyboardCommand(SDLK_W, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 0,-1,0 }, 200.f), dae::InputManager::InputType::Down);

    inputManager.BindKeyboardCommand(SDLK_Z, std::make_unique<dae::EventCommand>(player1.get(), scoreComp1, SCORE_EVENT, 10), dae::InputManager::InputType::Pressed);
    inputManager.BindKeyboardCommand(SDLK_X, std::make_unique<dae::EventCommand>(player1.get(), scoreComp1, SCORE_EVENT, 3), dae::InputManager::InputType::Pressed);
    inputManager.BindKeyboardCommand(SDLK_C, std::make_unique<dae::EventCommand>(player1.get(), healthComp1, DAMAGE_EVENT, 1), dae::InputManager::InputType::Pressed);

    scene.Add(std::move(player1));

    // ======================
    // --- PLAYER 2 SETUP ---
    // ======================
    auto player2 = std::make_unique<dae::GameObject>();
    player2->SetPosition(250, 300);
    player2->AddComponent<dae::RenderComponent>()->SetTexture("PengoCharacter.png");
    player2->SetTag(dae::Tags::Enemy); // tag as Enemy so player1 hitbox reacts to it

    // Score
    auto scoreComp2 = player2->AddComponent<dae::ScoreComponent>();

    auto scoreUI2 = std::make_unique<dae::GameObject>();
    auto text2 = scoreUI2->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
    scoreUI2->SetPosition(0, 310);
    scene.Add(std::move(scoreUI2));

    auto scoreDisplay2 = new dae::ScoreDisplay(text2, scoreComp2);
    scoreComp2->AddObserver(scoreDisplay2);

    // Health
    auto healthComp2 = player2->AddComponent<dae::HealthComponent>(3, std::vector<dae::EventId>
    {
        dae::make_sdbm_hash("Damage"),
        dae::make_sdbm_hash("OnOverlapBegin")
    });

    auto healthUI2 = std::make_unique<dae::GameObject>();
    auto hText2 = healthUI2->AddComponent<dae::TextComponent>("Lives: 3", fontSmall);
    healthUI2->SetPosition(0, 280);
    scene.Add(std::move(healthUI2));

    auto healthDisplay2 = new dae::HealthDisplay(hText2, healthComp2);
    healthComp2->AddObserver(healthDisplay2);

    // Hitbox — reacts to Player tag, notifies healthComp2 on overlap
    auto hitbox2 = player2->AddComponent<dae::HitboxComponent>(overlapE, 18.f, 18.f);
    hitbox2->SetTargetTags(dae::Tags::Player);
    hitbox2->AddObserver(healthComp2);

    // Controller movement bindings
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_RIGHT, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 1,0,0 }, 200.f), dae::InputManager::InputType::Down);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_LEFT, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ -1,0,0 }, 200.f), dae::InputManager::InputType::Down);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_DOWN, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 0,1,0 }, 200.f), dae::InputManager::InputType::Down);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_UP, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 0,-1,0 }, 200.f), dae::InputManager::InputType::Down);

    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_A, std::make_unique<dae::EventCommand>(player2.get(), scoreComp2, SCORE_EVENT), dae::InputManager::InputType::Pressed);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_B, std::make_unique<dae::EventCommand>(player2.get(), scoreComp2, SCORE_EVENT), dae::InputManager::InputType::Pressed);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_X, std::make_unique<dae::EventCommand>(player2.get(), healthComp2, DAMAGE_EVENT), dae::InputManager::InputType::Pressed);

    scene.Add(std::move(player2));
}

int main(int, char* []) {
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    dae::Minigin engine(data_location);
    engine.Run(load);
    return 0;
}