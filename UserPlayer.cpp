#include "master.h"


void Player::playerTurn()
{
    char valueHand;
    char targetHand;
    cout << "which of your hands value would you like to add to one of the enemies hands? (L/R):";
    cin >> valueHand;
    if (valueHand == 'l' || valueHand == 'L')
    {
        cout << "which of the enemies hands would you like to add your left hand's value to?: ";
        cin >> targetHand;
        cout << endl;
        if (targetHand == 'l' || targetHand == 'L')
        {
            playerl2l();
        }
        else if (targetHand == 'r' || targetHand == 'R')
        {
            playerl2r();
        }
        else
        {
            playerTurn();
        }
    }
    else if (valueHand == 'r' || valueHand == 'R')
    {
        cout << "which of the enemies hands would you like to add your right hand's value to?: ";
        cin >> targetHand;
        cout << endl;
        if (targetHand == 'l' || targetHand == 'L')
        {
            playerr2l();
        }
        else if (targetHand == 'r' || targetHand == 'R')
        {
            playerr2r();
        }
        else
        {
            playerTurn();
        }
    }
    else
    {
        playerTurn();
    }
}
void Player::playerl2l()
{
    if (CPU.getLeft() == 0)
    {
        cout << "That Hand is already Eliminated" << endl;
        playerTurn();

    }
    CPU.setPlusLeft(playerLeft);
    if (CPU.getLeft() > maxHandValue)
    {
        int remainder = CPU.getLeft() - maxHandValue;
        CPU.setLeft(remainder);
    }
}

void Player::playerl2r()
{
if (CPU.getRight() == 0)
    {
        cout << "That Hand is already Eliminated" << endl;
        playerTurn();

    }
    CPU.setPlusRight(playerLeft);
    if (CPU.getRight() > maxHandValue)
    {
        int remainder = CPU.getRight() - maxHandValue;
        CPU.setRight(remainder);
}
}

void Player::playerr2r()
{
if (CPU.getRight() == 0)
    {
        cout << "That Hand is already Eliminated" << endl;
        playerTurn();
    }
    
    CPU.setPlusRight(playerRight);
    if (CPU.getRight() > maxHandValue)
    {
        int remainder = CPU.getRight() - maxHandValue;
        CPU.setRight(remainder);
    }
}

void Player::playerr2l()
{
    if (CPU.getLeft() == 0)
    {
        cout << "That Hand is already Eliminated" << endl;
        playerTurn();
    }
    CPU.setPlusLeft(playerRight);
    if (CPU.getLeft() > maxHandValue)
    {
        int remainder = CPU.getLeft() - maxHandValue;
        CPU.setLeft(remainder);
    }
}

