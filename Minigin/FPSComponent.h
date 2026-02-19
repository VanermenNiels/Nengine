#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;
	class FPSComponent final : public BaseComponent
	{
	public:
		void Update(float deltaTime) override;
		FPSComponent(GameObject* powner, TextComponent* textComponent);

		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;
	private:
		TextComponent* m_TextComponentRPtr;

		float m_TimeBetweenFrames{0.1f};
		float m_AccumulatedTime{};

		float m_FPS{};
	};
}
