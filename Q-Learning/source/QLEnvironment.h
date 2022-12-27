#pragma once

#include <unordered_map>

namespace QL
{
	typedef unsigned int ActionEvent;
	typedef uint32_t State;

	class Environment
	{
	public:
		void AddActionEvent(ActionEvent event, float reward);
		float GetReward(ActionEvent event);

		inline State GetCurrentState() const { return m_CurrentState; }
		inline void SetCurrentState(State state) { m_CurrentState = state; }

	private:
		std::unordered_map<ActionEvent, float> m_ActionEvents;

		State m_CurrentState{};
	};
}
