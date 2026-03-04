#pragma once
#include "BaseComponent.h"
#include <vector>

namespace dae
{
	class TrashTheCasheComponent final : public BaseComponent
	{
	public:
		TrashTheCasheComponent(GameObject* owner);
		virtual ~TrashTheCasheComponent() = default;

		void Update(float deltaTime) override;
		void Render() const override;

	private:

		// ===== Benchmark control =====
		int m_Samples{ 100 };
		bool m_RunIntTest{ false };
		bool m_RunGameObjectTest{ false };

		// ===== Results =====
		std::vector<float> m_IntResults;
		std::vector<float> m_GameObjectResults;

		// ===== Exercise Runners =====
		void RunIntBenchmark();
		void RunGameObjectBenchmark();

		// ===== Helpers =====
		float AverageWithoutOutliers(std::vector<long long>& timings);
	};
}