#include "Pinky.h"
#include "../Pacman/Pacman.h"
#include "../../States/GameState/GameState.h"

Pinky::Pinky(sf::Vector2i gridPos, GameState* gameState)
	: Enemy(gridPos, sf::Vector2i(230, 81), gameState, Entities::Pinky)
{
	SetupAnimations();
}

Pinky::~Pinky() {	}

sf::Vector2i Pinky::GetChaseTargetPosition()
{
	sf::Vector2i pos = gameState->pacman->gridPos;
	switch (gameState->pacman->currentDir)
	{
	case Up:
		pos.y -= 4;
		break;
	case Down:
		pos.y += 4;
		break;
	case Left:
		pos.x -= 4;
		break;
	case Right:
		pos.x += 4;
		break;
	}

	return pos;
}

void Pinky::SetupAnimations() 
{
	sf::Texture r1, r2, l1, l2;
	r1.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(230, 81, 14, 14));
	r2.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(246, 81, 14, 14));
	l1.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(262, 81, 14, 14));
	l2.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(278, 81, 14, 14));
	std::vector<sf::Texture> leftAnimTextures{ l1,l2 };
	std::vector<sf::Texture> rightAnimTextures{ r1,r2 };

	sf::Texture u1, u2, d1, d2;
	u1.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(294, 81, 14, 14));
	u2.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(310, 81, 14, 14));
	d1.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(326, 81, 14, 14));
	d2.loadFromFile("Pacman-Clone-master/Resources/PacManSprites.png", sf::IntRect(342, 81, 14, 14));
	std::vector<sf::Texture> upAnimTextures{ u1,u2 };
	std::vector<sf::Texture> downAnimTextures{ d1,d2 };

	animations[0] = std::make_shared<Animation>(leftAnimTextures, true, 0.1f);
	animations[1] = std::make_shared<Animation>(rightAnimTextures, true, 0.1f);
	animations[2] = std::make_shared<Animation>(upAnimTextures, true, 0.1f);
	animations[3] = std::make_shared<Animation>(downAnimTextures, true, 0.1f);
}