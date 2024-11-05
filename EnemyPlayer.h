#ifndef ENEMYPLAYER_H_
#define ENEMYPLAYER_H_


class Enemy
{

public:

    void cpuTurn();
    void cpul2l();
    void cpul2r();
    void cpur2r();
    void cpur2l();
    int getRight () {return cpuRight;}
    int getLeft () {return cpuLeft;}
    void setRight (int newRight) {cpuRight = newRight;}
    void setLeft (int newLeft) {cpuLeft = newLeft;}
    void setPlusRight (int addRight) {cpuRight += addRight;}
    void setPlusLeft (int addLeft) {cpuLeft += addLeft;}

private:

    bool cpuHasHands() {return cpuRight || cpuLeft != 0; }
    int playersHands = 2;
    int maxHandValue = 5;
    int cpuRight = 1;
    int cpuLeft = 1;
};

#endif
