#include "main.h"
#include "minigame2/header.h"
#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

void SlashWiresInGridPosition(Plugs &plugs, Vector2 gridPosition);

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

  float fps;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    fps = GetFrameTime();

    DrawGrid();
    plugs.UpdatePlugs(fps);
    plugs.UpdateWires(fps);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 mousePosition = GetMousePosition();
      Vector2 gridPosition = MousePositionToGridPosition(mousePosition);
      cout << "Mouse Position: (" << mousePosition.x << ", " << mousePosition.y
           << ") Grid Position: (" << gridPosition.x << ", " << gridPosition.y
           << ")" << endl;
      SlashWiresInGridPosition(plugs, gridPosition);
    }

    DrawWires(plugs);
    DrawPlugs(plugs);
    DrawHud();

    EndDrawing();
  }

  return 0;
}

Rectangle GridPositionToRectangle(Vector2 gridPosition) {
  float x = MINIGAME_START_X + gridPosition.x * GRID_SIZE;
  float y = MINIGAME_START_Y + gridPosition.y * GRID_SIZE;
  return Rectangle{x, y, static_cast<float>(GRID_SIZE),
                   static_cast<float>(GRID_SIZE)};
}

void SlashWiresInGridPosition(Plugs &plugs, Vector2 gridPosition) {
  Rectangle gridRect = GridPositionToRectangle(gridPosition);
  for (auto &wire : plugs.wires) {
    Plug &startPlug = plugs.plugs[wire.plugIndex1];
    Plug &endPlug = plugs.plugs[wire.plugIndex2];

    if (startPlug.ignore || endPlug.ignore) {
      continue;
    }

    Vector2 startCenter = startPlug.GetWireAnchor();
    Vector2 endCenter = endPlug.GetWireAnchor();

    Vector2 startHead = GetWireHeadPosition(startCenter, startPlug.side);
    Vector2 endHead = GetWireHeadPosition(endCenter, endPlug.side);

    double x0 = startHead.x;
    double y0 = startHead.y;
    double x1 = endHead.x;
    double y1 = endHead.y;

    if (CohenSutherlandLineClip(x0, y0, x1, y1, gridRect.x, gridRect.y,
                                gridRect.x + gridRect.width,
                                gridRect.y + gridRect.height)) {

      startPlug.explodeAt = startPlug.tick;
      endPlug.explodeAt = endPlug.tick;
    }
  }
}