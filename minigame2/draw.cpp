#include "header.h"
#include "raylib.h"
#include "raymath.h"
#include <string>

void DrawGrid() {
  int xOffset, yOffset;
  for (int x = 1; x < ROWS; x++) {
    xOffset = MINIGAME_START_X + x * CELL_SIZE;
    DrawLine(xOffset, MINIGAME_START_Y, xOffset,
             MINIGAME_START_Y + CELL_SIZE * COLUMNS, GRAY);
  }
  for (int y = 1; y < COLUMNS; y++) {
    yOffset = MINIGAME_START_Y + y * CELL_SIZE;
    DrawLine(MINIGAME_START_X, yOffset, MINIGAME_START_X + CELL_SIZE * ROWS,
             yOffset, GRAY);
  }
}

float GetFadeAlpha(Plug plug) {
  if (plug.expired || !plug.ignore) {
    return 1.0f;
  }
  return Clamp(1.0f - (plug.tick - plug.resolvedAt) / FADE_DURATION, 0.0f,
               1.0f);
}

void DrawPlugs(Plugs plugs) {
  for (const auto &plug : plugs.plugs) {
    Color plugColor = SAFE_PLUG_COLOR;
    if (!plug.safe && !plug.ignore) {
      float progress = Clamp(plug.tick / plug.explodeAt, 0.0f, 1.0f);
      plugColor = ColorLerp(DANGER_PLUG_COLOR, SAFE_PLUG_COLOR, progress);
    }
    plugColor = Fade(plugColor, GetFadeAlpha(plug));
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

    float alphaNormalized = GetFadeAlpha(startPlug);
    DrawWireHeadTexture(startCenter, startPlug.side, alphaNormalized);
    DrawWireHeadTexture(endCenter, endPlug.side, alphaNormalized);
    ConnectWireHeads(startCenter, endCenter, startPlug.side, endPlug.side,
                     alphaNormalized);
  }
}

Vector2 MousePositionToGridPosition(Vector2 mousePosition) {
  int col = (mousePosition.x - MINIGAME_START_X) / CELL_SIZE;
  int row = (mousePosition.y - MINIGAME_START_Y) / CELL_SIZE;
  col = max(0, min(col, COLUMNS - 1));
  row = max(0, min(row, ROWS - 1));
  return Vector2{static_cast<float>(col), static_cast<float>(row)};
}

void DrawSlices(vector<Slice> slices) {
  for (const auto &slice : slices) {
    float offset = CELL_SIZE * 0.1f;
    Rectangle cell = slice.cell;
    Vector2 topLeft = {cell.x + offset, cell.y + offset};
    Vector2 topRight = {cell.x + cell.width - offset, cell.y + offset};
    Vector2 bottomLeft = {cell.x + offset, cell.y + cell.height - offset};
    Vector2 bottomRight = {cell.x + cell.width - offset,
                           cell.y + cell.height - offset};

    float alphaNormalized = Clamp(slice.tick / SLICE_DURATION, 0.0f, 1.0f);
    DrawLineEx(topLeft, bottomRight, 4.0f, Fade(WHITE, alphaNormalized));
    DrawLineEx(topRight, bottomLeft, 4.0f, Fade(WHITE, alphaNormalized));
  }
}

// The shock bar is two rectangles: the outline and the bar filling it
void DrawShockBar(int x, int y) {
  int width = 300;
  int height = 24;
  int border = 2;

  float filled = Clamp(SHOCK_AMOUNT / SHOCK_MAX, 0.0f, 1.0f);
  DrawRectangleLines(x, y, width, height, WHITE);
  DrawRectangle(x + border, y + border,
                static_cast<int>((width - border * 2) * filled),
                height - border * 2, YELLOW);
}

void DrawHud() {
  int offsetY = PADDING * 2 + CELL_SIZE * ROWS;
  int textSize = 20;
  DrawText("Minigame 2: Unplug the plugs!", PADDING, offsetY, textSize, WHITE);

  Vector2 gridPos = MousePositionToGridPosition(GetMousePosition());
  string gridPosText = "Grid X: " + to_string(static_cast<int>(gridPos.x)) +
                       ", Y: " + to_string(static_cast<int>(gridPos.y));
  DrawText(gridPosText.c_str(), PADDING, offsetY + textSize + PADDING, textSize,
           WHITE);

  int panelX = MINIGAME_START_X + CELL_SIZE * COLUMNS + PADDING * 2;
  int panelY = MINIGAME_START_Y;
  DrawText(TextFormat("Time: %.1f", max(CURRENT_TIME_LEFT, 0.0f)), panelX,
           panelY, textSize, WHITE);
  DrawText("Shock", panelX, panelY + textSize + PADDING, textSize, WHITE);
  DrawShockBar(panelX, panelY + (textSize + PADDING) * 2);

  int totalWires = NUM_PLUGS / 2;
  int maxAllowedMissed = static_cast<int>(totalWires * FAIL_THRESHOLD_RATIO);

  int statsY = panelY + (textSize + PADDING) * 3 + 20;
  Color missedColor =
      (MISSED_CABLES_COUNT > maxAllowedMissed)
          ? RED
          : (MISSED_CABLES_COUNT == maxAllowedMissed ? YELLOW : WHITE);
  DrawText(TextFormat("Missed: %d / %d", MISSED_CABLES_COUNT, maxAllowedMissed),
           panelX, statsY, textSize, missedColor);
  DrawText(TextFormat("Total Cables: %d", totalWires), panelX,
           statsY + textSize + 6, 18, LIGHTGRAY);
  DrawText(TextFormat("Grid: %dx%d", ROWS, COLUMNS), panelX,
           statsY + (textSize + 6) * 2, 18, LIGHTGRAY);

  int resultY = statsY + 110;
  switch (CURRENT_GAME_STATE) {
  case GAME_WON:
    DrawText("All clear, you win!", panelX, resultY, 20, GREEN);
    break;
  case GAME_LOST_TOO_MANY_MISSED:
    DrawText("Too many cables missed!", panelX, resultY, 20, RED);
    break;
  case GAME_LOST_TIME:
    DrawText("Time ran out!", panelX, resultY, 20, RED);
    break;
  default:
    break;
  }
}
