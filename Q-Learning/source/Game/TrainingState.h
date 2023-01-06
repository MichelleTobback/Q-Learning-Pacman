#pragma once

#include "Src/States/GameState/GameState.h"

class TrainingState : public GameState
{
public:
	TrainingState(sf::RenderWindow* window, std::stack<State*>* states, GameManager* gameManager);
	virtual ~TrainingState() override;
	virtual void Update(const float& deltaTime) override;
	virtual void Draw() override;

private:
	void CreateMapCollidersAndSnacks();
	void Restart();
};