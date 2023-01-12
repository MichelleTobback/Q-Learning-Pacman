#pragma once
#include "../QLearning.h"
#include "Src/States/GameState/GameState.h"

struct sTile;
class Enemy;
class Pacman;
class Controller;

#define TO_STATE(state) static_cast<QL::State>(state)

struct PacmanQLearning
{
	enum class ActionType{Left, Right, Up, Down};
	enum class StateType
	{
		None = 0,
		EnemyFront = 1, EnemyBack = 2, EnemyLeft = 4, EnemyRight = 8,
		CoinFront = 16, CoinBack = 32, CoinLeft = 64, CoinRight = 128,
		WallFront = 256, WallBack = 512, WallLeft = 1024, WallRight = 2048,

		Types = 13
	};

	enum class ActionEventType
	{
		None, HitByEnemy, PickedCoin, HitWall, Won, Lost, HighscoreBeaten
	};

	QL::QLearning ql{0.4f, 0.f, 0.8f};
	QL::Environment env{};
	QL::Agent agent;

	size_t numTrainedRounds{};
	std::string currentPath;

	bool trainingState{ false };

	int highscore{};

	void Init(Controller* pController, const std::string& path);
	void Update(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman, bool isDead, int score);

private:
	void InitQTable();
	void InitEnvironment();
	void InitAgent(Controller* pController);

	QL::State ObserveEnvironment(sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman);
	ActionEventType ReflectAction(ActionType action, sTile pTiles[NumberOfTilesX][NumberOfTilesY], int numTilesX, int numTilesY, Pacman* pPacman, bool isDead, int score);
};


