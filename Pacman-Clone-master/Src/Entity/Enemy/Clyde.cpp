#include "Clyde.h"

#include "../../Tile.h"
#include "../Pacman/Pacman.h"
#include "../../States/GameState/GameState.h"

Clyde::Clyde(sf::Vector2i gridPos, GameState* gameState)
	: Enemy(gridPos, sf::Vector2i(230, 113), gameState, Entities::Clyde)
{
	SetupAnimations();
}

Clyde::~Clyde() {	}


sf::Vector2i Clyde::GetScatterTargetPosition()
{
	return sf::Vector2i(1, 29);
}

sf::Vector2i Clyde::GetChaseTargetPosition()
{
	if (sTile::GetDistanceBetweenTiles(gridPos, gameState->pacman->gridPos) > 8)
		return gameState->pacman->gridPos;
	
	return sf::Vector2i(1, 29);
}

void Clyde::SetupAnimations()
{
	sf::Texture r1, r2, l1, l2;
	r1.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(230, 113, 14, 14));
	r2.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(246, 113, 14, 14));
	l1.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(262, 113, 14, 14));
	l2.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(278, 113, 14, 14));
	std::vector<sf::Texture> leftAnimTextures{ l1,l2 };
	std::vector<sf::Texture> rightAnimTextures{ r1,r2 };

	sf::Texture u1, u2, d1, d2;
	u1.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(294, 113, 14, 14));
	u2.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(310, 113, 14, 14));
	d1.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(326, 113, 14, 14));
	d2.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(342, 113, 14, 14));
	std::vector<sf::Texture> upAnimTextures{ u1,u2 };
	std::vector<sf::Texture> downAnimTextures{ d1,d2 };

	animations[0] = std::make_shared<Animation>(leftAnimTextures, true, 0.1f);
	animations[1] = std::make_shared<Animation>(rightAnimTextures, true, 0.1f);
	animations[2] = std::make_shared<Animation>(upAnimTextures, true, 0.1f);
	animations[3] = std::make_shared<Animation>(downAnimTextures, true, 0.1f);
}