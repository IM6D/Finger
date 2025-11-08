#ifndef USERPLAYER_H_
#define USERPLAYER_H_

class Player
{
public:
    void playerTurn();

    // concrete move helpers
    void playerl2l();
    void playerl2r();
    void playerr2r();
    void playerr2l();

    int  getRight() { return playerRight; }
    int  getLeft () { return playerLeft;  }

    void setRight(int newRight) { playerRight = newRight; }
    void setLeft (int newLeft)  { playerLeft  = newLeft;  }

    void setPlusRight(int addRight) { playerRight += addRight; }
    void setPlusLeft (int addLeft)  { playerLeft  += addLeft;  }

private:
    bool playerHasHands() { return (playerRight != 0) || (playerLeft != 0); }
    int  playersHands = 2;
    int  maxHandValue = 5;
    int  playerRight  = 1;
    int  playerLeft   = 1;
};

#endif
