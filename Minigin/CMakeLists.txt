add_library(Minigin STATIC
    Font.cpp
    GameObject.cpp
    InputManager.cpp
    Minigin.cpp
    Renderer.cpp
    ResourceManager.cpp
    Scene.cpp
    SceneManager.cpp
    BaseComponent.cpp
    TextComponent.cpp
    Texture2D.cpp
    Transform.cpp
    FPSComponent.cpp
    RenderComponent.cpp
    RotateComponent.cpp
    TrashTheCasheComponent.cpp
    ControllerInput.cpp
    HealthComponent.cpp
    ScoreComponent.cpp
    Commands/Command.cpp
    Commands/MoveCommand.cpp
    Commands/EventCommand.cpp
    Observers/Observer.cpp
    Observers/ScoreDisplay.cpp
    Observers/HealthDisplay.cpp
    Components/ObserverComponent.cpp
    Events/Subject.cpp
    Events/Event.cpp
    HitboxComponent.cpp
    CollisionManager.cpp
    Sound/ServiceLocator.cpp 
    Sound/SDLMixerSoundService.cpp 
    Observers/SoundObserver.cpp 
    AnimatorComponent.cpp 
    Components/GridComponent.cpp 
    State.cpp 
    Components/StateComponent.cpp)

target_include_directories(Minigin PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
    ${glm_SOURCE_DIR}
    ${glm_SOURCE_DIR}/glm
    ${sdl3_mixer_BINARY_DIR}/include
    ${sdl3_mixer_SOURCE_DIR}/include
)

target_link_libraries(Minigin
    PUBLIC
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        imgui
        ${VLD_LIBRARIES}
        SDL3_mixer::SDL3_mixer
)

if(WIN32)
    target_link_libraries(Minigin PUBLIC xinput)
endif()

if(MSVC)
    target_compile_options(Minigin PRIVATE /W4 /WX)
else()
    target_compile_options(Minigin PRIVATE -Wall -Wextra -Werror)
endif()