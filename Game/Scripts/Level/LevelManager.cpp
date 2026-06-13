#include <SDL3/SDL_Keycode.h>
#include "LevelManager.h"
#include "GameObject.h"
#include "../Variables.h"
#include "LevelLoader.h"
#include "Components/PengoGridComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "AnimatorComponent.h"
#include "RenderComponent.h"
#include "InputManager.h"
#include "PengoStateComponent.h"
#include "EnemyStateComponent.h"
#include "../EventIDs.h"
#include "Tags.h"
#include "HealthComponent.h"
#include "Commands/EventCommand.h"
#include "Components/BlockComponent.h"
#include "HitboxComponent.h"
#include "EnemyManager.h"
#ifndef __EMSCRIPTEN__
#include <Xinput.h>
#endif

static void SetupLevel(
    dae::PengoGridComponent* gridComp,
    dae::EnemyManager* eMRPtr,
    const dae::SpawnData& spawnData,
    dae::Scene& scene,
    glm::vec2& player1SpawnOut,
    glm::vec2& player2SpawnOut)
{
    for (const auto& data : spawnData)
    {
        switch (data.type)
        {
        case dae::CellType::PlayerSpawn:
            if (player1SpawnOut == glm::vec2{})
                player1SpawnOut = data.position;
            else
                player2SpawnOut = data.position;
            break;
        case dae::CellType::EnemySpawn:
        {
            auto enemyGO = std::make_unique<dae::GameObject>();
            enemyGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
            enemyGO->AddComponent<dae::AnimatorComponent>();
            auto state{ enemyGO->AddComponent<dae::EnemyStateComponent>(gridComp, false) };
            enemyGO->SetTag(dae::Tags::Enemy);
            enemyGO->AddComponent<dae::HitboxComponent>(32, 32);
            enemyGO->SetPosition(data.position.x, data.position.y);
            gridComp->AddEnemyObject(enemyGO.get());
            eMRPtr->AddEnemyOnField(state);
            scene.Add(std::move(enemyGO));
            break;
        }
        case dae::CellType::IceBlock:
        {
            auto cellGO = std::make_unique<dae::GameObject>();
            cellGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/IceBlocks.png");
            cellGO->AddComponent<dae::AnimatorComponent>()->PlayAnimation(0, 0, 32, 32, 1, 0.1f, false);
            cellGO->AddComponent<dae::HitboxComponent>(32, 32);
            auto cell{ gridComp->WorldToCell(data.position) };
            gridComp->AddBlockComponent(cell, cellGO->AddComponent<dae::BlockComponent>(cell, data.type, gridComp));
            cellGO->GetComponent<dae::BlockComponent>()->GetSubject().AddObserver(eMRPtr);
            cellGO->SetPosition(data.position.x, data.position.y);
            scene.Add(std::move(cellGO));
            break;
        }
        case dae::CellType::EggBlock:
        {
            auto cellGO = std::make_unique<dae::GameObject>();
            cellGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/IceBlocks.png");
            cellGO->AddComponent<dae::AnimatorComponent>()->PlayAnimation(0, 0, 32, 32, 2, 0.1f, false, false, 6);
            cellGO->AddComponent<dae::HitboxComponent>(32, 32);
            auto cell{ gridComp->WorldToCell(data.position) };
            gridComp->AddBlockComponent(cell, cellGO->AddComponent<dae::BlockComponent>(cell, data.type, gridComp));
            cellGO->GetComponent<dae::BlockComponent>()->GetSubject().AddObserver(eMRPtr);
            auto block{ cellGO->GetComponent<dae::BlockComponent>() };
            cellGO->SetPosition(data.position.x, data.position.y);
            scene.Add(std::move(cellGO));

            auto enemyGO = std::make_unique<dae::GameObject>();
            enemyGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
            enemyGO->AddComponent<dae::AnimatorComponent>();
            auto state{ enemyGO->AddComponent<dae::EnemyStateComponent>(gridComp, true) };
            enemyGO->SetTag(dae::Tags::Enemy);
            enemyGO->AddComponent<dae::HitboxComponent>(32, 32)->SetEnabled(false);
            enemyGO->SetPosition(data.position.x, data.position.y);
            eMRPtr->AddEnemyInEgg(state, block);
            block->SetEnemyGO(enemyGO.get());
            scene.Add(std::move(enemyGO));
            break;
        }
        case dae::CellType::DiamondBlock:
        {
            auto cellGO = std::make_unique<dae::GameObject>();
            cellGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/IceBlocks.png");
            cellGO->AddComponent<dae::AnimatorComponent>(true)->PlayAnimation(0, 1, 32, 32, 2, 0.1f);
            cellGO->AddComponent<dae::HitboxComponent>(32, 32);
            auto cell{ gridComp->WorldToCell(data.position) };
            gridComp->AddBlockComponent(cell, cellGO->AddComponent<dae::BlockComponent>(cell, data.type, gridComp));
            cellGO->GetComponent<dae::BlockComponent>()->GetSubject().AddObserver(eMRPtr);
            cellGO->SetPosition(data.position.x, data.position.y);
            scene.Add(std::move(cellGO));
            break;
        }
        default:
            break;
        }
    }
}

