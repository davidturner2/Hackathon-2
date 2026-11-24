#include "header.h"
#include "raymath.h"

Slice::Slice(Rectangle newCell, float newTick) {
  cell = newCell;
  tick = newTick;
}

Rectangle GetGridArea() {
  return Rectangle{static_cast<float>(MINIGAME_START_X),
                   static_cast<float>(MINIGAME_START_Y),
                   static_cast<float>(CELL_SIZE * COLUMNS),
                   static_cast<float>(CELL_SIZE * ROWS)};
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
  if (fuseRange <= 0.0f)
    fuseRange = 1.0f;
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
        SHOCK_AMOUNT += SHOCK_SAFE_CUT;
        plugs.ResolveWire(wire, true);
      } else {
        float fuse = !startPlug.safe ? startPlug.explodeAt : endPlug.explodeAt;
        SHOCK_AMOUNT += GetCutShock(fuse);
        plugs.ResolveWire(wire, true);
      }
    }
  }
  return cutSafeWire;
}
