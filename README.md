# Nengine

A 2D game engine built on top of the [Minigin](https://github.com/avadae/minigin) framework from DAE, written in C++.
Used to recreate *(write about the game)* as the exam project for the Programming 4 course at DAE.

[![Build Status](https://github.com/VanermenNiels/Nengine/actions/workflows/cmake.yml/badge.svg)](https://github.com/VanermenNiels/Nengine/actions)

---

## Project Structure

```
Nengine/
├── Minigin/                  # Engine library (static lib)
│   ├── AnimatorComponent     # Spritesheet animation, frame-based playback
│   ├── RenderComponent       # SDL3 texture rendering
│   ├── HitboxComponent       # AABB collision and overlap detection
│   ├── HealthComponent       # Hit tracking and death detection
│   ├── InputManager          # Keyboard + XInput gamepad, command binding
│   ├── SceneManager          # Scene creation and lifecycle
│   ├── Scene                 # GameObject container, update/render loop
│   ├── GameObject            # Entity with component attachment (ECS-style)
│   ├── EventQueue            # Observer-based event system
│   └── Sound/                # Service locator, SDL3_mixer (dedicated thread)
└── Game/                     # Game executable
    ├── Scripts/
    │   ├── Level/            # LevelManager, LevelLoader, grid setup
    │   └── Components/       # PengoGridComponent, BlockComponent, state components
    ├── Commands/             # EventCommand, MoveCommand
    ├── States/               # Enemy and player FSM states
    └── Data/                 # Level files, spritesheets, audio
```

**Two CMake targets**: `minigin` (static library) and the game executable. The engine has no knowledge of the game; all game-specific code lives in the game project.

---

## Engine Architecture & Design Choices

### Game Loop

Runs at ~60 FPS. Each frame processes SDL events, dispatches input, updates all scenes, then renders. Delta time is passed down to every component's `Update(float deltaTime)`.

### Entity-Component System

`GameObject` is the entity. Components inherit from a base class and are added via `AddComponent<T>(args...)`. Each `GameObject` holds its components and forwards `Update` and `Render` calls to them. Tags are supported for filtering (e.g. `Tags::Player`, `Tags::Enemy`).

### Command Pattern

Abstract `Command` with `Execute()` decouples input from logic. `InputManager` binds keyboard keys and gamepad buttons to `unique_ptr<Command>` instances. `EventCommand` is the primary command, firing an event ID toward a target state component.

### Observer & Event System

Components expose a `Subject` that other systems can subscribe to as observers. Events carry an ID and an optional integer payload. Used throughout for player hits, enemy kills, egg destruction, and game mode transitions.

### Input

`InputManager` supports both keyboard (SDL3 keycodes) and Xbox gamepad (XInput, Windows only). Commands are bound per key/button with a trigger type: `Pressed`, `Released`, or `Down`. An optional animation group can be passed to coordinate visual state with input.

### Audio

`ServiceLocator` provides global access to `ISoundService` with a `NullSoundService` fallback (used on Emscripten). The concrete `SDLMixerSoundService` runs a dedicated audio thread with a command queue, communicating via `mutex` + `condition_variable`. Call anywhere via:

```cpp
ServiceLocator::GetSoundService().Play("Sounds/MySound.wav", 100);
```

### State Machine

Enemy and player behaviour is driven by state components (`EnemyStateComponent`, `PengoStateComponent`) that hold a current `unique_ptr<State>` and transition on events. States implement `OnEnter`, `OnExit`, and `Update`.

### Collision

`HitboxComponent` does AABB overlap checks each frame against tagged targets. It fires a configurable `Event` on overlap and supports enable/disable for things like enemies inside egg blocks.

---

## Game Features

*(Write about the game)*

---

## Controls

### Keyboard (Player 1)

| Key | Action |
|-----|--------|
| **W / A / S / D** | Move |
| **Arrow keys** | Move (alternative) |
| **Q** | Push ice block |

### Gamepad (Player 2)

| Button | Action |
|--------|--------|
| **D-Pad** | Move |
| **A** | Push ice block |

### Global

| Key | Action |
|-----|--------|
| **1** | Start singleplayer |
| **2** | Start multiplayer |

---

## Patterns Used

| Pattern | Where |
|---------|-------|
| **Component (ECS)** | `GameObject` + component hierarchy |
| **Command** | Input binding: `EventCommand`, `MoveCommand` |
| **Observer** | `Subject` + observer interface across gameplay systems |
| **State** | `EnemyStateComponent`, `PengoStateComponent` |
| **Service Locator** | `ServiceLocator::GetSoundService()` |
| **Null Object** | `NullSoundService` (Emscripten fallback) |
| **Game loop** | |`Minigin`|

---

## Building

### Windows

Open the root folder in Visual Studio or any CMake-aware editor. The project is configured via `CMakeLists.txt` and `CMakePresets.json`.

### Emscripten (Web)

```bash
mkdir build_web
cd build_web
emcmake cmake ..
emmake ninja
python -m http.server
```

Then open `http://localhost:8000`.

---

## Dependencies

- **SDL3** — windowing, input, rendering
- **SDL3_mixer** — audio playback
- **glm** — vector math

All fetched via CMake `FetchContent`.

---

## Source Control

[GitHub Repository](https://github.com/VanermenNiels/Nengine)

## Credits

- **Engine starting point**: [Minigin](https://github.com/avadae/minigin) by Alex Vanden Abeele
- **Patterns reference**: [Game Programming Patterns](https://gameprogrammingpatterns.com/) by Robert Nystrom


## Issue
- The spawning of a new level does not completely work in release mode. The new level spawns in but you won't be able to move.
- It fully works in debug you just have to remove the comments on line 116-118 in InputManager.cpp.
