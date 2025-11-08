#include <cstdlib>
#include "master.h"

// Helper: apply value to user's left; returns true if applied
static bool apply_to_user_left(int add, int maxHandValue) {
    if (User.getLeft() == 0) return false;
    int sum = User.getLeft() + add;
    if (sum == maxHandValue) {
        User.setLeft(maxHandValue); // game loop will eliminate (set to 0)
    } else if (sum > maxHandValue) {
        User.setLeft(sum - maxHandValue); // rollover
    } else {
        User.setLeft(sum);
    }
    return true;
}

// Helper: apply value to user's right; returns true if applied
static bool apply_to_user_right(int add, int maxHandValue) {
    if (User.getRight() == 0) return false;
    int sum = User.getRight() + add;
    if (sum == maxHandValue) {
        User.setRight(maxHandValue); // game loop will eliminate (set to 0)
    } else if (sum > maxHandValue) {
        User.setRight(sum - maxHandValue);
    } else {
        User.setRight(sum);
    }
    return true;
}

void Enemy::cpuTurn()
{
    // DO NOT seed here (avoid infinite-repeat seeds). Seed once in main if you want randomness.
    // srand((unsigned)time(nullptr));

    const int l2l = 25;
    const int l2r = 50;
    const int r2r = 75;

    // Short aliases of current values
    const int CL = cpuLeft;
    const int CR = cpuRight;
    const int UL = User.getLeft();
    const int UR = User.getRight();

    // If both CPU hands are eliminated, nothing to do (defensive)
    if (CL == 0 && CR == 0) return;

    // 1) Forced/Winning moves first (sum == 5)
    // We only consider moves where both source and target hands are alive (non-zero).
    if (CL != 0 && UL != 0 && CL + UL == maxHandValue) {
        if (apply_to_user_left(CL, maxHandValue)) {
            cout << "Enemy has added their left hand value to your left hand value\n" << endl;
            return;
        }
    }
    if (CL != 0 && UR != 0 && CL + UR == maxHandValue) {
        if (apply_to_user_right(CL, maxHandValue)) {
            cout << "Enemy has added their left hand value to your right hand value\n" << endl;
            return;
        }
    }
    if (CR != 0 && UL != 0 && CR + UL == maxHandValue) {
        if (apply_to_user_left(CR, maxHandValue)) {
            cout << "Enemy has added their right hand value to your left hand value\n" << endl;
            return;
        }
    }
    if (CR != 0 && UR != 0 && CR + UR == maxHandValue) {
        if (apply_to_user_right(CR, maxHandValue)) {
            cout << "Enemy has added their right hand value to your right hand value\n" << endl;
            return;
        }
    }

    // 2) Build a list of all legal moves, then choose randomly.
    // Each entry is (label, lambda)
    struct Move {
        const char* label;
        bool (*fn)(int add, int maxHandValue);
        int addValue; // CL or CR
    };

    Move candidates[4];
    int count = 0;

    if (CL != 0 && UL != 0) {
        candidates[count++] = {"Enemy has added their left hand value to your left hand value randomly\n",
                               [](int add, int M){ return apply_to_user_left(add, M); }, CL};
    }
    if (CL != 0 && UR != 0) {
        candidates[count++] = {"Enemy has added their left hand value to your right hand value randomly\n",
                               [](int add, int M){ return apply_to_user_right(add, M); }, CL};
    }
    if (CR != 0 && UR != 0) {
        candidates[count++] = {"Enemy has added their right hand value to your right hand value randomly\n",
                               [](int add, int M){ return apply_to_user_right(add, M); }, CR};
    }
    if (CR != 0 && UL != 0) {
        candidates[count++] = {"Enemy has added their right hand value to your left hand value randomly\n",
                               [](int add, int M){ return apply_to_user_left(add, M); }, CR};
    }

    if (count == 0) {
        // No legal targets (user hands are both 0) — game loop will end the game.
        return;
    }

    // Keep the "seed message" behavior, but don't reseed here.
    int moveSeed = rand() % 100;
    cout << "THIS TURN MOVE SEED:" << moveSeed << "\n\n";

    // To preserve your original distribution idea, pick a preferred quadrant, then fallback if invalid.
    int choiceIdx;
    if (moveSeed > 0 && moveSeed <= l2l) {
        // prefer l2l -> index 0 if present, else random among the rest
        choiceIdx = -1;
        for (int i = 0; i < count; ++i) {
            if (std::string(candidates[i].label).find("left hand value to your left") != std::string::npos) {
                choiceIdx = i; break;
            }
        }
        if (choiceIdx == -1) choiceIdx = rand() % count;
    } else if (moveSeed > l2l && moveSeed <= l2r) {
        choiceIdx = -1;
        for (int i = 0; i < count; ++i) {
            if (std::string(candidates[i].label).find("left hand value to your right") != std::string::npos) {
                choiceIdx = i; break;
            }
        }
        if (choiceIdx == -1) choiceIdx = rand() % count;
    } else if (moveSeed > l2r && moveSeed <= r2r) {
        choiceIdx = -1;
        for (int i = 0; i < count; ++i) {
            if (std::string(candidates[i].label).find("right hand value to your right") != std::string::npos) {
                choiceIdx = i; break;
            }
        }
        if (choiceIdx == -1) choiceIdx = rand() % count;
    } else { // > r2r
        choiceIdx = -1;
        for (int i = 0; i < count; ++i) {
            if (std::string(candidates[i].label).find("right hand value to your left") != std::string::npos) {
                choiceIdx = i; break;
            }
        }
        if (choiceIdx == -1) choiceIdx = rand() % count;
    }

    // Execute chosen legal move
    candidates[choiceIdx].fn(candidates[choiceIdx].addValue, maxHandValue);
    cout << candidates[choiceIdx].label << endl;
}

// Legacy helper names kept for compatibility (no recursion inside)
void Enemy::cpul2l() { apply_to_user_left(cpuLeft, maxHandValue); }
void Enemy::cpul2r() { apply_to_user_right(cpuLeft, maxHandValue); }
void Enemy::cpur2r() { apply_to_user_right(cpuRight, maxHandValue); }
void Enemy::cpur2l() { apply_to_user_left(cpuRight, maxHandValue); }