static void SetupWalls(dae::PengoGridComponent* gridComp, dae::Scene& scene)
{
    const auto& bounds{ gridComp->GetBounds() };
    const float cellSize{ gridComp->GetCellSize() };

    auto wallGO = std::make_unique<dae::GameObject>();
    wallGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/HorizontalWall.png");
    wallGO->AddComponent<dae::AnimatorComponent>()->PlayAnimation(0, 0, 416, 16, 1, 0.1f, false);
    wallGO->SetPosition(bounds[0].x + cellSize, bounds[0].y + cellSize / 2);
    scene.Add(std::move(wallGO));

    wallGO = std::make_unique<dae::GameObject>();
    wallGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/HorizontalWall.png");
    wallGO->AddComponent<dae::AnimatorComponent>()->PlayAnimation(0, 0, 416, 16, 1, 0.1f, false);
    wallGO->SetPosition(bounds[2].x + cellSize, bounds[2].y - cellSize);
    scene.Add(std::move(wallGO));

    wallGO = std::make_unique<dae::GameObject>();
    wallGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/VerticalWall.png");
    wallGO->AddComponent<dae::AnimatorComponent>()->PlayAnimation(0, 0, 16, 512, 1, 0.1f, false);
    wallGO->SetPosition(bounds[0].x + cellSize / 2, bounds[0].y + cellSize / 2);
    scene.Add(std::move(wallGO));

    wallGO = std::make_unique<dae::GameObject>();
    wallGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/VerticalWall.png");
    wallGO->AddComponent<dae::AnimatorComponent>()->PlayAnimation(0, 0, 16, 512, 1, 0.1f, false);
    wallGO->SetPosition(bounds[1].x - cellSize, bounds[1].y + cellSize / 2);
    scene.Add(std::move(wallGO));
}

static void BindPlayer1Keys(
    dae::InputManager& inputManager,
    dae::GameObject* player,
    dae::PengoStateComponent* stateComp,
    int animGroup)
{
    // WASD
    inputManager.BindKeyboardCommand(SDLK_W,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Pressed, animGroup);
    inputManager.BindKeyboardCommand(SDLK_W,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_S,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Pressed, animGroup);
    inputManager.BindKeyboardCommand(SDLK_S,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_A,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Pressed, animGroup);
    inputManager.BindKeyboardCommand(SDLK_A,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_D,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Pressed, animGroup);
    inputManager.BindKeyboardCommand(SDLK_D,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_Q,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::IceBlockPushed[0], 0),
        dae::InputManager::InputType::Pressed);

    // Arrow keys
    inputManager.BindKeyboardCommand(SDLK_UP,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Pressed, animGroup);
    inputManager.BindKeyboardCommand(SDLK_UP,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_DOWN,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Pressed, animGroup);
    inputManager.BindKeyboardCommand(SDLK_DOWN,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_LEFT,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Pressed, animGroup);
    inputManager.BindKeyboardCommand(SDLK_LEFT,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_RIGHT,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Pressed, animGroup);
    inputManager.BindKeyboardCommand(SDLK_RIGHT,
        std::make_unique<dae::EventCommand>(player, stateComp, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Released, -1, true);
}

