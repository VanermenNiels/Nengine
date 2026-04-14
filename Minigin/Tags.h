#pragma once
#include <cstdint>

namespace dae::Tags
{
    constexpr uint32_t None       = 0;
    constexpr uint32_t Player     = 1 << 0;
    constexpr uint32_t Enemy      = 1 << 1;
    constexpr uint32_t Projectile = 1 << 2;
    constexpr uint32_t Pickup     = 1 << 3;
}