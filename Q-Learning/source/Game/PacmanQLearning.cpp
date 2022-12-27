#include "PacmanQLearning.h"
#include "Src/Entity/Pacman/Pacman.h"
#include "Src/Entity/Enemy/Enemy.h"
#include "../Game/Controller.h"

#include "Src/Tile.h"

#include <SFML-2.5.1/include/SFML/System/Vector2.hpp>

void PacmanQLearning::Init(Controller* pController)
{
	InitQTable();
	InitEnvironment();
	InitAgent(pController);
}

void PacmanQLearning::Update(sTile* pTiles, int numTilesX, int numTilesY, Pacman* pPacman)
{
	QL::Action action{ ql.SelectAction(env.GetCurrentState(), static_cast<size_t>(ActionType::Down) + 1) };
	agent.PerformAction(action);
	auto actionEvent{ ReflectAction(static_cast<ActionType>(action), pTiles, numTilesX, numTilesY, pPacman) };
	float reward{env.GetReward(static_cast<QL::ActionEvent>(actionEvent))};
	auto nextState{ static_cast<QL::State>(ObserveEnvironment(pTiles, numTilesX, numTilesY, pPacman)) };
	ql.Learn(env.GetCurrentState(), action, nextState, reward);
	env.SetCurrentState(nextState);
}

void PacmanQLearning::InitQTable()
{
	for (QL::State state{}; state < static_cast<QL::State>(StateType::WallRight) + 1; ++state)
	{
		ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 0.f);
		ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 0.f);
		ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 0.f);
		ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 0.f);
	}
}

void PacmanQLearning::InitEnvironment()
{
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::None), -0.1f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::HitByEnemy), -6.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::PickedCoin), 3.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::HitWall), -0.1f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::Won), 20.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::Lost), -10.f);
}

void PacmanQLearning::InitAgent(Controller* pController)
{
	agent.AddAction(static_cast<QL::Action>(ActionType::Left), pController->m_MoveLeft);
	agent.AddAction(static_cast<QL::Action>(ActionType::Right), pController->m_MoveRight);
	agent.AddAction(static_cast<QL::Action>(ActionType::Up), pController->m_MoveUp);
	agent.AddAction(static_cast<QL::Action>(ActionType::Down), pController->m_MoveDown);
}

PacmanQLearning::StateType PacmanQLearning::ObserveEnvironment(sTile* pTiles, int numTilesX, int numTilesY, Pacman* pPacman)
{
	auto gridPos{ pPacman->gridPos };
	size_t tileIdx{static_cast<size_t>(gridPos.x * numTilesX + gridPos.y)};

	// chack for enemies
	if (pTiles[tileIdx + 1].DoesTileHaveType(sTile::Ghost))
	{
		return PacmanQLearning::StateType::EnemyRight;
	}
	else if (pTiles[tileIdx - 1].DoesTileHaveType(sTile::Ghost))
	{
		return PacmanQLearning::StateType::EnemyLeft;
	}
	else if (pTiles[tileIdx + numTilesX].DoesTileHaveType(sTile::Ghost))
	{
		return PacmanQLearning::StateType::EnemyFront;
	}
	else if (pTiles[tileIdx - numTilesX].DoesTileHaveType(sTile::Ghost))
	{
		return PacmanQLearning::StateType::EnemyBack;
	}
	//check for wall
	else if (pTiles[tileIdx + 1].DoesTileHaveType(sTile::Wall))
	{
		return PacmanQLearning::StateType::WallRight;
	}
	else if (pTiles[tileIdx - 1].DoesTileHaveType(sTile::Wall))
	{
		return PacmanQLearning::StateType::WallLeft;
	}
	else if (pTiles[tileIdx + numTilesX].DoesTileHaveType(sTile::Wall))
	{
		return PacmanQLearning::StateType::WallFront;
	}
	else if (pTiles[tileIdx - numTilesX].DoesTileHaveType(sTile::Wall))
	{
		return PacmanQLearning::StateType::WallBack;
	}
	//check for coins
	else if (pTiles[tileIdx + 1].DoesTileHaveType(sTile::Snack))
	{
		return PacmanQLearning::StateType::CoinRight;
	}
	else if (pTiles[tileIdx - 1].DoesTileHaveType(sTile::Snack))
	{
		return PacmanQLearning::StateType::CoinLeft;
	}
	else if (pTiles[tileIdx + numTilesX].DoesTileHaveType(sTile::Snack))
	{
		return PacmanQLearning::StateType::CoinFront;
	}
	else if (pTiles[tileIdx - numTilesX].DoesTileHaveType(sTile::Snack))
	{
		return PacmanQLearning::StateType::CoinBack;
	}

	return PacmanQLearning::StateType::None;
}

PacmanQLearning::ActionEventType PacmanQLearning::ReflectAction(ActionType action, sTile* pTiles, int numTilesX, int numTilesY, Pacman* pPacman)
{
	sf::Vector2i nextGridPos{  };
	switch (action)
	{
	case ActionType::Left:
		nextGridPos = pPacman->gridPos + sf::Vector2i{ 0, -1 };
		break;

	case ActionType::Right:
		nextGridPos = pPacman->gridPos + sf::Vector2i{ 0, 1 };
		break;

	case ActionType::Up:
		nextGridPos = pPacman->gridPos + sf::Vector2i{ 1, 0 };
		break;

	case ActionType::Down:
		nextGridPos = pPacman->gridPos + sf::Vector2i{ -1, 0 };
		break;
	}

	size_t gridIdx{ static_cast<size_t>(numTilesX * nextGridPos.x + nextGridPos.y) };

	if (pTiles[gridIdx].DoesTileHaveType(sTile::Ghost))
	{
		return PacmanQLearning::ActionEventType::HitByEnemy;
	}
	else if (pTiles[gridIdx].DoesTileHaveType(sTile::Snack))
	{
		return PacmanQLearning::ActionEventType::PickedCoin;
	}
	else if (pTiles[gridIdx].DoesTileHaveType(sTile::Wall))
	{
		return PacmanQLearning::ActionEventType::HitWall;
	}

	return PacmanQLearning::ActionEventType::None;
}
