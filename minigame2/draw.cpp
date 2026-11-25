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

void DrawShockBar(int x, int y) {
  int width = 360;
  int height = 20;
  int border = 1;

  float filled = Clamp(SHOCK_AMOUNT / SHOCK_MAX, 0.0f, 1.0f);
  DrawRectangleLines(x, y, width, height, WHITE);
  DrawRectangle(x + border, y + border,
                static_cast<int>((width - border * 2) * filled),
                height - border * 2, YELLOW);
}

void DrawHud() {
  int offsetY = PADDING * 2 + CELL_SIZE * ROWS;
  int titleSize = 20;
  int textSize = 16;

  // title
  // Convert to char[] since DrawText does not support string
  DrawText(TextFormat("Unplug the %s plugs.",
                      DANGER_PLUG_COLOR_NAME.c_str()),
           PADDING, offsetY, titleSize, WHITE);

  // shock bar
  DrawText("Shock", PADDING, offsetY + titleSize + PADDING + 2, textSize,
           WHITE);
  DrawShockBar(PADDING * 2 + 50, offsetY + titleSize + PADDING);

  // grid position
  const int belowShockBar = offsetY + titleSize + PADDING * 3 + textSize;
  Vector2 gridPos = MousePositionToGridPosition(GetMousePosition());
  DrawText(TextFormat("Grid X: %.0f, Y: %.0f", gridPos.x, gridPos.y), PADDING,
           belowShockBar, textSize, WHITE);
  DrawText(TextFormat("Time: %.1f", max(CURRENT_TIME_LEFT, 0.0f)),
           PADDING * 2 + 150, belowShockBar, textSize, WHITE);

  // Missed wires
  int totalWires = NUM_PLUGS / 2;
  int maxAllowedMissed = static_cast<int>(totalWires * FAIL_THRESHOLD_RATIO);

  Color missedColor;
  if (MISSED_CABLES_COUNT > maxAllowedMissed) {
    missedColor = RED;
  } else if (MISSED_CABLES_COUNT == maxAllowedMissed) {
    missedColor = YELLOW;
  } else {
    missedColor = WHITE;
  }
  DrawText(TextFormat("Missed: %d / %d", MISSED_CABLES_COUNT, maxAllowedMissed),
           PADDING * 3 + 300, belowShockBar, textSize, missedColor);
  DrawText(TextFormat("Grid: %dx%d", ROWS, COLUMNS), PADDING * 2 + 450, offsetY,
           textSize, LIGHTGRAY);
  DrawText(TextFormat("Total Cables: %d", totalWires), PADDING * 2 + 450,
           offsetY + textSize + PADDING, textSize, LIGHTGRAY);

  switch (CURRENT_GAME_STATE) {
  case GAME_WON:
    DrawText("Data Center Deactivated.", PADDING,
             belowShockBar + (PADDING + textSize) * 2, textSize, GREEN);
    break;
  case GAME_LOST_TOO_MANY_MISSED:
    DrawText("Too many cables missed!", PADDING,
             belowShockBar + (PADDING + textSize) * 2, textSize, RED);
    break;
  case GAME_LOST_TIME:
    DrawText("Time ran out!", PADDING, belowShockBar + (PADDING + textSize) * 2,
             textSize, RED);
    break;
  default:
    break;
  }
}
