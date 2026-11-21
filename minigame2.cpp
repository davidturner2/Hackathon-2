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

  Plugs plugs = Plugs(ROWS, COLUMNS);
  plugs.GeneratePlugs();
  plugs.GenerateWires();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawGrid();
    DrawPlugs(plugs);
    DrawWires(plugs);

    EndDrawing();
  }

  return 0;
}