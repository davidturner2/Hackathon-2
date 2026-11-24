#include "main.h"
#include <iostream>
#include "raylib.h"
#include "minigameboat.h"
using namespace std;

int main() {

    // Starting
    InitWindow(1000, 800, "hackathon example");
    SetTargetFPS(30);
    Minigameboat hi = Minigameboat(100,70);

    hi.update();
    
    // run every frame
   
    /***
    //from raylibs example
deltaCircle.x += GetFrameTime()*6.0f*speed;
     your project code here!

     ***/

  // To see graphics for minigame 2, right now no gameplay loop
  // minigame2_sample();


    return 0;
}