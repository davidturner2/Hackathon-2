#include "header.h"
#include <cstdlib>

Plug::Plug(int newRow, int newColumn, Side newSide) {
  row = newRow;
  column = newColumn;
  side = newSide;

  explodeAt = 0.0;
  tick = 0.0;
  resolvedAt = 0.0;

  ignore = false;
  safe = true;
  saved = false;
  expired = false;

  if (rand() % 3 == 0) {
    safe = false;
    int minT = static_cast<int>(MIN_EXPLODE_TIME);
    int maxT = static_cast<int>(MAX_EXPLODE_TIME);
    explodeAt = static_cast<float>(minT + rand() % (maxT - minT + 1));
  }
}

// Get the rectangle that represents the plug's position and size on the screen
Rectangle Plug::GetPlugRect() const {
  // The plug rectangle's cell left side
  float cellX = MINIGAME_START_X + column * CELL_SIZE;
  // The plug rectangle's cell top side
  float cellY = MINIGAME_START_Y + row * CELL_SIZE;
  // Center the plug rectangle in the cell on its longest side
  float centered = (CELL_SIZE - PLUG_WIDTH) / 2.0f;
  // Push the plug rectangle to the bottom or right of the cell
  float pushed = CELL_SIZE - PLUG_HEIGHT;

  switch (side) {
  case SIDE_TOP:
    return Rectangle{cellX + centered, cellY, static_cast<float>(PLUG_WIDTH),
                     static_cast<float>(PLUG_HEIGHT)};
  case SIDE_BOTTOM:
    return Rectangle{cellX + centered, cellY + pushed,
                     static_cast<float>(PLUG_WIDTH),
                     static_cast<float>(PLUG_HEIGHT)};
  case SIDE_LEFT:
    return Rectangle{cellX, cellY + centered, static_cast<float>(PLUG_HEIGHT),
                     static_cast<float>(PLUG_WIDTH)};

  default:
    return Rectangle{cellX + pushed, cellY + centered,
                     static_cast<float>(PLUG_HEIGHT),
                     static_cast<float>(PLUG_WIDTH)};
  }
}

// Get the middle of the plug's inner edge
Vector2 Plug::GetWireAnchor() const {
  Rectangle r = GetPlugRect();
  switch (side) {
  case SIDE_TOP:
    return Vector2{r.x + r.width / 2, r.y + r.height};
  case SIDE_BOTTOM:
    return Vector2{r.x + r.width / 2, r.y};
  case SIDE_LEFT:
    return Vector2{r.x + r.width, r.y + r.height / 2};
  default:
    return Vector2{r.x, r.y + r.height / 2};
  }
}
