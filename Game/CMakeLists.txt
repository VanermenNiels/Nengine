if(EMSCRIPTEN)
    set(GAME_TARGET_NAME game_web)
    set(GAME_TARGET_PARAM)
else()
    set(GAME_TARGET_NAME game)
    set(GAME_TARGET_PARAM)
endif()

add_executable(${GAME_TARGET_NAME} ${GAME_TARGET_PARAM}
    Main.cpp
    Scripts/States/PengoIdleState.cpp
    Scripts/States/PengoState.cpp
    Scripts/PengoStateComponent.cpp
    Scripts/States/PengoWalkingStates.cpp
    Scripts/States/PushingState.cpp)

target_include_directories(${GAME_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/States
)

target_link_libraries(${GAME_TARGET_NAME} PRIVATE Minigin)

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