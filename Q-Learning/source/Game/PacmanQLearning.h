#pragma once
#include "../QLearning.h"
#include "Src/States/GameState/GameState.h"

struct sTile;
class Enemy;
class Pacman;
class Controller;

struct PacmanQLearning
{
	enum class ActionType{Left, Right, Up, Down};
	enum class StateType
	{
		None,
		EnemyFront, EnemyBack, EnemyLeft, EnemyRight,
		CoinFront, CoinBack, CoinLeft, CoinRight,
		WallFront, WallBack, WallLeft, WallRight,

		End = WallRight + 1
	};

	enum class ActionEventType
	{
		None, HitByEnemy, PickedCoin, HitWall, Won, Lost
	};

	QL::QLearning ql{0.9f, 0.8f, 0.3f};
	QL::Environment env{};
	QL::Agent agent;

	size_t numTrainedRounds{};
	std::string currentPath;

	bool trainingState{ true };

	void Init(Controller* pController, const std::string& path);
	void Update(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman);

private:
	void InitQTable();
	void InitEnvironment();
	void InitAgent(Controller* pController);

	QL::State ObserveEnvironment(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman);
	QL::State ObserveEnvironmentCombinedStates(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman);
	ActionEventType ReflectAction(ActionType action, sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman);
};


