#include "master.h"

bool GameCode::playerHasHands() {return User.getRight() || User.getLeft() != 0; }
bool GameCode::cpuHasHands() {return CPU.getRight() || CPU.getLeft() != 0; }

void GameCode::start()
{
    while (gameRunning)
    {
        gameLoop();
    }
}

void GameCode::gameLoop()
{
    Player Player;
    Enemy Enemy;
    cout << "New Game, Player goes first: " << endl << endl;
    while (gameRunning)
    {
        cout << "Your hand(s):               Opponent hand(s):" << endl;
        cout << "L:" << Player.getLeft() << " R:" << Player.getRight() << "                     " << "L:" << Enemy.getLeft() << " R:" << Enemy.getRight() << endl << endl;
        if (playerHasHands() && cpuHasHands())
        {
        User.playerTurn();
        if (Enemy.getLeft() == maxHandValue)
        {
            cout << "Enemy Left Hand Eliminated!" << endl;
            Enemy.setLeft(0);
        }
        else if (Enemy.getRight() == maxHandValue)
        {
            cout << "Enemy Right Hand Eliminated!" << endl;
            Enemy.setRight(0);
        }
        else if (!cpuHasHands())
        {
            cout << "Player Wins!" << endl;
            return;
        }
        }


        Enemy.cpuTurn();

        if (Player.getLeft() == maxHandValue)
        {
            cout << "Your Left Hand Has Been Eliminated!" << endl;
            Player.setLeft(0);
        }
        else if (Player.getRight() == maxHandValue)
        {
            cout << "Your Right Hand Has Been Eliminated!" << endl;
            Player.setRight(0);
        }
        if (!playerHasHands())
        {
            cout << "Computer Wins!" << endl;
            return;
        }
    }
}

