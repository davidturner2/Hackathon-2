#ifndef SLIPSTREAMH
#define SLIPSTREAMH
#include "raylib.h"
class Slipstream
{
private:
    Rectangle stream ={500,300,50,200};
    int speed;
    // using an array instead of a vector and hardcoding 10 becuase a vector is unnescery here
    float offset[10] = {0,0,0,0,0,0,0,0,0,0};
    float step1[10] = {0,0,0,0,0,0,0,0,0,0};
    float step2[10] = {0,0,0,0,0,0,0,0,0,0};

public:
    Slipstream(Rectangle i =(Rectangle){500,300,50,200} ,int sspeed=100);
    int GetSpeed();
    void SetSpeed(int hi);
    void resetoffset();
    Rectangle Collider();
    void SetCollider(Rectangle hi);
    void update();
};
#endif

