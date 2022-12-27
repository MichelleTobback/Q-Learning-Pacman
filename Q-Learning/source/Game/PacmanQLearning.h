#pragma once
#include "../QLearning.h"

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
		WallFront, WallBack, WallLeft, WallRight
	};

	enum class ActionEventType
	{
		None, HitByEnemy, PickedCoin, HitWall, Won, Lost
	};

	QL::QLearning ql{0.1f, 0.9f, 0.2f};
	QL::Environment env{};
	QL::Agent agent;

	void Init(Controller* pController);
	void Update(sTile* pTiles, int numTilesX, int numTilesY, Pacman* pPacman);

private:
	void InitQTable();
	void InitEnvironment();
	void InitAgent(Controller* pController);

	StateType ObserveEnvironment(sTile* pTiles, int numTilesX, int numTilesY, Pacman* pPacman);
	ActionEventType ReflectAction(ActionType action, sTile* pTiles, int numTilesX, int numTilesY, Pacman* pPacman);
};


