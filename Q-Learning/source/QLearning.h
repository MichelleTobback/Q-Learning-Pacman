#pragma once
#include "QLAgent.h"
#include "QLEnvironment.h"

#include <unordered_map>
#include <random>
#include <fstream>
#include <iostream>

namespace QL
{
	typedef uint32_t State;
	typedef uint32_t Action;

	static State CombineStates(const std::vector<State>& values)
	{
		auto hash{ std::hash<State>{}(values[0]) };
		for (size_t i{ 1 }; i < values.size(); i++)
			hash ^= std::hash<State>{}(values[i]);
		return static_cast<State>(hash);
	}

	struct StateAction_Hash
	{
		std::size_t operator () (const std::pair<State, Action>& stateAction) const
		{
			auto hash1 = std::hash<State>{}(stateAction.first);
			auto hash2 = std::hash<Action>{}(stateAction.second);

			return hash1 ^ hash2;
		}
	};

	template<typename T>
	T BellmanEquation(T state, T alpha, T reward, T gamma, T nextState)
	{
		return state + alpha * (reward + gamma * nextState - state);
	}

	class QTable
	{
	public:
		QTable() = default;
		~QTable() = default;

		inline float GetQValue(State state, Action action)
		{
			return m_QTable[std::make_pair(state, action)];
		}
		inline void SetQValue(State state, Action action, float value)
		{
			m_QTable[std::make_pair(state, action)] = value;
		}
		inline void AddToQValue(State state, Action action, float value)
		{
			m_QTable[std::make_pair(state, action)] += value;
		}
		inline std::vector<Action> GetActions(const State state)
		{
			std::vector<Action> actions;
			for (auto& stateAction : m_QTable)
			{
				if (stateAction.first.first == state)
				{
					actions.push_back(stateAction.first.second);
				}
			}
			return actions;
		}
		inline void SaveToFile(const std::string& path, size_t numTrainedRounds)
		{
			std::ofstream file;
			file.open(path);

			for (auto& q : m_QTable)
			{
				file << "s " << q.first.first << " a " << q.first.second << " v " << q.second << "\n";
			}
			file.close();
		}
		inline void LoadFromFile(const std::string& path)
		{
			m_QTable.clear();

			std::fstream file(path);
			if (!file)
			{
				std::cout << "invalid QTable file!\n";
				return;
			}

			std::string command;
			State state{};
			Action action{};
			float value{};

			while (!file.eof())
			{
				file >> command;

				if (command == "s")
				{
					state = 0;
					file >> state;
				}
				else if (command == "a")
				{
					action = 0;
					file >> action;
				}
				else if (command == "v")
				{
					value = 0;
					file >> value;

					SetQValue(state, action, value);
				}
			}
			file.close();
		}

	private:
		std::unordered_map<std::pair<State, Action>, float, StateAction_Hash> m_QTable;
	};

	class QLearning
	{
	public:
		QLearning(float alpha, float gamma, float epsilon)
			: m_Alpha{ alpha }, m_Gamma{ gamma }, m_Epsilon{ epsilon }, m_QTable{}
		{

		}
		~QLearning() = default;

		// calculate value for action-state using bellman equation
		void Learn(State state, Action action, State nextState, float reward)
		{
			auto stateReward{ m_QTable.GetQValue(state, action) };
			auto nextStateReward{ m_QTable.GetQValue(nextState, action) };

			auto value{ BellmanEquation(stateReward, m_Alpha, reward, m_Gamma, nextStateReward) };
			m_QTable.AddToQValue(state, action, value);
		}

		// select action the agent should perform at current state (at random or by highest value)
		Action SelectAction(State state, size_t numActions)
		{
			// Generate a random number to choose an action
			std::random_device rd;
			std::default_random_engine gen(rd());
			std::uniform_real_distribution<double> dis(0.0, 1.0);
			double randomNumber = dis(gen);

			float maxQValue = GetMaxQValue(state, numActions);
			// Choose the action with the highest Q-value if randomNumber is greater than epsilon
			if (randomNumber > m_Epsilon && maxQValue > FLT_MIN)
			{
				float epsilon = 0.0001f;
				std::vector<Action> maxActions;
				for (Action i{}; i < numActions; i++)
				{
					auto currentQValue{ m_QTable.GetQValue(state, i) };
					if (currentQValue >= FLT_MAX ||
						std::abs(currentQValue - maxQValue) < epsilon)
					{
						maxActions.push_back(i);
					}
				}

				// Choose a random action from the list of actions with the highest Q-value
				std::uniform_int_distribution<size_t> actionDis(0, maxActions.size() - 1);
				size_t actionIndex = actionDis(gen);
				return maxActions[actionIndex];
			}
			// Choose a random action if randomNumber is less than epsilon
			else 
			{
				std::uniform_int_distribution<int> actionDis(0, 3);
				int actionIndex = actionDis(gen);
				return actionIndex;
			}
		}

		float GetMaxQValue(State state, size_t numActions) 
		{
			float maxQValue = FLT_MIN;
			for (Action i{}; i < numActions; i++)
			{
				if (m_QTable.GetQValue(state, i) > maxQValue)
				{
					maxQValue = m_QTable.GetQValue(state, i);
				}
			}
			return maxQValue;
		}

		inline QTable& GetQTable() { return m_QTable; }

	private:
		QTable m_QTable;
		float m_Alpha; // learning rate
		float m_Gamma; // discount rate
		float m_Epsilon; // epsilon greedy strategy
	};
}