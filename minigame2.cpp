#include "main.h"
#include "minigame2/header.h"
#include "raylib.h"
#include "raymath.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

Rectangle GridPositionToRectangle(Vector2 gridPosition);
bool SlashWiresInGridPosition(Plugs &plugs, Vector2 gridPosition);
void UpdateSlices(vector<Slice> &slices, float deltaTime);

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

  Rectangle gridArea = {static_cast<float>(MINIGAME_START_X),
                        static_cast<float>(MINIGAME_START_Y),
                        static_cast<float>(CELL_SIZE * COLUMNS),
                        static_cast<float>(CELL_SIZE * ROWS)};
  vector<Slice> slices;
  GameState state = GAME_PLAYING;
  float timeLeft = GAME_DURATION;

  float deltaTime;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    deltaTime = GetFrameTime();

    DrawGrid();

    plugs.UpdatePlugs(deltaTime);
    UpdateSlices(slices, deltaTime);
    SHOCK_AMOUNT = max(0.0f, SHOCK_AMOUNT - SHOCK_DRAIN_RATE * deltaTime);

    if (state == GAME_PLAYING) {
      timeLeft -= deltaTime;

      // When an overheating system explode shocks the player
      SHOCK_AMOUNT += plugs.UpdateWires(deltaTime) * SHOCK_EXPLOSION;

      // The player can't cut anything while they are shocked
      Vector2 mousePosition = GetMousePosition();
      if (SHOCK_AMOUNT <= 0.0f && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
          CheckCollisionPointRec(mousePosition, gridArea)) {
        Vector2 gridPosition = MousePositionToGridPosition(mousePosition);

        slices.push_back(Slice{GridPositionToRectangle(gridPosition), 0.0f});
        bool cutSafeWire = SlashWiresInGridPosition(plugs, gridPosition);
        if (cutSafeWire) {
          state = GAME_LOST_CUT_SAFE;
        }
      }
      SHOCK_AMOUNT = min(SHOCK_AMOUNT, SHOCK_MAX);

      if (state == GAME_PLAYING) {
        if (!plugs.HasUnresolvedOverheating()) {
          state = GAME_WON;
        } else if (timeLeft <= 0.0f) {
          state = GAME_LOST_TIME;
        }
      }
    }

    DrawWires(plugs);
    DrawPlugs(plugs);
    DrawSlices(slices);
    DrawHud(timeLeft, state);

    EndDrawing();
  }

  return 0;
}

Rectangle GridPositionToRectangle(Vector2 gridPosition) {
  float x = MINIGAME_START_X + gridPosition.x * CELL_SIZE;
  float y = MINIGAME_START_Y + gridPosition.y * CELL_SIZE;
  return Rectangle{x, y, static_cast<float>(CELL_SIZE),
                   static_cast<float>(CELL_SIZE)};
}

void UpdateSlices(vector<Slice> &slices, float deltaTime) {
  for (int i = static_cast<int>(slices.size()) - 1; i >= 0; i--) {
    slices[i].tick += deltaTime;
    if (slices[i].tick > SLICE_DURATION) {
      slices.erase(slices.begin() + i);
    }
  }
}

float GetCutShock(float explodeAt) {
  float fuseRange = MAX_EXPLODE_TIME - MIN_EXPLODE_TIME;
  float fastness = 1.0f - (explodeAt - MIN_EXPLODE_TIME) / fuseRange;
  return Lerp(SHOCK_CUT_MIN, SHOCK_CUT_MAX, Clamp(fastness, 0.0f, 1.0f));
}

// Cuts every wire that passes through the grid cell. Returns true if a safe
// wire is cut.
bool SlashWiresInGridPosition(Plugs &plugs, Vector2 gridPosition) {
  bool cutSafeWire = false;
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

      if (startPlug.safe && endPlug.safe) {
        cutSafeWire = true;
        plugs.ResolveWire(wire, false);
      } else {
        SHOCK_AMOUNT += GetCutShock(startPlug.explodeAt);
        plugs.ResolveWire(wire, true);
      }
    }
  }
  return cutSafeWire;
}