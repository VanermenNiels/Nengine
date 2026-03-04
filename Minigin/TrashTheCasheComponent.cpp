#include "TrashTheCasheComponent.h"
#include "imgui.h"
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace dae;

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};

class GameObject3D
{
public:
	Transform transform;
	int id;
};

TrashTheCasheComponent::TrashTheCasheComponent(GameObject* owner)
	: BaseComponent(owner)
{
}

void TrashTheCasheComponent::Update(float)
{
	if (m_RunIntTest)
	{
		RunIntBenchmark();
		m_RunIntTest = false;
	}

	if (m_RunGameObjectTest)
	{
		RunGameObjectBenchmark();
		m_RunGameObjectTest = false;
	}
}

void TrashTheCasheComponent::Render() const
{
	ImGui::Begin("Exercise 2");

	ImGui::InputInt("Samples", const_cast<int*>(&m_Samples));

	if (ImGui::Button("Thrash the cache with int"))
	{
		const_cast<TrashTheCasheComponent*>(this)->m_RunIntTest = true;
	}

	if (ImGui::Button("Thrash the cache with GameObject3D"))
	{
		const_cast<TrashTheCasheComponent*>(this)->m_RunGameObjectTest = true;
	}

	ImGui::Separator();

	ImGui::BeginChild("GraphRegion", ImVec2(0, 300), true);

	if (!m_IntResults.empty())
	{
		ImGui::Text("Int results");
		ImGui::PlotLines("##intplot",
			m_IntResults.data(),
			static_cast<int>(m_IntResults.size()),
			0,
			nullptr,
			0.0f,
			*m_IntResults.rbegin(),
			ImVec2(0, 150));
	}

	if (!m_GameObjectResults.empty())
	{
		ImGui::Text("GameObject3D results");
		ImGui::PlotLines("##goplot",
			m_GameObjectResults.data(),
			static_cast<int>(m_GameObjectResults.size()),
			0,
			nullptr,
			0.0f,
			*m_GameObjectResults.rbegin(),
			ImVec2(0, 150));
	}

	ImGui::EndChild();
	ImGui::End();
}

void TrashTheCasheComponent::RunIntBenchmark()
{
	m_IntResults.clear();

	const size_t size = static_cast<size_t>(std::pow(2, 26));
	std::vector<int> buffer(size);

	for (int step = 1; step <= 1024; step *= 2)
	{
		std::vector<long long> timings;

		for (int sample = 0; sample < m_Samples; ++sample)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (size_t i = 0; i < buffer.size(); i += step)
				buffer[i] *= 2;

			auto end = std::chrono::high_resolution_clock::now();

			auto duration =
				std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			timings.push_back(duration);
		}

		m_IntResults.push_back(AverageWithoutOutliers(timings));
	}
}

void TrashTheCasheComponent::RunGameObjectBenchmark()
{
	m_GameObjectResults.clear();

	const size_t size = static_cast<size_t>(std::pow(2, 26));
	std::vector<GameObject3D> buffer(size);

	for (int step = 1; step <= 1024; step *= 2)
	{
		std::vector<long long> timings;

		for (int sample = 0; sample < m_Samples; ++sample)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (size_t i = 0; i < buffer.size(); i += step)
				buffer[i].id *= 2;

			auto end = std::chrono::high_resolution_clock::now();

			auto duration =
				std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			timings.push_back(duration);
		}

		m_GameObjectResults.push_back(AverageWithoutOutliers(timings));
	}
}

float TrashTheCasheComponent::AverageWithoutOutliers(std::vector<long long>& timings)
{
	if (timings.size() <= 2)
		return static_cast<float>(timings[0]);

	std::sort(timings.begin(), timings.end());

	// remove lowest & highest
	timings.erase(timings.begin());
	timings.pop_back();

	long long sum = 0;
	for (auto t : timings)
		sum += t;

	return static_cast<float>(sum) / timings.size();
}