#include "PacmanQLearning.h"
#include "Src/Entity/Pacman/Pacman.h"
#include "Src/Entity/Enemy/Enemy.h"
#include "../Game/Controller.h"

#include "Src/Tile.h"

#include <SFML-2.5.1/include/SFML/System/Vector2.hpp>

void PacmanQLearning::Init(Controller* pController, const std::string& path)
{
	currentPath = path;
	InitQTable();
	InitEnvironment();
	InitAgent(pController);
}

void PacmanQLearning::Update(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
	{
		ql.GetQTable().SaveToFile("Q-Learning/source/Saves/QTablePacman.txt", numTrainedRounds);
	}

	QL::Action action{ ql.SelectAction(env.GetCurrentState(), static_cast<size_t>(ActionType::Down) + 1) };
	agent.PerformAction(action);
	auto actionEvent{ ReflectAction(static_cast<ActionType>(action), pTiles, numTilesX, numTilesY, pPacman) };
	float reward{env.GetReward(static_cast<QL::ActionEvent>(actionEvent))};
	auto nextState{ static_cast<QL::State>(ObserveEnvironment(pTiles, numTilesX, numTilesY, pPacman)) };
	//auto nextState{ ObserveEnvironmentCombinedStates(pTiles, numTilesX, numTilesY, pPacman) };
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

	//single states
	//for (QL::State state{}; state < static_cast<QL::State>(StateType::WallRight) + 1; ++state)
	//{
	//	// test
	//	switch (static_cast<StateType>(state))
	//	{
	//		case StateType::CoinBack:
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 100.f);
	//			break;
	//
	//		case StateType::CoinFront:
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 0.f);
	//			break;
	//
	//		case StateType::CoinLeft:
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 0.f);
	//			break;
	//
	//		case StateType::CoinRight:
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 0.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 0.f);
	//			break;
	//
	//		case StateType::EnemyBack:
	//		case StateType::WallBack:
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 50.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 50.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), -100.f);
	//			break;
	//
	//		case StateType::EnemyFront:
	//		case StateType::WallFront:
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 50.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 50.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), -100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 0.f);
	//			break;
	//
	//		case StateType::EnemyLeft:
	//		case StateType::WallLeft:
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), -100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 50.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 50.f);
	//			break;
	//
	//		case StateType::EnemyRight:
	//		case StateType::WallRight:
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), -100.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 50.f);
	//			ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 50.f);
	//			break;
	//	}
	//	//ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Left), 0.f);
	//	//ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Right), 0.f);
	//	//ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Up), 0.f);
	//	//ql.GetQTable().SetQValue(state, static_cast<QL::Action>(ActionType::Down), 0.f);
	//}

	//combined states
	int numStates{};
	ql.GetQTable().SetQValue(QL::CombineStates({ 0 }), static_cast<QL::Action>(ActionType::Left), 0.f);
	ql.GetQTable().SetQValue(QL::CombineStates({ 0 }), static_cast<QL::Action>(ActionType::Right), 0.f);
	ql.GetQTable().SetQValue(QL::CombineStates({ 0 }), static_cast<QL::Action>(ActionType::Up), 0.f);
	ql.GetQTable().SetQValue(QL::CombineStates({ 0 }), static_cast<QL::Action>(ActionType::Down), 0.f);
	for (QL::State state{ static_cast<QL::State>(StateType::EnemyFront) }; state < static_cast<QL::State>(StateType::EnemyRight) + 1; ++state)
	{
		QL::State currentStateA{ QL::CombineStates({ state }) };
		ql.GetQTable().SetQValue(currentStateA, static_cast<QL::Action>(ActionType::Left), 0.f);
		ql.GetQTable().SetQValue(currentStateA, static_cast<QL::Action>(ActionType::Right), 0.f);
		ql.GetQTable().SetQValue(currentStateA, static_cast<QL::Action>(ActionType::Up), 0.f);
		ql.GetQTable().SetQValue(currentStateA, static_cast<QL::Action>(ActionType::Down), 0.f);
		for (QL::State stateB{ static_cast<QL::State>(StateType::CoinFront) }; stateB < static_cast<QL::State>(StateType::CoinRight) + 1; ++stateB)
		{
			QL::State currentStateB{ QL::CombineStates({state, stateB}) };
			ql.GetQTable().SetQValue(currentStateB, static_cast<QL::Action>(ActionType::Left), 0.f);
			ql.GetQTable().SetQValue(currentStateB, static_cast<QL::Action>(ActionType::Right), 0.f);
			ql.GetQTable().SetQValue(currentStateB, static_cast<QL::Action>(ActionType::Up), 0.f);
			ql.GetQTable().SetQValue(currentStateB, static_cast<QL::Action>(ActionType::Down), 0.f);

			for (QL::State stateC{ static_cast<QL::State>(StateType::WallFront) }; stateC < static_cast<QL::State>(StateType::WallRight) + 1; ++stateC)
			{
				QL::State currentStateC{ QL::CombineStates({state, stateB, stateC}) };
				ql.GetQTable().SetQValue(currentStateC, static_cast<QL::Action>(ActionType::Left), 0.f);
				ql.GetQTable().SetQValue(currentStateC, static_cast<QL::Action>(ActionType::Right), 0.f);
				ql.GetQTable().SetQValue(currentStateC, static_cast<QL::Action>(ActionType::Up), 0.f);
				ql.GetQTable().SetQValue(currentStateC, static_cast<QL::Action>(ActionType::Down), 0.f);
			}
		}
	}
}

