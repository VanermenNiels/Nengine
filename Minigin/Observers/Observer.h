#pragma once
#include "GameObject.h"
#include "Events/Event.h"

namespace dae
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void onNotify(const GameObject& entity, Event event) = 0;
	};
}