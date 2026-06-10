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
#include "Scripts/Components/PengoStateComponent.h"
#include "EventIDs.h"
#include "Variables.h"
#include "Scripts/Level/LevelLoader.h"
#include "Scripts/Components/PengoGridComponent.h"

#include "Scripts/Level/LevelManager.h"

#include <filesystem>
namespace fs = std::filesystem;

constexpr int PLAYER1_MOVE_GROUP = 0;
constexpr int PLAYER1_ANIM_GROUP = 2;
constexpr int PLAYER2_MOVE_GROUP = 1;
constexpr int PLAYER2_ANIM_GROUP = 3;

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
    auto LevelManager = std::make_unique<dae::GameObject>();
    LevelManager->AddComponent<dae::LevelManager>()->StartLevel();
    scene.Add(std::move(LevelManager));

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