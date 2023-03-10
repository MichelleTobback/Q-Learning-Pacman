#pragma once
#include "Enemy.h"

class Blinky : public Enemy
{
public:
	Blinky(sf::Vector2i gridPos, GameState* gameState);
	virtual ~Blinky() override;
private:
	sf::Vector2i GetScatterTargetPosition() override;
	void SetupAnimations() override;
};

