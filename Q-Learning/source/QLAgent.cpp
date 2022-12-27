#include "QLAgent.h"

namespace QL
{
    Agent::Agent()
    {
       
    }

    void Agent::PerformAction(Action action)
    {
        m_ActionControllerMappings[action]();
    }

    void Agent::AddAction(Action action, std::function<void()> controllerAction)
    {
        m_ActionControllerMappings[action] = controllerAction;
    }
}