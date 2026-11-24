#include "main.h"
#include "minigameboat.h"
#include "minigame2/header.h"
#include "raylib.h"
#include "minigameboat.h"
using namespace std;

int main() {

    // Starting
    InitWindow(1000, 800, "hackathon example");
    SetTargetFPS(30);
    Minigameboat hi = Minigameboat(100,70);

  // minigame 2 initialization
  if (InitMinigame2() != 0)
    return -1;

    Minigameboat hi = Minigameboat(BOAT_SPEED,SHIP_FORCE);
  // run every frame
    while (!WindowShouldClose()) {
    if (CURRENT_STATE == MINIGAME_1) {
    hi.update();
    if(hi.won){
        CURRENT_STATE=MINIGAME_2;
        hi.won=false;
    }

      
    } else {
      NextFrame();
      SessionState sessionState = LevelUpIfNeeded();
      if (sessionState == SESSION_LOST) {
        cout << "Game over!" << endl;
        cout << "You reached level: " << MINIGAME_2_ITERATIONS << endl;
        break;
      }
      if (sessionState == SESSION_GAME_WON) {
        MINIGAME_2_ITERATIONS++;
      }
    }
  }
  /***
  //from raylibs example
deltaCircle.x += GetFrameTime()*6.0f*speed;
     your project code here!

     ***/

  // To see graphics for minigame 2, right now no gameplay loop
  // minigame2_sample();


    return 0;
}