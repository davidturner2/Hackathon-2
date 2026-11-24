#include "main.h"
#include "minigame2/header.h"
#include "raylib.h"
#include <iostream>

using namespace std;

enum State {
  MINIGAME_1,
  MINIGAME_2,
};
State CURRENT_STATE = MINIGAME_2;
int MINIGAME_2_ITERATIONS = 0;

int main() {

  // Starting
  InitWindow(1000, 800, "hackathon example");
  SetTargetFPS(30);

  // minigame 2 initialization
  if (InitMinigame2() != 0)
    return -1;

  Vector2 mousePosition;
  Vector2 boatPosition = {100, 400};
  Color blue = {120, 240, 200, 255};
  Rectangle stream = {500, 300, 50, 200};
  // run every frame
  while (!WindowShouldClose()) {
    if (CURRENT_STATE == MINIGAME_1) {
      BeginDrawing();
      mousePosition = GetMousePosition();
      // background color
      ClearBackground(blue);
      // position= Vector2MoveTowards(Vector2 v, Vector2 target, float
      // maxDistance)
      if (IsMouseButtonDown(0) &&
          abs(mousePosition.y - boatPosition.y) > 5.1f &&
          mousePosition.y < 800 && mousePosition.y > 0) {
        if (mousePosition.y < boatPosition.y) {
          boatPosition.y -= 120.0f * GetFrameTime();
        } else {
          boatPosition.y += 120.0f * GetFrameTime();
        }
      }
      if (boatPosition.x > 1000) {
        boatPosition.x = -20;
      }

      // slipstream
      DrawRectangle(stream.x, stream.y, stream.width, stream.height,
                    (Color){58, 79, 23, 255});

      // cursor
      DrawCircle(mousePosition.x, mousePosition.y, 20,
                 (Color){255, 255, 255, 100});

      // boat
      DrawCircle(boatPosition.x, boatPosition.y, 20, WHITE);

      if (CheckCollisionCircleRec(boatPosition, 20, stream)) {
        boatPosition.y -= 100 * GetFrameTime();
        boatPosition.x += 20.5 * GetFrameTime();
      } else {
        boatPosition.x += 70.5 * GetFrameTime();
      }

      // ice berg
      DrawRectangle(800, 300, 20, 100, (Color){219, 233, 255, 255});
      EndDrawing();
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

  // Minigame 2 stuff
  UnloadTexture(PLUG);

  return 0;
}