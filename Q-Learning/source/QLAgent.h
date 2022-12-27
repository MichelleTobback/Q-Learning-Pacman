#pragma once
#include <unordered_map>
#include <functional>

namespace QL
{
	typedef uint32_t State;
	typedef uint32_t Action;

	class Agent
	{
	public:
		Agent();

		void PerformAction(Action action);
		void AddAction(Action action, std::function<void()> controllerAction);

	private:
		std::unordered_map<Action, std::function<void()>> m_ActionControllerMappings;
	};
}
