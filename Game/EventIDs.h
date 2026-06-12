#pragma once
#include "Events/Event.h"

namespace dae::EventIDs
{
    inline constexpr EventId PlayerMoved[2] =
    {
        make_sdbm_hash("MovedP1"),
        make_sdbm_hash("MovedP2")
    };

    inline constexpr EventId PlayerStop[2] =
    {
        make_sdbm_hash("StopP1"),
        make_sdbm_hash("StopP2")
    };

    inline constexpr EventId PlayerDied[2] =
    {
        make_sdbm_hash("DiedP1"),
        make_sdbm_hash("DiedP2")
    };
    
    inline constexpr EventId PlayerHit[2] =
    {
        make_sdbm_hash("HitP1"),
        make_sdbm_hash("HitP2")
    };

    inline constexpr EventId IceBlockPushed[2] =
    {
        make_sdbm_hash("PushIceBlockP1"),
        make_sdbm_hash("PushIceBlockP2")
    };
    

    inline constexpr EventId EnemyKilled =
        make_sdbm_hash("EnemyKilled");
    inline constexpr EventId EggDestroyed =
        make_sdbm_hash("EggDestroyed");

    inline constexpr EventId EnemyHit = 
        make_sdbm_hash("EnemyHit");
}