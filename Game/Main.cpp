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
#include "Commands/EventCommand.h"
#include <Xinput.h>
#include "Observers/ScoreDisplay.h"
#include "Observers/HealthDisplay.h"
#include "Observers/SoundObserver.h"
#include "HealthComponent.h"
#include "HitboxComponent.h"
#include "AnimatorComponent.h"
#include "Events/Event.h"
#include "Tags.h"
#include "Sound/ServiceLocator.h"
#include "Sound/SDLMixerSoundService.h"
#include "Scripts/PengoStateComponent.h"
#include "EventIDs.h"
#include "Variables.h"

#include <filesystem>
namespace fs = std::filesystem;

constexpr int PLAYER1_MOVE_GROUP = 0;
constexpr int PLAYER2_MOVE_GROUP = 1;

static void load()
{

    auto& scene = dae::SceneManager::GetInstance().CreateScene();
    auto& inputManager = dae::InputManager::GetInstance();
    inputManager.InitializeControllers();

    constexpr auto SCORE_EVENT = dae::make_sdbm_hash("AddScore");
    constexpr auto DAMAGE_EVENT = dae::make_sdbm_hash("Damage");
    constexpr auto OVERLAP_EVENT = dae::make_sdbm_hash("OnOverlapBegin");
    constexpr auto OVERLAP_END_EVENT = dae::make_sdbm_hash("OnOverlapEnd");

    dae::Event beginOE{ OVERLAP_EVENT };
    beginOE.args[0].intVal = -1;
    beginOE.nbArgs = 1;

    dae::Event endOE{ OVERLAP_END_EVENT };
    endOE.args[0].intVal = 10;
    endOE.nbArgs = 1;

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
    instr->AddComponent<dae::TextComponent>("Use D-Pad to move Pengo 2, use WASD to move Pengo 1", fontSmall);
    instr->SetPosition(0, 120);
    scene.Add(std::move(instr));

    // ======================
    // --- PLAYER 1 SETUP ---
    // ======================
    auto player1 = std::make_unique<dae::GameObject>();
    player1->SetPosition(275, 300);
    player1->AddComponent<dae::RenderComponent>()->SetTexture("Spritesheets/PengoCharactersSprites.png");
    player1->AddComponent<dae::AnimatorComponent>();
    player1->SetTag(dae::Tags::Player);

    auto stateComp1 = player1->AddComponent<dae::PengoStateComponent>(
        std::vector<dae::EventId>{ dae::EventIDs::PlayerMoved[0], dae::EventIDs::PlayerStop[0] }, 0);

    // Score
    auto scoreComp1 = player1->AddComponent<dae::ScoreComponent>(std::vector<dae::EventId>{ OVERLAP_END_EVENT });

    auto scoreUI1 = std::make_unique<dae::GameObject>();
    auto text1 = scoreUI1->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
    scoreUI1->SetPosition(0, 250);
    scene.Add(std::move(scoreUI1));
    scoreComp1->AddObserver(new dae::ScoreDisplay(text1, scoreComp1));

    // Health
    auto healthComp1 = player1->AddComponent<dae::HealthComponent>(3, std::vector<dae::EventId>
    {
        dae::make_sdbm_hash("Damage"), OVERLAP_EVENT
    });

    auto healthUI1 = std::make_unique<dae::GameObject>();
    auto hText1 = healthUI1->AddComponent<dae::TextComponent>("Lives: 3", fontSmall);
    healthUI1->SetPosition(0, 220);
    scene.Add(std::move(healthUI1));
    healthComp1->AddObserver(new dae::HealthDisplay(hText1, healthComp1));

    // Hitbox
    auto hitbox1 = player1->AddComponent<dae::HitboxComponent>(beginOE, endOE, 18.f, 18.f);
    hitbox1->SetTargetTags(dae::Tags::Enemy);
    hitbox1->AddObserver(healthComp1);
    hitbox1->AddObserver(scoreComp1);

    auto* soundObserver1 = new dae::SoundObserver(&dae::ServiceLocator::GetSoundService());
    soundObserver1->BindSound(OVERLAP_EVENT, "Data/IceBlockDestroy.mp3", 100);
    hitbox1->AddObserver(soundObserver1);

    // W
    inputManager.BindKeyboardCommand(SDLK_W,
        std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 0,-1,0 }, 200.f),
        dae::InputManager::InputType::Down, PLAYER1_MOVE_GROUP);
    inputManager.BindKeyboardCommand(SDLK_W,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Pressed, -1, true);
    inputManager.BindKeyboardCommand(SDLK_W,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Released, -1, true);

    // S
    inputManager.BindKeyboardCommand(SDLK_S,
        std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 0,1,0 }, 200.f),
        dae::InputManager::InputType::Down, PLAYER1_MOVE_GROUP);
    inputManager.BindKeyboardCommand(SDLK_S,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Pressed, -1, true);
    inputManager.BindKeyboardCommand(SDLK_S,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Released, -1, true);

    // A
    inputManager.BindKeyboardCommand(SDLK_A,
        std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ -1,0,0 }, 200.f),
        dae::InputManager::InputType::Down, PLAYER1_MOVE_GROUP);
    inputManager.BindKeyboardCommand(SDLK_A,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Pressed, -1, true);
    inputManager.BindKeyboardCommand(SDLK_A,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Released, -1, true);

    // D
    inputManager.BindKeyboardCommand(SDLK_D,
        std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3{ 1,0,0 }, 200.f),
        dae::InputManager::InputType::Down, PLAYER1_MOVE_GROUP);
    inputManager.BindKeyboardCommand(SDLK_D,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Pressed, -1, true);
    inputManager.BindKeyboardCommand(SDLK_D,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Released, -1, true);

    // Other player1 bindings — always fire regardless of movement
    inputManager.BindKeyboardCommand(SDLK_Z,
        std::make_unique<dae::EventCommand>(player1.get(), scoreComp1, SCORE_EVENT, 10),
        dae::InputManager::InputType::Pressed, -1, true);
    inputManager.BindKeyboardCommand(SDLK_X,
        std::make_unique<dae::EventCommand>(player1.get(), scoreComp1, SCORE_EVENT, 3),
        dae::InputManager::InputType::Pressed, -1, true);
    inputManager.BindKeyboardCommand(SDLK_C,
        std::make_unique<dae::EventCommand>(player1.get(), healthComp1, DAMAGE_EVENT, 1),
        dae::InputManager::InputType::Pressed, -1, true);

    scene.Add(std::move(player1));

    // ======================
    // --- PLAYER 2 SETUP ---
    // ======================
    auto player2 = std::make_unique<dae::GameObject>();
    player2->SetPosition(250, 300);
    player2->AddComponent<dae::RenderComponent>()->SetTexture("Spritesheets/PengoCharactersSprites.png");
    player2->AddComponent<dae::AnimatorComponent>();
    player2->SetTag(dae::Tags::Enemy);

    auto stateComp2 = player2->AddComponent<dae::PengoStateComponent>(
        std::vector<dae::EventId>{ dae::EventIDs::PlayerMoved[1] }, 1);

    // Score
    auto scoreComp2 = player2->AddComponent<dae::ScoreComponent>();

    auto scoreUI2 = std::make_unique<dae::GameObject>();
    auto text2 = scoreUI2->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
    scoreUI2->SetPosition(0, 310);
    scene.Add(std::move(scoreUI2));
    scoreComp2->AddObserver(new dae::ScoreDisplay(text2, scoreComp2));

    // Health
    auto healthComp2 = player2->AddComponent<dae::HealthComponent>(3, std::vector<dae::EventId>
    {
        dae::make_sdbm_hash("Damage"), dae::make_sdbm_hash("OnOverlapBegin")
    });

    auto healthUI2 = std::make_unique<dae::GameObject>();
    auto hText2 = healthUI2->AddComponent<dae::TextComponent>("Lives: 3", fontSmall);
    healthUI2->SetPosition(0, 280);
    scene.Add(std::move(healthUI2));
    healthComp2->AddObserver(new dae::HealthDisplay(hText2, healthComp2));

    // Hitbox
    auto hitbox2 = player2->AddComponent<dae::HitboxComponent>(beginOE, endOE, 18.f, 18.f);
    hitbox2->SetTargetTags(dae::Tags::Player);
    hitbox2->AddObserver(healthComp2);

    auto* soundObserver2 = new dae::SoundObserver(&dae::ServiceLocator::GetSoundService());
    soundObserver2->BindSound(OVERLAP_EVENT, "Data/IceBlockDestroy.mp3", 100);
    hitbox2->AddObserver(soundObserver2);

    // D-Pad Right
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_RIGHT,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 1,0,0 }, 200.f),
        dae::InputManager::InputType::Down, PLAYER2_MOVE_GROUP);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_RIGHT,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerMoved[1], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Pressed, -1, true);

    // D-Pad Left
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_LEFT,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ -1,0,0 }, 200.f),
        dae::InputManager::InputType::Down, PLAYER2_MOVE_GROUP);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_LEFT,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerMoved[1], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Pressed, -1, true);

    // D-Pad Down
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_DOWN,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 0,1,0 }, 200.f),
        dae::InputManager::InputType::Down, PLAYER2_MOVE_GROUP);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_DOWN,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerMoved[1], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Pressed, -1, true);

    // D-Pad Up
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_UP,
        std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3{ 0,-1,0 }, 200.f),
        dae::InputManager::InputType::Down, PLAYER2_MOVE_GROUP);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_UP,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerMoved[1], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Pressed, -1, true);

    // Other player2 bindings — always fire regardless of movement
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_A,
        std::make_unique<dae::EventCommand>(player2.get(), scoreComp2, SCORE_EVENT),
        dae::InputManager::InputType::Pressed, -1, true);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_B,
        std::make_unique<dae::EventCommand>(player2.get(), scoreComp2, SCORE_EVENT),
        dae::InputManager::InputType::Pressed, -1, true);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_X,
        std::make_unique<dae::EventCommand>(player2.get(), healthComp2, DAMAGE_EVENT),
        dae::InputManager::InputType::Pressed, -1, true);

    scene.Add(std::move(player2));
}

int main(int, char* [])
{
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    dae::Minigin engine(data_location);

    auto soundService = std::make_unique<dae::SDLMixerSoundService>();
    dae::ServiceLocator::RegisterSoundService(soundService.get());

    engine.Run(load);

    dae::ServiceLocator::RegisterSoundService(nullptr);
    soundService.reset();

    return 0;
}