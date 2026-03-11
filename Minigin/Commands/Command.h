#pragma once
namespace dae
{
	class GameObject;
	class Command
	{
	public:
		Command(GameObject* target) : m_TargetGameObjectRPtr{ target } {};

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;

		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual ~Command() = default;

		virtual void Execute() = 0;
		void SetTargetGO(GameObject* newTarget) { m_TargetGameObjectRPtr = newTarget; }
	protected:
		GameObject* m_TargetGameObjectRPtr;
	};
}