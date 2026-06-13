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
#include "Scripts/Components/PengoStateComponent.h"
#include "EventIDs.h"
#include "Variables.h"
#include "Scripts/Level/LevelLoader.h"
#include "Scripts/Components/PengoGridComponent.h"
#include "Scripts/Level/LevelManager.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene();
    auto& inputManager = dae::InputManager::GetInstance();
    inputManager.InitializeControllers();

    auto levelManagerGO = std::make_unique<dae::GameObject>();
    auto levelManager = levelManagerGO->AddComponent<dae::LevelManager>(
        std::vector<dae::EventId>{ dae::EventIDs::StartSingleplayer, dae::EventIDs::StartMultiplayer });
    scene.Add(std::move(levelManagerGO));

    inputManager.BindKeyboardCommand(SDLK_1,
        std::make_unique<dae::EventCommand>(nullptr, levelManager, dae::EventIDs::StartSingleplayer, 0),
        dae::InputManager::InputType::Pressed);

    inputManager.BindKeyboardCommand(SDLK_2,
        std::make_unique<dae::EventCommand>(nullptr, levelManager, dae::EventIDs::StartMultiplayer, 0),
        dae::InputManager::InputType::Pressed);
}

int main(int, char* [])
{
#if __EMSCRIPTEN__
    fs::path data_location = "Data/";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    dae::Minigin engine(data_location);

#ifndef __EMSCRIPTEN__
    auto soundService = std::make_unique<dae::SDLMixerSoundService>();
    dae::ServiceLocator::RegisterSoundService(soundService.get());
#endif

    engine.Run(load);

#ifndef __EMSCRIPTEN__
    dae::ServiceLocator::RegisterSoundService(nullptr);
    soundService.reset();
#endif

    return 0;
}