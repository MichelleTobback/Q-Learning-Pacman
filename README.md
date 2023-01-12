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
-the Environment (responsible for rewards on the chosen actions)
-the Agent (chooses actions and controls the pacman)
-Q-table class
-Q-learning class (is responsible for the implementation of the algorithm)
-Wrapper struct (which brings together the different parts and to change as little as possible of the original game)

Then I still had to decide how I was going to implement the possible states and actions.
I decided to keep the q learning code abstract so it could be applied to other games and put everything that was needed specifically for pacman into the wrapper class.
Because of this choice the q table consists of just usnigned int for the states and actions.
In the wrapper struct, these are structs with possible actions and states.
The states are the positions of the enemies, coins and walls relatively to the agent.
For the actions its as simple as:
* Up
* Down
* Left
* Right
It was quickly apparent that with the current states the agent does not have enough information to learn and make correct decisions.
I needed a way to combine these possible states so that the agent also has states for say, an enemy on the left and a coin above.
For this I chose to make the enum states bitflags which can thus be easily combined.

To start of the algorithm, the Q-Table needs to be initialized to all zeros or loaded from a file with values previously learned.
3 hyperparameters (values 0-1) are used which influence the result of the calculated Q-Values.
* Learning rate (alpha)
* Discount rate (gamma)
* epsilon
In every update one action is chosen, this involves the hyperparameter epsilon.
If this value is closer to 0, the agent choses its action mostly based on the highest q value.
The higher the value, the more the action are chosen at random based on the epsilon-greedy strategy which is a method for balancing exploration and exploitation.
After an action is chosen and executed, the environment returns a reward based on the environments response to this action (e.g. -1 if an enemy is hit or +1 if a snack is eaten).
In this project, the rewards are:
* -0.45 When nothing has happened (to encourage the agent to get to the goal as quickly as possible).
* -4 when hit by an enemy.
* +4 when a snack is eaten.
* -2 when hit by a wall.
* +20 when won (all snacks eaten).
* -10 when lost.
* +10 when the highscore is beaten.
After this, the environment determines what the current state is after the chosen action.

When these steps are done the Q-Learning calculation (actual learning) happens:
Here it takes the Q-values of the previous state-action pair and the new state-action pair as well as the received reward and the hyperparameters alpha and gamma.
These values are plugged into the bellman equation and added to the current Q value.
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

