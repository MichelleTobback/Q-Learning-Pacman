#pragma once
#include "Enemy.h"

class Inky : public Enemy
{
public:
	Inky(sf::Vector2i gridPos, GameState* gameState);
	virtual ~Inky() override;
private:
	sf::Vector2i GetScatterTargetPosition() override;
	sf::Vector2i GetChaseTargetPosition() override;
	void SetupAnimations() override;
};

