# Q-Learning-Pacman


## Description

### Reinforcement Learning

Reinforcement Learning (RL) is a type of Machine learning that teaches an agent to act on its own in an interactive environment through trial and error.
The agent chooses possible actions after which the environment gives feedback according to what happened by means of a reward (positive values after a good result or negative values as a penalty for a bad action).

### Q-learning

In this project I will explore q learning.
Q-learning is a type of reinforcement learning algorithm that uses a Q-table to store information about the best action to take in a given state. 
It is a model-free algorithm, meaning that it does not require a model of the environment in order to learn.
Unlike model-based algorithms (which has the ability to compute prediction), a model-free algorithm can only make decisions through experience.
Q-learning uses a q-table which is a matrix with as rows, the possible states and the possible actions as columns.
This qTable is updated by means of the Bellman equation which I will explain further in the implementation section.
After the agent has executed some random actions and several values have been filled into the q-table, the agent will choose more and more actions based on the highest q-value at the current state.

### Motivation

Reinforcement learning is a topic that interests me but that I did not have much experience with. 
because of this I found it interesting to delve further into this topic.
In this research I will make use c++ and apply it to a pacman game i found on github (link below).

## Implementation

I started the project by adding a basic controller to the game for which the pacman could be controlled by the agents actions.

My implementation of this algorithm consists of :
* the Environment 
> responsible for rewards on the chosen actions
* the Agent 
> chooses actions and controls the pacman
* Q-table class
> keeps track of the Q-Values
* Q-learning class 
> is responsible for the implementation of the algorithm
* Wrapper struct  
> which brings together the different parts and to change as little as possible of the original game

