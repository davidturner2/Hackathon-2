#include "main.h"
#include "minigame2/header.h"
#include "raylib.h"
#include <iostream>

using namespace std;

int minigame2_sample() {
  InitWindow(1000, 800, "Minigame 2 Sample");
  SetTargetFPS(30);
  SetDangerPlugColor("ORANGE");

  if (InitMinigame2() != 0)
    return -1;

  while (!WindowShouldClose()) {
    NextFrame();
    SessionState sessionState = LevelUpIfNeeded();
    if (sessionState == SESSION_LOST) {
      cout << "Game over!" << endl;
      break;
    }
  }

  UnloadTexture(PLUG);
  CloseWindow();
  return 0;
}
