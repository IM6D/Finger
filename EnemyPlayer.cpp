#include "master.h"


void Enemy::cpuTurn()
{
int l2l = 25;                // chooses l2l if random moveSeed is between 0 and 25
    int l2r = 50;                // chooses l2r if random moveSeed is between 25 and 50
    int r2r = 75;                // chooses r2r if random moveSeed is between 50 and 75
    srand(time(NULL));
    int moveSeed = rand() % 100; // chooses r2l if random moveSeed is above 75 (max possible from seed is 100)

    if (cpuLeft != 0 && cpuRight != 0) // allows access to all possible moves while both hands are in play
    {
        
        if (cpuLeft + User.getLeft() == 5) // checks if there is a way to currently win or eliminate a player hand
        {
            cpul2l();   
            cout << "Enemy has added their left hand value to your left hand value\n" << endl;
        }
        else if (cpuLeft + User.getRight() == 5)
        {
            cpul2r();
            cout << "Enemy has added their left hand value to your right hand value\n" << endl;
        }
        else if (cpuRight + User.getLeft() == 5)
        {
            cpur2l();
            cout << "Enemy has added their right hand value to your left hand value\n" << endl;
        }
        else if (cpuRight + User.getRight() == 5)
        {
            cpur2r();
            cout << "Enemy has added their right hand value to your right hand value\n" << endl;
        }

        cout << "THIS TURN MOVE SEED:" << moveSeed << endl << endl;
        if (moveSeed > 0 && moveSeed <= l2l) // uses a random number to determine move if there is no way to win currently
        {
            if (User.getLeft() == 0)
            {
                cpuTurn();
            }
            cpul2l();
            cout << "Enemy has added their left hand value to your left hand value randomly\n" << endl;
        }
        else if (moveSeed > l2l && moveSeed <= l2r)
        {
            if (User.getRight() == 0)
            {
                cpuTurn();
            }
            cpul2r();
            cout << "Enemy has added their left hand value to your right hand value randomly\n" << endl;
        }
        else if (moveSeed > l2r && moveSeed <= r2r)
        {
            if (User.getRight() == 0)
            {
                cpuTurn();
            }
            cpur2r();
            cout << "Enemy has added their right hand value to your right hand value randomly\n" << endl;
        }
        else if (moveSeed > r2r)
        {
            if (User.getLeft() == 0)
            {
                cpuTurn();
            }
            cpur2l();
            cout << "Enemy has added their right hand value to your left hand value randomly\n" << endl;
        }
    }
    else if(cpuLeft == 0 )// only allows cpu to use its right hand
    {
        int leftElimSeed = rand() % 50;
        cout << "THIS LEFT ELIM MOVESEED:" << leftElimSeed << endl;
        if (cpuRight + User.getLeft() == 5)
        {
            if (User.getLeft() == 0)
            {
                cpuTurn();
            }
            cpur2l();
            cout << "Enemy has added their right hand value to your left hand value by calculation\n" << endl;
        }
        else if (cpuRight + User.getRight() == 5)
        {
            if (User.getRight() == 0)
            {
                cpuTurn();
            }
            cpur2r();
            cout << "Enemy has added their right hand value to your right hand value by calculation\n" << endl;
        }
        else if (leftElimSeed >= 25)
        {
            if (User.getRight() == 0)
            {
                cpuTurn();
            }
            cpur2r();
            cout << "Enemy has added their right hand value to your right hand value by calculation\n" << endl;
        }
        else if (leftElimSeed < 25)
        {
            if (User.getLeft() == 0)
            {
                cpuTurn();
            }
            cpur2l();
            cout << "Enemy has added their right hand value to your left hand value by calculation\n" << endl;
        }
    else if(cpuLeft == 0) // only allows cpu to use its left hand randomly
    {
        int rightElimSeed = rand() % 50;
        cout << "THIS RIGHT ELIM MOVESEED:" << leftElimSeed << endl;
        if (cpuLeft + User.getLeft() == 5)
        {
            if (User.getLeft() == 0)
            {
                cpuTurn();
            }
            cpul2l();
            cout << "Enemy has added their left hand value to your left hand value randomly\n" << endl;
        }
        else if (cpuLeft + User.getRight() == 5)
        {
            if (User.getRight() == 0)
            {
                cpuTurn();
            }
            cpul2r();
            cout << "Enemy has added their left hand value to your right hand value randomly\n" << endl;
        }
        else if (rightElimSeed >= 25)
        {
            if (User.getLeft() == 0)
            {
                cpuTurn();
            }
            cpul2l();
            cout << "Enemy has added their right hand value to your right hand value randomly\n" << endl;
        }
        else if (rightElimSeed < 25)
        {
            if (User.getRight() == 0)
            {
                cpuTurn();
            }
            cpul2r();
            cout << "Enemy has added their right hand value to your left hand value randomly\n" << endl;
        }
    }
}
}

void Enemy::cpul2l()
{
if (User.getLeft() == 0)
    {
        cpuTurn();
    }
    User.setPlusLeft(cpuLeft);
    if (User.getLeft() > maxHandValue)
    {
        int remainder = User.getLeft() - maxHandValue;
        User.setLeft(remainder);
    }
}

void Enemy::cpul2r()
{
if (User.getRight() == 0)
    {
        cpuTurn();
    }
    User.setPlusRight(cpuLeft);
    if (User.getRight() > maxHandValue)
    {
        int remainder = User.getRight() - maxHandValue;
        User.setRight(remainder);
    }
}

void Enemy::cpur2r()
{
    if (User.getRight() == 0)
    {
        cpuTurn();
    }
    User.setPlusRight(cpuRight);
    if (User.getRight() > maxHandValue)
    {
        int remainder = User.getRight() - maxHandValue;
        User.setRight(remainder);
    }
}

void Enemy::cpur2l()
{
    if (User.getLeft() == 0)
    {
        cpuTurn();
    }
    User.setPlusLeft(cpuRight);
    if (User.getLeft() > maxHandValue)
    {
        int remainder = User.getLeft() - maxHandValue;
        User.setLeft(remainder);
    }
}