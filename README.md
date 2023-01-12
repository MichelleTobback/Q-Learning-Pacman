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
-Up
-Down
-Left
-Right
It was quickly apparent that with the current states the agent does not have enough information to learn and make correct decisions.
I needed a way to combine these possible states so that the agent also has states for say, an enemy on the left and a coin above.
For this I chose to make the enum states bitflags which can thus be easily combined.

To start of the algorithm, the Q-Table needs to be initialized to all zeros or loaded from a file with values previously learned.

## Result

### Conclusion

### Remarks
