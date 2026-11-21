#include "main.h"
#include "minigame2/header.h"
#include "raylib.h"
#include <cstdlib>
#include <ctime>

using namespace std;

int minigame2_sample() {
  srand(time(0));
  InitWindow(1000, 800, "Minigame 2 Sample");
  SetTargetFPS(30);

  setup(9, 9);
  PLUG = LoadTexture("minigame2/assets/plug.png");
  if (PLUG.id == 0) {
    CloseWindow();
    return -1;
  }

  Plugs plugs = Plugs();
  plugs.GeneratePlugs();
  plugs.GenerateWires();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawGrid();
    DrawWires(plugs);
    DrawPlugs(plugs);
    DrawHud();

    EndDrawing();
  }

  return 0;
}