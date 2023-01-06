#include "TrainingState.h"

#include "Src/Entity/Pacman/Pacman.h"
#include "Src/Entity/Enemy/Enemy.h"
#include "Src/Entity/Enemy/Blinky.h"
#include "Src/Entity/Enemy/Pinky.h"
#include "Src/Entity/Enemy/Inky.h"
#include "Src/Entity/Enemy/Clyde.h"
#include "Src/Entity/Snack/Snack.h"

#include "../Q-Learning/source/Game/PacmanQLearning.h"

TrainingState::TrainingState(sf::RenderWindow* window, std::stack<State*>* states, GameManager* gameManager)
    : GameState(window, states, gameManager)
{
}

TrainingState::~TrainingState()
{
    
}

void TrainingState::Update(const float& deltaTime)
{
    constexpr const size_t numEpisodes{ 1000 };
    if (m_QLearn->numTrainedRounds == numEpisodes)
    {
        m_QLearn->ql.GetQTable().SaveToFile(m_QLearn->currentPath, m_QLearn->numTrainedRounds);
        states->pop();
        return;
    }

    for (int i{}; i < numEpisodes; i++)
    {
        if (isPacmanDead)
        {
            std::cout << "lost, score: " << score << "\n";
            std::cout << "episode: " << m_QLearn->numTrainedRounds << '\n';
            Restart();
        }

        m_QLearn->Update(tileArray, NumberOfTilesX, NumberOfTilesY, pacman);
        pacman->Update(deltaTime);

        for (auto const& x : enemys)
        {
            if (x != nullptr)
                x->Update(deltaTime);
        }

        for (auto const& x : SnackList)
        {
            x->Update(deltaTime);
        }

        Draw();
    }
}

void TrainingState::Draw()
{
    window->clear();
    window->draw(mapSprite);

    for (auto const& x : SnackList)
        x->Draw(*window);

    pacman->Draw(*window);

    for (auto const& x : enemys)
    {
        if (x != NULL)
            x->Draw(*window);
    }

    window->display();
}

void TrainingState::CreateMapCollidersAndSnacks()
{
    int mapDesign[NumberOfTilesY][NumberOfTilesX] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,3,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,3,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,5,1,1,5,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,5,1,1,5,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,5,5,5,5,5,5,5,5,5,5,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,5,1,1,1,2,2,1,1,1,5,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,5,1,2,2,2,2,2,2,1,5,1,1,0,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,5,5,5,1,2,2,2,2,2,2,1,5,5,5,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,0,1,1,5,1,2,2,2,2,2,2,1,5,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,5,1,1,1,1,1,1,1,1,5,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,5,5,5,5,5,5,5,5,5,5,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,5,1,1,1,1,1,1,1,1,5,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,5,1,1,1,1,1,1,1,1,5,1,1,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,3,0,0,1,1,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,1,1,0,0,3,1},
    {1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1},
    {1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int y = 0; y < NumberOfTilesY; y++)
    {
        for (int x = 0; x < NumberOfTilesX; x++)
        {
            if (mapDesign[y][x] == 0) // small snack
            {
                tileArray[x][y].isEmpty = false;
                tileArray[x][y].tileTypes.push_back(sTile::Snack);
                Snack* s = new Snack(Snack::SmallSnack, sf::Vector2i(x, y), this);
                SnackList.push_back(s);
            }
            else if (mapDesign[y][x] == 1) // wall collider
            {
                tileArray[x][y].isEmpty = false;
                tileArray[x][y].tileTypes.push_back(sTile::Wall);
            }
            else if (mapDesign[y][x] == 2) // ghost house
            {
                tileArray[x][y].isEmpty = false;
                tileArray[x][y].tileTypes.push_back(sTile::GhostHouse);
            }
            else if (mapDesign[y][x] == 3) // big snack
            {
                tileArray[x][y].isEmpty = false;
                tileArray[x][y].tileTypes.push_back(sTile::Snack);
                Snack* s = new Snack(Snack::BigSnack, sf::Vector2i(x, y), this);
                SnackList.push_back(s);
            }
            else if (mapDesign[y][x] == 5) // empty
            {
            }
        }
    }
}

void TrainingState::Restart()
{
    score = 0;
    DeleteSnacks();
    EmptyTileArray();
    CreateMapCollidersAndSnacks();

    for (auto& x : enemys)
    {
        delete x;
        x = nullptr;
    }
    delete pacman;
    pacman = nullptr;

    isPacmanDead = false;
    ++m_QLearn->numTrainedRounds;

    CreatePacmanAndEnemys();
}
