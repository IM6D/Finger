#ifndef MASTER_H_
#define MASTER_H_

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::rand;
using std::srand;
using std::time;

#include "GameCode.h"
#include "EnemyPlayer.h"
#include "UserPlayer.h"

// Declare single global instances (defined in master.cpp)
extern GameCode game;
extern Player   User;
extern Enemy    CPU;

// Only a *declaration* here (no initializer)
extern const int maxHandValue;

#endif // MASTER_H_
