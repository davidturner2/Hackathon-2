#include "header.h"
#include "raylib.h"
#include "raymath.h"
#include <string>

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
    Color plugColor;
    if (plug.safe) {
      plugColor = DARKGRAY;
    } else if (plug.tick > plug.explodeAt) {
      float alphaNormalized =
          Lerp(255, 0, (plug.tick - plug.explodeAt) / 0.5) / 255.0f;
      plugColor = Fade(RED, alphaNormalized);
    } else {
      plugColor = ColorLerp(DARKGRAY, RED, plug.tick / plug.explodeAt);
    }
    DrawRectangleRec(plug.GetPlugRect(), plugColor);
  }
}

void DrawWireHeadTexture(Vector2 center, Side side, float alphaNormalized) {
  float rotation = 0.0f;
  // scale so that head fits in the plug rectangle
  float scale = PLUG_WIDTH / static_cast<float>(PLUG.width);
  float outset = PLUG_HEIGHT * 0.55f;

  float xDimension = PLUG.width * scale;
  float yDimension = PLUG.height * scale;
  switch (side) {
  case SIDE_TOP:
    rotation = 0.0f;
    center.y += outset;
    break;
  case SIDE_BOTTOM:
    rotation = 180.0f;
    center.y -= outset;
    break;
  case SIDE_LEFT:
    rotation = -90.0f;
    center.x += outset;
    break;
  case SIDE_RIGHT:
    rotation = 90.0f;
    center.x -= outset;
    break;
  }

  DrawTexturePro(PLUG,
                 Rectangle{0, 0, static_cast<float>(PLUG.width),
                           static_cast<float>(PLUG.height)},
                 Rectangle{center.x, center.y, xDimension, yDimension},
                 Vector2{xDimension / 2, yDimension / 2}, rotation,
                 Fade(WHITE, alphaNormalized));
}

Vector2 GetWireHeadPosition(Vector2 center, Side side) {
  float scale = PLUG_WIDTH / static_cast<float>(PLUG.width);
  float outset = PLUG_HEIGHT * 0.55f;

  float xOffset = PLUG.width * scale / 2.0;
  float yOffset = PLUG.height * scale / 2.0;
  switch (side) {
  case SIDE_TOP:
    center.y += outset + yOffset;
    break;
  case SIDE_BOTTOM:
    center.y -= outset + yOffset;
    break;
  case SIDE_LEFT:
    center.x += outset + xOffset;
    break;
  case SIDE_RIGHT:
    center.x -= outset + xOffset;
    break;
  }
  return center;
}

void ConnectWireHeads(Vector2 startCenter, Vector2 endCenter, Side startSide,
                      Side endSide, float alphaNormalized) {
  Vector2 startHead = GetWireHeadPosition(startCenter, startSide);
  Vector2 endHead = GetWireHeadPosition(endCenter, endSide);
  DrawLineEx(startHead, endHead, 2.0, Fade(WIRE_COLOR, alphaNormalized));
}

void DrawWires(Plugs plugs) {
  for (const auto &wire : plugs.wires) {
    Plug startPlug = plugs.plugs[wire.plugIndex1];
    Plug endPlug = plugs.plugs[wire.plugIndex2];

    Vector2 startCenter = startPlug.GetWireAnchor();
    Vector2 endCenter = endPlug.GetWireAnchor();

    float alphaNormalized = 1.0f;
    if (!startPlug.safe) {
      alphaNormalized =
          Lerp(255, 0, (startPlug.tick - startPlug.explodeAt) / 0.5) / 255.0f;
    }
    DrawWireHeadTexture(startCenter, startPlug.side, alphaNormalized);
    DrawWireHeadTexture(endCenter, endPlug.side, alphaNormalized);
    ConnectWireHeads(startCenter, endCenter, startPlug.side, endPlug.side,
                     alphaNormalized);
  }
}

Vector2 MousePositionToGridPosition(Vector2 mousePosition) {
  int col = (mousePosition.x - MINIGAME_START_X) / GRID_SIZE;
  int row = (mousePosition.y - MINIGAME_START_Y) / GRID_SIZE;
  col = max(0, min(col, COLUMNS - 1));
  row = max(0, min(row, ROWS - 1));
  return Vector2{static_cast<float>(col), static_cast<float>(row)};
}

void DrawHud() {
  int offsetY = PADDING * 2 + GRID_SIZE * ROWS;
  int textSize = 20;
  DrawText("Minigame 2: Unplug the plugs!", PADDING, offsetY, textSize, WHITE);

  Vector2 gridPos = MousePositionToGridPosition(GetMousePosition());
  string gridPosText = "Grid X: " + to_string(static_cast<int>(gridPos.x)) +
                       ", Y: " + to_string(static_cast<int>(gridPos.y));
  DrawText(gridPosText.c_str(), PADDING, offsetY + textSize + PADDING, textSize,
           WHITE);
}