void PacmanQLearning::InitEnvironment()
{
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::None), -0.45f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::HitByEnemy), -1.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::PickedCoin), 1.f);
	env.AddActionEvent(static_cast<QL::ActionEvent>(ActionEventType::HitWall), -1.f);
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

QL::State PacmanQLearning::ObserveEnvironment(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman)
{
	auto gridPos{ pPacman->gridPos };
	size_t tileIdx{static_cast<size_t>(gridPos.x + numTilesX * gridPos.y)};

	std::vector<QL::State> states;

	sTile* tileUp{ (gridPos.y - 1 >= 0) ? &pTiles[gridPos.x][gridPos.y - 1] : nullptr };
	sTile* tileDown{ (gridPos.y + 1 < numTilesY) ? &pTiles[gridPos.x][gridPos.y + 1] : nullptr };
	sTile* tileLeft{ (gridPos.x - 1 >= 0) ? &pTiles[gridPos.x - 1][gridPos.y] : nullptr };
	sTile* tileRight{ (gridPos.x + 1 < numTilesX) ? &pTiles[gridPos.x + 1][gridPos.y] : nullptr };

	sTile* tileUp2{ (gridPos.y - 2 >= 0) ? &pTiles[gridPos.x][gridPos.y - 2] : nullptr };
	sTile* tileDown2{ (gridPos.y + 2 < numTilesY) ? &pTiles[gridPos.x][gridPos.y + 2] : nullptr };
	sTile* tileLeft2{ (gridPos.x - 2 >= 0) ? &pTiles[gridPos.x - 2][gridPos.y] : nullptr };
	sTile* tileRight2{ (gridPos.x + 2 < numTilesX) ? &pTiles[gridPos.x + 2][gridPos.y] : nullptr };

	// chack for enemies
	if ((tileUp && tileUp->DoesTileHaveType(sTile::Ghost)) || (tileUp2 && tileUp2->DoesTileHaveType(sTile::Ghost)))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::EnemyFront));
	}
	else if ((tileDown && tileDown->DoesTileHaveType(sTile::Ghost)) || (tileDown2 && tileDown2->DoesTileHaveType(sTile::Ghost)))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::EnemyBack));
		//return PacmanQLearning::StateType::EnemyBack;
	}
	else if ((tileRight && tileRight->DoesTileHaveType(sTile::Ghost)) || (tileRight2 && tileRight2->DoesTileHaveType(sTile::Ghost)))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::EnemyRight));
		//return PacmanQLearning::StateType::EnemyRight;
	}
	else if ((tileLeft && tileLeft->DoesTileHaveType(sTile::Ghost)) || (tileLeft2 && tileLeft2->DoesTileHaveType(sTile::Ghost)))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::EnemyLeft));
		//return PacmanQLearning::StateType::EnemyLeft;
	}

	//check for coins
	if (tileUp && tileUp->DoesTileHaveType(sTile::Snack))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::CoinFront));
		//return PacmanQLearning::StateType::CoinFront;
	}
	else if (tileDown && tileDown->DoesTileHaveType(sTile::Snack))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::CoinBack));
		//return PacmanQLearning::StateType::CoinBack;
	}
	else if (tileRight && tileRight->DoesTileHaveType(sTile::Snack))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::CoinRight));
		//return PacmanQLearning::StateType::CoinRight;
	}
	else if (tileLeft && tileLeft->DoesTileHaveType(sTile::Snack))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::CoinLeft));
		//return PacmanQLearning::StateType::CoinLeft;
	}

	//check for wall
	if (tileUp && tileUp->DoesTileHaveType(sTile::Wall))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::WallFront));
		//return PacmanQLearning::StateType::WallFront;
	}
	else if (tileDown && tileDown->DoesTileHaveType(sTile::Wall))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::WallBack));
		//return PacmanQLearning::StateType::WallBack;
	}
	else if (tileRight && tileRight->DoesTileHaveType(sTile::Wall))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::WallRight));
		//return PacmanQLearning::StateType::WallRight;
	}
	else if (tileLeft && tileLeft->DoesTileHaveType(sTile::Wall))
	{
		states.push_back(static_cast<QL::State>(PacmanQLearning::StateType::WallLeft));
		//return PacmanQLearning::StateType::WallLeft;
	}

	if (states.size() == 0)
		return QL::State(0);
	
	return QL::CombineStates(states);
}

