#include "header.h"

void DrawGrid() {
  int xOffset, yOffset;
  for (int x = 1; x < ROWS; x++) {
    xOffset = MINIGAME_START_X + x * GRID_SIZE;
    DrawLine(xOffset, MINIGAME_START_Y, xOffset,
             MINIGAME_START_Y + GRID_SIZE * COLUMNS, GRAY);
  }
  for (int y = 1; y < COLUMNS; y++) {
    yOffset = MINIGAME_START_Y + y * GRID_SIZE;
    DrawLine(MINIGAME_START_X, yOffset, MINIGAME_START_X + GRID_SIZE * ROWS,
             yOffset, GRAY);
  }
}

void DrawPlugs(Plugs plugs) {
  for (const auto &plug : plugs.plugs) {
    DrawRectangleRec(plug.GetPlugRect(), BLUE);
  }
}

void DrawWires(Plugs plugs) {
  for (const auto &wire : plugs.wires) {
    Vector2 start = plugs.plugs[wire.plugIndex1].GetWireAnchor();
    Vector2 end = plugs.plugs[wire.plugIndex2].GetWireAnchor();
    DrawLineEx(start, end, 3, RED);
  }
}