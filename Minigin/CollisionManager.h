#pragma once
#include <vector>
#include "Singleton.h"

namespace dae
{
	class HitboxComponent;

	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void RegisterHitbox(HitboxComponent* hitbox);
		void UnregisterHitbox(HitboxComponent* hitbox);

		void Update();

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager(CollisionManager&&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;
		CollisionManager& operator=(CollisionManager&&) = delete;


		std::vector<HitboxComponent*> m_Hitboxes{};
	};
}