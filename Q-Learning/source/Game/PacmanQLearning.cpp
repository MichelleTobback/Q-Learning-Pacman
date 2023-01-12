#include "PacmanQLearning.h"
#include "Src/Entity/Pacman/Pacman.h"
#include "Src/Entity/Enemy/Enemy.h"
#include "../Game/Controller.h"

#include "Src/Tile.h"

#include <SFML-2.5.1/include/SFML/System/Vector2.hpp>

void PacmanQLearning::Init(Controller* pController, const std::string& path)
{
	currentPath = path;

	if (trainingState)
	{
		ql = QL::QLearning(0.3f, 0.9f, 0.2f);
	}
	else
	{
		ql = QL::QLearning(0.f, 0.9f, 0.f);
	}

	InitQTable();
	InitEnvironment();
	InitAgent(pController);
}

void PacmanQLearning::Update(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman, bool isDead, int score)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
	{
		ql.GetQTable().SaveToFile("Q-Learning/source/Saves/QTablePacman.txt", numTrainedRounds);
	}

	QL::Action action{ ql.SelectAction(env.GetCurrentState(), static_cast<size_t>(ActionType::Down) + 1) };
	agent.PerformAction(action);
	auto actionEvent{ ReflectAction(static_cast<ActionType>(action), pTiles, numTilesX, numTilesY, pPacman, isDead, score) };
	float reward{env.GetReward(static_cast<QL::ActionEvent>(actionEvent))};
	auto nextState{ static_cast<QL::State>(ObserveEnvironment(pTiles, numTilesX, numTilesY, pPacman)) };
	ql.Learn(env.GetCurrentState(), action, nextState, reward);
	env.SetCurrentState(nextState);
}

void PacmanQLearning::InitQTable()
{
	if (!currentPath.empty())
	{
		ql.GetQTable().LoadFromFile(currentPath);
		return;
	}

	//combined states
	size_t numStates{ static_cast<size_t>(StateType::Types) };
	for (size_t i{0}; i < numStates; ++i)
	{
		QL::State stateI{ TO_STATE(1 << i) };
		ql.GetQTable().SetQValue(stateI, static_cast<QL::Action>(ActionType::Left), 0.f);
		ql.GetQTable().SetQValue(stateI, static_cast<QL::Action>(ActionType::Right), 0.f);
		ql.GetQTable().SetQValue(stateI, static_cast<QL::Action>(ActionType::Up), 0.f);
		ql.GetQTable().SetQValue(stateI, static_cast<QL::Action>(ActionType::Down), 0.f);

		for (size_t j{ i + 1 }; j < numStates; ++j)
		{
			QL::State statej{ stateI | TO_STATE(1 << j) };
			ql.GetQTable().SetQValue(statej, static_cast<QL::Action>(ActionType::Left), 0.f);
			ql.GetQTable().SetQValue(statej, static_cast<QL::Action>(ActionType::Right), 0.f);
			ql.GetQTable().SetQValue(statej, static_cast<QL::Action>(ActionType::Up), 0.f);
			ql.GetQTable().SetQValue(statej, static_cast<QL::Action>(ActionType::Down), 0.f);
		}
	}
}

void PacmanQLearning::InitEnvironment()
{
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::None), -0.45f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::HitByEnemy), -4.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::PickedCoin), 4.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::HitWall), -2.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::Won), 20.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::Lost), -10.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::HighscoreBeaten), 10.f);
}

void PacmanQLearning::InitAgent(Controller* pController)
{
	agent.AddAction(static_cast<QL::Action>(ActionType::Left), pController->m_MoveLeft);
	agent.AddAction(static_cast<QL::Action>(ActionType::Right), pController->m_MoveRight);
	agent.AddAction(static_cast<QL::Action>(ActionType::Up), pController->m_MoveUp);
	agent.AddAction(static_cast<QL::Action>(ActionType::Down), pController->m_MoveDown);
}

