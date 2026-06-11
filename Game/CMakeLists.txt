if(EMSCRIPTEN)
    set(GAME_TARGET_NAME game_web)
    set(GAME_TARGET_PARAM)
else()
    set(GAME_TARGET_NAME game)
    set(GAME_TARGET_PARAM)
endif()

add_executable(${GAME_TARGET_NAME} ${GAME_TARGET_PARAM}
    Main.cpp
    Scripts/Components/PengoStateComponent.cpp
    Scripts/Components/EnemyStateComponent.cpp
    Scripts/Components/PengoGridComponent.cpp
    Scripts/Components/BlockComponent.cpp
    Scripts/States/PengoIdleState.cpp
    Scripts/States/PengoState.cpp
    Scripts/States/PengoWalkingStates.cpp
    Scripts/States/PushingState.cpp
    Scripts/Level/LevelLoader.cpp
    Scripts/Level/LevelManager.cpp Scripts/States/Enemies/EnemySpawnState.cpp
    Scripts/States/Enemies/EnemyWalkingState.cpp
    Scripts/States/Enemies/EnemyPunchingState.cpp 
    Scripts/States/Enemies/EnemyRunningState.cpp 
    Scripts/States/Enemies/EnemySquished.cpp 
    Scripts/States/Enemies/EnemyState..cpp)

target_include_directories(${GAME_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/States
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Components
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Level
)

target_link_libraries(${GAME_TARGET_NAME} PRIVATE Minigin)

add_custom_command(TARGET ${GAME_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Data
    $<TARGET_FILE_DIR:${GAME_TARGET_NAME}>/Data
)

# Copy DLLs next to the .exe
if(WIN32)
    add_custom_command(TARGET ${GAME_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
        $<TARGET_FILE:SDL3_mixer::SDL3_mixer>
        $<TARGET_FILE_DIR:${GAME_TARGET_NAME}>
    )
endif()

if(EMSCRIPTEN)
    set_target_properties(${GAME_TARGET_NAME} PROPERTIES
        SUFFIX ".html"
    )
    target_compile_options(${GAME_TARGET_NAME} PRIVATE -fexceptions)
    target_link_options(${GAME_TARGET_NAME} PRIVATE
        -fexceptions
        -sALLOW_MEMORY_GROWTH=1
        -sASSERTIONS=1
        --preload-file ${CMAKE_SOURCE_DIR}/Data@/Data
    )
endif()