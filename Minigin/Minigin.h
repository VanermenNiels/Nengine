#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include <chrono>

namespace dae
{
	class Minigin final
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath, int maxFrameRate = 60);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame(std::chrono::steady_clock::time_point& lastTime);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		bool m_quit{};
		float m_MillisecondsBetweenFrames{};
	};
}