QL::State PacmanQLearning::ObserveEnvironment(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman)
{
	auto gridPos{ pPacman->gridPos };
	size_t tileIdx{static_cast<size_t>(gridPos.x + numTilesX * gridPos.y)};

	std::vector<QL::State> states;

	sTile* tileUp{ (gridPos.y - 1 >= 0) ? &pTiles[gridPos.x][gridPos.y - 1] : nullptr };
	sTile* tileDown{ (gridPos.y + 1 < numTilesY) ? &pTiles[gridPos.x][gridPos.y + 1] : nullptr };
	sTile* tileLeft{ (gridPos.x - 1 >= 0) ? &pTiles[gridPos.x - 1][gridPos.y] : nullptr };
	sTile* tileRight{ (gridPos.x + 1 < numTilesX) ? &pTiles[gridPos.x + 1][gridPos.y] : nullptr };

	sTile* tile2Up{ (gridPos.y - 2 >= 0) ? &pTiles[gridPos.x][gridPos.y - 2] : nullptr };
	sTile* tile2Down{ (gridPos.y + 2 < numTilesY) ? &pTiles[gridPos.x][gridPos.y + 2] : nullptr };
	sTile* tile2Left{ (gridPos.x - 2 >= 0) ? &pTiles[gridPos.x - 2][gridPos.y] : nullptr };
	sTile* tile2Right{ (gridPos.x + 2 < numTilesX) ? &pTiles[gridPos.x + 2][gridPos.y] : nullptr };

	bool left{ false }, right{ false }, up{ false }, down{ false };

	// check for enemies
	if ((tileUp && tileUp->DoesTileHaveType(sTile::Ghost))
		|| (tile2Up && tile2Up->DoesTileHaveType(sTile::Ghost)
			&& !tileUp->DoesTileHaveType(sTile::Wall)))
	{
		up = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::EnemyFront));
	}
	if ((tileDown && tileDown->DoesTileHaveType(sTile::Ghost))
		|| (tile2Down && tile2Down->DoesTileHaveType(sTile::Ghost)
			&& !tileDown->DoesTileHaveType(sTile::Wall)))
	{
		down = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::EnemyBack));
	}
	if ((tileRight && tileRight->DoesTileHaveType(sTile::Ghost))
		|| (tile2Right && tile2Right->DoesTileHaveType(sTile::Ghost)
			&& !tileRight->DoesTileHaveType(sTile::Wall)))
	{
		right = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::EnemyRight));
	}
	if ((tileLeft && tileLeft->DoesTileHaveType(sTile::Ghost))
		|| (tile2Left && tile2Left->DoesTileHaveType(sTile::Ghost)
			&& !tileLeft->DoesTileHaveType(sTile::Wall)))
	{
		left = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::EnemyLeft));
	}

	//check for coins
	if (!up && tileUp && tileUp->DoesTileHaveType(sTile::Snack))
	{
		up = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::CoinFront));
	}
	if (!down && tileDown && tileDown->DoesTileHaveType(sTile::Snack))
	{
		down = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::CoinBack));
	}
	if (!right && tileRight && tileRight->DoesTileHaveType(sTile::Snack))
	{
		right = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::CoinRight));
	}
	if (!left && tileLeft && tileLeft->DoesTileHaveType(sTile::Snack))
	{
		left = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::CoinLeft));
	}

	//check for wall
	if (!up && tileUp && tileUp->DoesTileHaveType(sTile::Wall))
	{
		up = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::WallFront));
	}
	if (!down && tileDown && tileDown->DoesTileHaveType(sTile::Wall))
	{
		down = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::WallBack));
	}
	if (!right && tileRight && tileRight->DoesTileHaveType(sTile::Wall))
	{
		right = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::WallRight));
	}
	if (!left && tileLeft && tileLeft->DoesTileHaveType(sTile::Wall))
	{
		left = true;
		states.push_back(TO_STATE(PacmanQLearning::StateType::WallLeft));
	}

	if (states.size() > 4)
		states.resize(4);

	else if (states.size() == 0)
		return QL::State(0);
	
	return QL::CombineStates(states);
}

PacmanQLearning::ActionEventType PacmanQLearning::ReflectAction(ActionType action, sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman, bool isDead, int score)
{
	if (isDead)
		return PacmanQLearning::ActionEventType::Lost;

	sf::Vector2i newGridPos{  };
	sf::Vector2i nextGridPos{  };
	switch (action)
	{
	case ActionType::Left:
		newGridPos = pPacman->gridPos + sf::Vector2i{ -1, 0 };
		nextGridPos = pPacman->gridPos + sf::Vector2i{ -2, 0 };
		break;

	case ActionType::Right:
		newGridPos = pPacman->gridPos + sf::Vector2i{ 1, 0 };
		nextGridPos = pPacman->gridPos + sf::Vector2i{ 2, 0 };
		break;

	case ActionType::Up:
		newGridPos = pPacman->gridPos + sf::Vector2i{ 0, -1 };
		nextGridPos = pPacman->gridPos + sf::Vector2i{ 0, -2 };
		break;

	case ActionType::Down:
		newGridPos = pPacman->gridPos + sf::Vector2i{ 0, 1 };
		nextGridPos = pPacman->gridPos + sf::Vector2i{ 0, 2 };
		break;
	}
	sTile* pCurrentPos{ &pTiles[pPacman->gridPos.x][pPacman->gridPos.y] };
	sTile* pTile{ (newGridPos.x >= 0 && newGridPos.x < numTilesX &&
		newGridPos.y >= 0 && newGridPos.y < NumberOfTilesY) ? 
		&pTiles[newGridPos.x][newGridPos.y] : nullptr};
	sTile* nextGridTile{ (nextGridPos.x >= 0 && nextGridPos.x < numTilesX &&
		nextGridPos.y >= 0 && nextGridPos.y < NumberOfTilesY) ?
		&pTiles[nextGridPos.x][nextGridPos.y] : nullptr };

	if ((pCurrentPos->DoesTileHaveType(sTile::Ghost))
		|| (pTile && pTile->DoesTileHaveType(sTile::Ghost))
		|| (nextGridTile && nextGridTile->DoesTileHaveType(sTile::Ghost) && !pTile->DoesTileHaveType(sTile::Wall)))
	{
		return PacmanQLearning::ActionEventType::HitByEnemy;
	}
	else if (pTile && pTile->DoesTileHaveType(sTile::Snack))
	{
		if (score + 10 > highscore)
		{
			highscore = score + 10;
			return PacmanQLearning::ActionEventType::HighscoreBeaten;
		}

		return PacmanQLearning::ActionEventType::PickedCoin;
	}
	else if (pTile && pTile->DoesTileHaveType(sTile::Wall))
	{
		return PacmanQLearning::ActionEventType::HitWall;
	}

	return PacmanQLearning::ActionEventType::None;
}
