#include "QLEnvironment.h"

namespace QL
{
	void Environment::AddActionEvent(ActionEvent event, float reward)
	{
		m_ActionEvents[event] = reward;
	}

	float Environment::GetReward(ActionEvent event)
	{
		return m_ActionEvents[event];
	}
}