[//]: # (endlist)

The idea behind the algorithm goes as follows:

![1_mPGk9WTNNvp3i4-9JFgD3w](https://user-images.githubusercontent.com/58373355/212118764-e2bbaac4-3eff-4add-a977-4f4aa50d6ca9.png)

> agent chooses action,
> environment returns a reward,
> environment determines new state of the agent,
> calculate new Q-value,
> Repeat

Then I still had to decide how I was going to implement the possible states and actions.
I decided to keep the q learning code abstract so it could be applied to other games and put everything that was needed specifically for pacman into the wrapper class.
Because of this choice the q table consists of just usnigned ints for the states and actions.
In the wrapper struct, these are structs with possible actions and states.
The states are the positions of the enemies, coins and walls relatively to the agent.

```c++

enum class StateType
	{
		None = 0,
		EnemyFront = 1, EnemyBack = 2, EnemyLeft = 4, EnemyRight = 8,
		CoinFront = 16, CoinBack = 32, CoinLeft = 64, CoinRight = 128,
		WallFront = 256, WallBack = 512, WallLeft = 1024, WallRight = 2048,

		Types = 13
	};

```


For the actions its as simple as:

```c++

enum class ActionType{Left, Right, Up, Down};

```

[//]: # (endlist)

It was quickly apparent that with the current states the agent does not have enough information to learn and make correct decisions.
I needed a way to combine these possible states so that the agent also has states for say, an enemy on the left and a coin above.
For this I chose to make the enum states bitflags which can thus be easily combined.

To start of the algorithm, the Q-Table needs to be initialized to all zeros or loaded from a file with values previously learned.
3 hyperparameters (values 0-1) are used which influence the result of the calculated Q-Values.
* Learning rate (alpha)
* Discount rate (gamma)
* epsilon 

[//]: # (endlist)

```c++

void PacmanQLearning::Update(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman, bool isDead, int score)
{
	QL::Action action{ ql.SelectAction(env.GetCurrentState(), static_cast<size_t>(ActionType::Down) + 1) };
	agent.PerformAction(action);
	auto actionEvent{ ReflectAction(static_cast<ActionType>(action), pTiles, numTilesX, numTilesY, pPacman, isDead, score) };
	float reward{env.GetReward(static_cast<QL::ActionEvent>(actionEvent))};
	auto nextState{ static_cast<QL::State>(ObserveEnvironment(pTiles, numTilesX, numTilesY, pPacman)) };
	ql.Learn(env.GetCurrentState(), action, nextState, reward);
	env.SetCurrentState(nextState);
}

```

In every update one action is chosen, this involves the hyperparameter epsilon.

```c++

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
    
 ```

> If this value is closer to 0, the agent choses its action mostly based on the highest q value.
> The higher the value, the more the action are chosen at random based on the epsilon-greedy strategy which is a method for balancing exploration and exploitation.
> After an action is chosen and executed, the environment returns a reward based on the environments response to this action (e.g. -1 if an enemy is hit or +1 if a 
> snack is eaten).

In this project, the rewards are:
* -0.45 When nothing has happened (to encourage the agent to get to the goal as quickly as possible).
* -4 when hit by an enemy.
* +4 when a snack is eaten.
* -2 when hit by a wall.
* +20 when won (all snacks eaten).
* -10 when lost.
* +10 when the highscore is beaten.

[//]: # (endlist)

After this, the environment determines what the current state is after the chosen action.
> this function is specific to how the pacman game was implemented so i put it in the wrapper struct.
> here I use a naive way to check the surrounding tiles for enemies, snacks and walls.

When these steps are done the Q-Learning calculation (actual learning) happens:

```c++

void Learn(State state, Action action, State nextState, float reward)
		{
			auto stateReward{ m_QTable.GetQValue(state, action) };
			auto nextStateReward{ m_QTable.GetQValue(nextState, action) };

			auto value{ BellmanEquation(stateReward, m_Alpha, reward, m_Gamma, nextStateReward) };
			m_QTable.SetQValue(state, action, value);
		}
    
```

Here it takes the Q-values of the previous state-action pair and the new state-action pair as well as the received reward and the hyperparameters alpha and gamma.
These values are plugged into the bellman equation and added to the current Q value.

```c++

template<typename T>
	T BellmanEquation(T state, T alpha, T reward, T gamma, T nextState)
	{
		return state + alpha * (reward + gamma * nextState - state);
	}
  
```

![1_EQ-tDj-iMdsHlGKUR81Xgw](https://user-images.githubusercontent.com/58373355/212123002-5222db16-ea01-47f3-b9d7-dfc021225d74.png)

The alpha value will determine how fast the agent learns and gamma determines how much future rewards will be taken into account.
these values will be different for every usecase and possible states.
A high learning rate can cause the agent to react too strongly to new information, leading to instability in the Q-values and poor performance.
A low learning rate would make the agent converge on the optimal policy but it would take a lot of time. 
A high discount factor can cause the agent to be too short-sighted, only caring about immediate rewards and not looking for long-term.
It's a trade-off between time and the quality of the policy.

These steps are repeated n times depending on the amount of states and action.
The more states that are possible, the more accurate decisions the agent will be able to make but more episodes will have to be performed.

## Result

### Conclusion
Q-Learning is a fairly simple form of reinforcement learning to apply but has the danger of rapidly requiring too many states for the agent to make correct decisions.
The way that I implemented is still very much tied to the game itself for things such as knowing where there are enemies and snacks or determining the reaction to actions.
After many hours of having the agent trained, I still haven't managed to let the agent finish the game. 
I suspect this is because the agent still does not have enough information about the environment and only knows when an enemy, snack or wall is 1 tile away from the agent (up, down, left, right).
However, there is great improvement in playing the game, for example, the agent goes off mostly on snacks, almost never runs into a wall and most of the time runs away when an enemy is nearby.
The problem occurs when an enemy comes from behind the corner because the agent is not aware of it.

### Possible future improvements
For this application it would be ideal if there were combination of the current states with every possible tile that the agent could run
so that the agent does not always make the same mistakes in the same places and this would solve the problem of enemies coming from behind a corner.
But this would drastically increase states which would also require the agent to train for a much longer time.

Also, another form of reinforcement learning would come in handy for this application such as Deep Q-Learning (DQN).
This is a variant of Q-Learning that uses a deep neural network to approximate the Q-value function instead of a table.
The network takes the state as input and outputs a Q-value for each action. This allows DQN to handle environments with much more states and information.
DQN can also learn from sensory inputs, such as raw pixels from video frames

## Sources
* [Pacman-game source code](https://github.com/NicolasPCouts/Pacman-Clone.git)
* [towardsdatascience](https://towardsdatascience.com/reinforcement-learning-101-e24b50e1d292)
* [RLbook2020.pdf](http://incompleteideas.net/book/RLbook2020.pdf)
* [wikipedia](https://en.wikipedia.org/wiki/Q-learning)
* [Can a robot learn by itself how to clean my appartement? (C++ reinforcement learning)](https://www.youtube.com/watch?v=Xn-8HNuoFaA&t=1187s&ab_channel=ZenSepiol)
