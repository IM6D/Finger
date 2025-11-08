#include "master.h"
#include <cctype>
#include <iostream>

static bool is_zero_hand(char h) {
    if (h == 'l' || h == 'L') return User.getLeft() == 0;
    if (h == 'r' || h == 'R') return User.getRight() == 0;
    return true;
}

void Player::playerTurn()
{
    while (true) {
        char valueHand;
        cout << "which of your hands value would you like to add to one of the enemies hands? (L/R):";
        cin >> valueHand;

        if (!(valueHand == 'l' || valueHand == 'L' || valueHand == 'r' || valueHand == 'R')) {
            cout << "Invalid input, please try again." << endl;
            continue;
        }
        if (is_zero_hand(valueHand)) {
            cout << "That hand is eliminated; choose a hand that is still in play." << endl;
            continue;
        }

        // Inner loop lets the user pick enemy hand or go back
        while (true) {
            char targetHand;
            cout << "which of the enemies hands would you like to add your "
                 << ((valueHand=='l'||valueHand=='L') ? "left" : "right")
                 << " hand's value to? (L/R, or B to go back): ";
            cin >> targetHand;
            cout << endl;

            if (targetHand == 'b' || targetHand == 'B') {
                // go back to choose our own hand again
                break;
            }

            if (!(targetHand == 'l' || targetHand == 'L' || targetHand == 'r' || targetHand == 'R')) {
                cout << "Invalid input, please try again." << endl;
                continue;
            }

            // Target must be alive
            if ((targetHand=='l'||targetHand=='L') && CPU.getLeft() == 0) {
                cout << "That Hand is already Eliminated" << endl;
                continue;
            }
            if ((targetHand=='r'||targetHand=='R') && CPU.getRight() == 0) {
                cout << "That Hand is already Eliminated" << endl;
                continue;
            }

            // Execute the chosen move
            if ((valueHand=='l'||valueHand=='L') && (targetHand=='l'||targetHand=='L')) {
                playerl2l();
            } else if ((valueHand=='l'||valueHand=='L') && (targetHand=='r'||targetHand=='R')) {
                playerl2r();
            } else if ((valueHand=='r'||valueHand=='R') && (targetHand=='r'||targetHand=='R')) {
                playerr2r();
            } else { // value right -> target left
                playerr2l();
            }

            // Done with a valid move — exit both loops
            return;
        }
        // If we reached here, user hit 'B' — re-prompt outer hand choice
    }
}

void Player::playerl2l()
{
    // CPU left must be alive; checked by caller, but keep defensive
    if (CPU.getLeft() == 0) return;
    int sum = CPU.getLeft() + playerLeft;
    if (sum == maxHandValue) {
        CPU.setLeft(maxHandValue); // game loop will eliminate
    } else if (sum > maxHandValue) {
        CPU.setLeft(sum - maxHandValue);
    } else {
        CPU.setLeft(sum);
    }
}

void Player::playerl2r()
{
    if (CPU.getRight() == 0) return;
    int sum = CPU.getRight() + playerLeft;
    if (sum == maxHandValue) {
        CPU.setRight(maxHandValue);
    } else if (sum > maxHandValue) {
        CPU.setRight(sum - maxHandValue);
    } else {
        CPU.setRight(sum);
    }
}

void Player::playerr2r()
{
    if (CPU.getRight() == 0) return;
    int sum = CPU.getRight() + playerRight;
    if (sum == maxHandValue) {
        CPU.setRight(maxHandValue);
    } else if (sum > maxHandValue) {
        CPU.setRight(sum - maxHandValue);
    } else {
        CPU.setRight(sum);
    }
}

void Player::playerr2l()
{
    if (CPU.getLeft() == 0) return;
    int sum = CPU.getLeft() + playerRight;
    if (sum == maxHandValue) {
        CPU.setLeft(maxHandValue);
    } else if (sum > maxHandValue) {
        CPU.setLeft(sum - maxHandValue);
    } else {
        CPU.setLeft(sum);
    }
}
