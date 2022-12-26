#include "Controller.h"
#include <../Pacman-Clone-master/SFML-2.5.1/include/SFML/Window/Keyboard.hpp>

void Controller::Update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		m_MoveUp();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		m_MoveDown();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		m_MoveLeft();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_MoveRight();
}
