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


void dae::LevelManager::StartLevel()
{
    constexpr int PLAYER1_ANIM_GROUP = 2;

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
    auto eMRPtr { eM->AddComponent<EnemyManager>(std::vector<EventId>{EventIDs::EnemyKilled})};
    eMRPtr->AddGrid(m_GridCompRPtr); // ADD THIS LINE
    scene.Add(std::move(eM));

    // --- WALLS ---
    const auto& bounds{ m_GridCompRPtr->GetBounds() };
    const float cellSize{ m_GridCompRPtr->GetCellSize() };

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

    // --- SPAWN DATA ---
    glm::vec2 playerSpawnPos{};

    for (const auto& data : spawnData)
    {
        switch (data.type)
        {
        case dae::CellType::PlayerSpawn:
            playerSpawnPos = data.position;
            break;
        case dae::CellType::EnemySpawn:
        {
            auto enemyGO = std::make_unique<dae::GameObject>();
            enemyGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
            enemyGO->AddComponent<dae::AnimatorComponent>();
            auto state{ enemyGO->AddComponent<dae::EnemyStateComponent>(m_GridCompRPtr, false) };
            enemyGO->AddComponent<dae::HitboxComponent>(32, 32);

            enemyGO->SetPosition(data.position.x, data.position.y);

            m_GridCompRPtr->AddEnemyObject(enemyGO.get());
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

            auto cell{ m_GridCompRPtr->WorldToCell(data.position) };
            m_GridCompRPtr->AddBlockComponent(cell, cellGO->AddComponent<dae::BlockComponent>(cell, data.type, m_GridCompRPtr));
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

            auto cell{ m_GridCompRPtr->WorldToCell(data.position) };
            m_GridCompRPtr->AddBlockComponent(cell, cellGO->AddComponent<dae::BlockComponent>(cell, data.type, m_GridCompRPtr));
            cellGO->GetComponent<dae::BlockComponent>()->GetSubject().AddObserver(eMRPtr);
            auto block{ cellGO->GetComponent<dae::BlockComponent>() };

            cellGO->SetPosition(data.position.x, data.position.y);
            scene.Add(std::move(cellGO));

            auto enemyGO = std::make_unique<dae::GameObject>();
            enemyGO->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
            enemyGO->AddComponent<dae::AnimatorComponent>();
            auto state{ enemyGO->AddComponent<dae::EnemyStateComponent>(m_GridCompRPtr, true) };
            enemyGO->AddComponent<dae::HitboxComponent>(32, 32);

            enemyGO->SetPosition(data.position.x, data.position.y);

            // DO NOT add to grid here — egg enemies are not active until hatched
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

            auto cell{ m_GridCompRPtr->WorldToCell(data.position) };
            m_GridCompRPtr->AddBlockComponent(cell, cellGO->AddComponent<dae::BlockComponent>(cell, data.type, m_GridCompRPtr));
            cellGO->GetComponent<dae::BlockComponent>()->GetSubject().AddObserver(eMRPtr);

            cellGO->SetPosition(data.position.x, data.position.y);
            scene.Add(std::move(cellGO));
            break;
        }

        default:
            break;
        }
    }

    // --- PLAYER 1 ---
    auto player1 = std::make_unique<dae::GameObject>();
    player1->SetPosition(playerSpawnPos.x, playerSpawnPos.y);
    player1->AddComponent<dae::RenderComponent>()->SetTexture("SpriteSheets/PengoCharactersSprites2.png");
    player1->AddComponent<dae::AnimatorComponent>();
    player1->SetTag(dae::Tags::Player);

    auto stateComp1 = player1->AddComponent<dae::PengoStateComponent>(
        m_GridCompRPtr, std::vector<dae::EventId>{ dae::EventIDs::PlayerMoved[0], dae::EventIDs::PlayerStop[0], dae::EventIDs::IceBlockPushed[0] }, 0);

    player1->AddComponent<dae::HealthComponent>(3, std::vector<dae::EventId>{ dae::make_sdbm_hash("Damage") });

    // NEW: register this player with the grid so GetPlayerCells() isn't empty
    m_GridCompRPtr->AddPlayerObject(player1.get());

    inputManager.BindKeyboardCommand(SDLK_W,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Pressed, PLAYER1_ANIM_GROUP);
    inputManager.BindKeyboardCommand(SDLK_W,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Up)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_S,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Pressed, PLAYER1_ANIM_GROUP);
    inputManager.BindKeyboardCommand(SDLK_S,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Down)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_A,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Pressed, PLAYER1_ANIM_GROUP);
    inputManager.BindKeyboardCommand(SDLK_A,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Left)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_D,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerMoved[0], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Pressed, PLAYER1_ANIM_GROUP);
    inputManager.BindKeyboardCommand(SDLK_D,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::PlayerStop[0], static_cast<int>(dae::Direction::Right)),
        dae::InputManager::InputType::Released, -1, true);

    inputManager.BindKeyboardCommand(SDLK_Q,
        std::make_unique<dae::EventCommand>(player1.get(), stateComp1, dae::EventIDs::IceBlockPushed[0], 0),
        dae::InputManager::InputType::Pressed);

    scene.Add(std::move(player1));
}

void dae::LevelManager::EventReaction(Event)
{}