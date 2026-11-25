
#ifndef MINIGAMEBOATH
#define MINIGAMEBOATH
#include "raylib.h"
#include "slipstream.h"

class Minigameboat
{
private:
    Vector2 boatPosition = {-10, 400};
    Vector2 mousePosition;
    float step = 0;
    float step2 = 0;
    float step3 = 0;
    float step4 = 0;
    bool fart = true;
    int icebergoffset = 0;
    float resistance = 1;
    int boatspeed;
    int force;
    // just make a set maximum array of 45 obstactles and then use a variable to restrict the number that get shown
    int obc = 29;
    Slipstream obstacles[45];
    
public:
    bool won = false;
    bool lose = false;
    void update();
    void restart();
    void SetRandom(int i, int max, int lol);
    Minigameboat(int bspeed, int force);
};
#endif