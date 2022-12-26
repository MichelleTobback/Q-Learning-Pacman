#pragma once
#include "Enemy.h"

class Pinky : public Enemy
{
public:
	Pinky(sf::Vector2i gridPos, GameState* gameState);
	virtual ~Pinky() override;
private:
	sf::Vector2i GetChaseTargetPosition() override;
	void SetupAnimations() override;
};