//combined states method
QL::State PacmanQLearning::ObserveEnvironmentCombinedStates(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman)
{
	auto gridPos{ pPacman->gridPos };
	size_t tileIdx{ static_cast<size_t>(gridPos.x * numTilesY + gridPos.y) };

	QL::State state{ static_cast<QL::State>(PacmanQLearning::StateType::None) };

	// check for enemies
	if (pTiles[gridPos.x + 1][gridPos.y].DoesTileHaveType(sTile::Ghost))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::EnemyRight);
	}
	else if (pTiles[gridPos.x - 1][gridPos.y].DoesTileHaveType(sTile::Ghost))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::EnemyLeft);
	}
	else if (pTiles[gridPos.x][gridPos.y - 1].DoesTileHaveType(sTile::Ghost))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::EnemyFront);
	}
	else if (pTiles[gridPos.x][gridPos.y + 1].DoesTileHaveType(sTile::Ghost))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::EnemyBack);
	}

	//check for coins
	if (pTiles[gridPos.x + 1][gridPos.y].DoesTileHaveType(sTile::Snack))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::CoinRight);
	}
	else if (pTiles[gridPos.x - 1][gridPos.y].DoesTileHaveType(sTile::Snack))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::CoinLeft);
	}
	else if (pTiles[gridPos.x][gridPos.y - 1].DoesTileHaveType(sTile::Snack))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::CoinFront);
	}
	else if (pTiles[gridPos.x][gridPos.y + 1].DoesTileHaveType(sTile::Snack))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::CoinBack);
	}

	//check for wall
	if (pTiles[gridPos.x + 1][gridPos.y].DoesTileHaveType(sTile::Wall))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::WallRight);
	}
	else if (pTiles[gridPos.x - 1][gridPos.y].DoesTileHaveType(sTile::Wall))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::WallLeft);
	}
	else if (pTiles[gridPos.x][gridPos.y - 1].DoesTileHaveType(sTile::Wall))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::WallFront);
	}
	else if (pTiles[gridPos.x][gridPos.y + 1].DoesTileHaveType(sTile::Wall))
	{
		state |= static_cast<QL::State>(PacmanQLearning::StateType::WallBack);
	}
	return state;
}

PacmanQLearning::ActionEventType PacmanQLearning::ReflectAction(ActionType action, sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman)
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

	sTile* pTile{ (nextGridPos.x >= 0 && nextGridPos.x < numTilesX &&
		nextGridPos.y >= 0 && nextGridPos.y < NumberOfTilesY) ? 
		&pTiles[nextGridPos.x][nextGridPos.y] : nullptr};

	if (pTile && pTile->DoesTileHaveType(sTile::Ghost))
	{
		return PacmanQLearning::ActionEventType::HitByEnemy;
	}
	else if (pTile && pTile->DoesTileHaveType(sTile::Snack))
	{
		return PacmanQLearning::ActionEventType::PickedCoin;
	}
	else if (pTile && pTile->DoesTileHaveType(sTile::Wall))
	{
		return PacmanQLearning::ActionEventType::HitWall;
	}

	return PacmanQLearning::ActionEventType::None;
}
