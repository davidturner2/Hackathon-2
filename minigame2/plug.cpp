#include "header.h"

Plug::Plug(int newRow, int newColumn, Side newSide) {
  row = newRow;
  column = newColumn;
  side = newSide;
}

// Get the rectangle that represents the plug's position and size on the screen
Rectangle Plug::GetPlugRect() const {
  // The plug rectangle's cell left side
  float cellX = MINIGAME_START_X + column * GRID_SIZE;
  // The plug rectangle's cell top side
  float cellY = MINIGAME_START_Y + row * GRID_SIZE;
  // Center the plug rectangle in the cell on its longest side
  float centered = (GRID_SIZE - PLUG_WIDTH) / 2.0f;
  // Push the plug rectangle to the bottom or right of the cell
  float pushed = GRID_SIZE - PLUG_HEIGHT;

  switch (side) {
  case SIDE_TOP:
    return Rectangle{cellX + centered, cellY, static_cast<float>(PLUG_WIDTH), static_cast<float>(PLUG_HEIGHT)};
  case SIDE_BOTTOM:
    return Rectangle{cellX + centered, cellY + pushed, static_cast<float>(PLUG_WIDTH), static_cast<float>(PLUG_HEIGHT)};
  case SIDE_LEFT:
    return Rectangle{cellX, cellY + centered, static_cast<float>(PLUG_HEIGHT), static_cast<float>(PLUG_WIDTH)};

  default:
    return Rectangle{cellX + pushed, cellY + centered, static_cast<float>(PLUG_HEIGHT), static_cast<float>(PLUG_WIDTH)};
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