void dae::LevelManager::StartSinglePlayerLevel()
{
    m_Mode = Modes::SinglePlayer;
    constexpr int PLAYER1_ANIM_GROUP = 2;

    auto& inputManager = dae::InputManager::GetInstance();
    auto& scene = dae::SceneManager::GetInstance().GetActiveScene();

    auto gridGO = std::make_unique<dae::GameObject>();
    gridGO->SetPosition(0, 0);
    m_GridCompRPtr = gridGO->AddComponent<dae::PengoGridComponent>(
        32.f, std::vector<dae::GameObject*>{}, std::vector<dae::GameObject*>{});
    scene.Add(std::move(gridGO));

    dae::LevelLoader::GenerateAndSaveLevels("Data/levels.txt", 5, m_GridCompRPtr);
    const auto& spawnData = dae::LevelLoader::LoadLevel("Data/Levels.txt", m_CurrentLevel, m_GridCompRPtr);

    auto eM = std::make_unique<dae::GameObject>();
    auto eMRPtr{ eM->AddComponent<EnemyManager>(std::vector<EventId>{EventIDs::EnemyKilled}) };
    m_EnemyManager = eMRPtr;
    eMRPtr->AddGrid(m_GridCompRPtr);
    scene.Add(std::move(eM));

    SetupWalls(m_GridCompRPtr, scene);

    glm::vec2 player1SpawnPos{};
    glm::vec2 player2SpawnPos{};
    SetupLevel(m_GridCompRPtr, eMRPtr, spawnData, scene, player1SpawnPos, player2SpawnPos);

    auto player1 = std::make_unique<dae::GameObject>();
    player1->SetPosition(player1SpawnPos.x, player1SpawnPos.y);
    player1->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
    player1->AddComponent<dae::AnimatorComponent>();
    player1->SetTag(dae::Tags::Player);

    auto stateComp1 = player1->AddComponent<dae::PengoStateComponent>(
        m_GridCompRPtr,
        std::vector<dae::EventId>{ dae::EventIDs::PlayerMoved[0], dae::EventIDs::PlayerStop[0], dae::EventIDs::IceBlockPushed[0] },
        0);

    auto health = player1->AddComponent<dae::HealthComponent>(3, std::vector<dae::EventId>{ EventIDs::PlayerHit[0] });
    m_PlayerHealths.push_back(health);

    auto playerHitbox = player1->AddComponent<dae::HitboxComponent>(
        dae::Event{ dae::EventIDs::PlayerHit[0] }, dae::Event{}, 32, 32);
    playerHitbox->AddObserver(health);
    playerHitbox->SetTargetTags(Tags::Enemy);
    m_GridCompRPtr->AddPlayerObject(player1.get());

    BindPlayer1Keys(inputManager, player1.get(), stateComp1, PLAYER1_ANIM_GROUP);

    scene.Add(std::move(player1));
}

