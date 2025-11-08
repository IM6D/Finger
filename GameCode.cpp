#include "master.h"

// Use the global User and CPU that master.h declares
bool GameCode::playerHasHands() { return (User.getRight() != 0) || (User.getLeft() != 0); }
bool GameCode::cpuHasHands() { return (CPU.getRight() != 0) || (CPU.getLeft() != 0); }

void GameCode::start()
{
    while (gameRunning)
    {
        gameLoop();
    }
}

void GameCode::gameLoop()
{
    // Show current state
    cout << "\n=== Current Hands ===\n";
    cout << "You  -> L:" << User.getLeft() << " R:" << User.getRight() << '\n';
    cout << "CPU  -> L:" << CPU.getLeft() << " R:" << CPU.getRight() << '\n';
    cout << "=====================\n\n";

    // Player turn
    if (playerHasHands())
    {
        User.playerTurn();

        // If CPU hit exactly 5 on a hand, eliminate it
        if (CPU.getLeft() == maxHandValue)
        {
            cout << "Enemy Left Hand Eliminated!\n";
            CPU.setLeft(0);
        }
        if (CPU.getRight() == maxHandValue)
        {
            cout << "Enemy Right Hand Eliminated!\n";
            CPU.setRight(0);
        }

        if (!cpuHasHands())
        {
            cout << "You Win!\n";
            gameRunning = false;
            return;
        }
    }

    // CPU turn
    if (cpuHasHands())
    {
        CPU.cpuTurn();

        // If YOU hit exactly 5 on a hand, eliminate it
        if (User.getLeft() == maxHandValue)
        {
            cout << "Your Left Hand Has Been Eliminated!\n";
            User.setLeft(0);
        }
        if (User.getRight() == maxHandValue)
        {
            cout << "Your Right Hand Has Been Eliminated!\n";
            User.setRight(0);
        }

        if (!playerHasHands())
        {
            cout << "Computer Wins!\n";
            gameRunning = false;
            return;
        }
    }
}
