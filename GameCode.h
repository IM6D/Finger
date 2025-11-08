#ifndef GAMECODE_H_
#define GAMECODE_H_

class GameCode
{
private:
    bool playerHasHands();
    bool cpuHasHands();
    bool gameRunning = false;
    int maxHandValue = 5;

public:
    GameCode() { gameRunning = true; }
    ~GameCode() { gameRunning = false; }
    
    void start();
    void gameLoop();
};

#endif
