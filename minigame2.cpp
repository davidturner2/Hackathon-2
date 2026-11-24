#include "main.h"
#include "minigame2/header.h"
#include "raylib.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int minigame2_sample() {
  srand(static_cast<unsigned int>(time(0)));
  InitWindow(1000, 800, "Minigame 2 Sample");
  SetTargetFPS(30);

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