void dae::LevelManager::StartMultiplayerLevel()
{
    m_Mode = Modes::Multiplayer;
    constexpr int PLAYER1_ANIM_GROUP = 2;
    constexpr int PLAYER2_ANIM_GROUP = 3;

    auto& inputManager = dae::InputManager::GetInstance();
    auto& scene = dae::SceneManager::GetInstance().GetActiveScene();

    // --- GRID SETUP ---
    auto gridGO = std::make_unique<dae::GameObject>();
    gridGO->SetPosition(0, 0);
    m_GridCompRPtr = gridGO->AddComponent<dae::PengoGridComponent>(
        32.f,
        std::vector<dae::GameObject*>{},
        std::vector<dae::GameObject*>{}
    );
    scene.Add(std::move(gridGO));

    dae::LevelLoader::GenerateAndSaveLevels("Data/levels.txt", 5, m_GridCompRPtr);
    const auto& spawnData = dae::LevelLoader::LoadLevel("Data/Levels.txt", m_CurrentLevel, m_GridCompRPtr);

    // --- ENEMY MANAGER ---
    auto eM = std::make_unique<dae::GameObject>();
    auto eMRPtr{ eM->AddComponent<EnemyManager>(std::vector<EventId>{EventIDs::EnemyKilled}) };
    m_EnemyManager = eMRPtr;
    eMRPtr->AddGrid(m_GridCompRPtr);
    scene.Add(std::move(eM));

    // --- WALLS ---
    SetupWalls(m_GridCompRPtr, scene);

    // --- SPAWN DATA ---
    glm::vec2 player1SpawnPos{};
    glm::vec2 player2SpawnPos{};
    SetupLevel(m_GridCompRPtr, eMRPtr, spawnData, scene, player1SpawnPos, player2SpawnPos);

    // --- PLAYER 1 ---
    auto player1 = std::make_unique<dae::GameObject>();
    player1->SetPosition(player1SpawnPos.x, player1SpawnPos.y);
    player1->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
    player1->AddComponent<dae::AnimatorComponent>();
    player1->SetTag(dae::Tags::Player);

    auto stateComp1 = player1->AddComponent<dae::PengoStateComponent>(
        m_GridCompRPtr, std::vector<dae::EventId>{ dae::EventIDs::PlayerMoved[0], dae::EventIDs::PlayerStop[0], dae::EventIDs::IceBlockPushed[0] }, 0);

    auto health1 = player1->AddComponent<dae::HealthComponent>(3, std::vector<dae::EventId>{ EventIDs::PlayerHit[0] });
    m_PlayerHealths.push_back(health1);
    auto playerHitbox1 = player1->AddComponent<dae::HitboxComponent>(
        dae::Event{ dae::EventIDs::PlayerHit[0] },
        dae::Event{},
        32, 32
    );
    playerHitbox1->AddObserver(health1);
    playerHitbox1->SetTargetTags(Tags::Enemy);
    m_GridCompRPtr->AddPlayerObject(player1.get());

    BindPlayer1Keys(inputManager, player1.get(), stateComp1, PLAYER1_ANIM_GROUP);

    scene.Add(std::move(player1));

    // --- PLAYER 2 ---
    auto player2 = std::make_unique<dae::GameObject>();
    player2->SetPosition(player2SpawnPos.x, player2SpawnPos.y);
    player2->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
    player2->AddComponent<dae::AnimatorComponent>();
    player2->SetTag(dae::Tags::Player);

    auto stateComp2 = player2->AddComponent<dae::PengoStateComponent>(
        m_GridCompRPtr, std::vector<dae::EventId>{ dae::EventIDs::PlayerMoved[1], dae::EventIDs::PlayerStop[1], dae::EventIDs::IceBlockPushed[1] }, 1);

    auto health2 = player2->AddComponent<dae::HealthComponent>(3, std::vector<dae::EventId>{ EventIDs::PlayerHit[1] });
    m_PlayerHealths.push_back(health2);
    auto playerHitbox2 = player2->AddComponent<dae::HitboxComponent>(
        dae::Event{ dae::EventIDs::PlayerHit[1] },
        dae::Event{},
        32, 32
    );
    playerHitbox2->AddObserver(health2);
    playerHitbox2->SetTargetTags(Tags::Enemy);
    m_GridCompRPtr->AddPlayerObject(player2.get());

#ifndef __EMSCRIPTEN__
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_UP,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerMoved[1], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Down, PLAYER2_ANIM_GROUP);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_UP,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerStop[1], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_DOWN,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerMoved[1], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Down, PLAYER2_ANIM_GROUP);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_DOWN,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerStop[1], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_LEFT,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerMoved[1], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Down, PLAYER2_ANIM_GROUP);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_LEFT,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerStop[1], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_RIGHT,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerMoved[1], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Down, PLAYER2_ANIM_GROUP);
    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_DPAD_RIGHT,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::PlayerStop[1], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindControllerCommand(0, XINPUT_GAMEPAD_A,
        std::make_unique<dae::EventCommand>(player2.get(), stateComp2, dae::EventIDs::IceBlockPushed[1], 0),
        dae::InputManager::InputType::Pressed, -1, true);
#endif

    scene.Add(std::move(player2));
}

void dae::LevelManager::EventReaction(Event event)
{
    if (m_Started) return;

    if (event.id == EventIDs::StartSingleplayer)
    {
        m_Started = true;
        StartSinglePlayerLevel();
    }
    else if (event.id == EventIDs::StartMultiplayer)
    {
        m_Started = true;
        StartMultiplayerLevel();
    }
}

void dae::LevelManager::Update(float)
{
    if (!m_Started) return;

    bool anyPlayerDead = false;
    for (auto* health : m_PlayerHealths)
    {
        if (health && health->IsDead()) { anyPlayerDead = true; break; }
    }

    if (anyPlayerDead)
    {
        auto& scene = dae::SceneManager::GetInstance().CreateScene();
        auto looseGO = std::make_unique<GameObject>();
        looseGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
        looseGO->AddComponent<dae::AnimatorComponent>()->PlayAnimation(0, 2, 32, 32, 2, 0.15f);
        looseGO->SetPosition(240, 300);
        scene.Add(std::move(looseGO));
        return;
    }

    if (m_EnemyManager && m_EnemyManager->AllEnemiesDead())
    {
        auto& scene = dae::SceneManager::GetInstance().CreateScene();
        auto winGO = std::make_unique<GameObject>();
        winGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
        winGO->AddComponent<dae::AnimatorComponent>()->PlayAnimation(4, 2, 32, 32, 2, 0.15f);
        winGO->SetPosition(240, 300);
        scene.Add(std::move(winGO));
    }
}