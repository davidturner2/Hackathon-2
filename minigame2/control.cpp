#include "header.h"
#include <iostream>
using namespace std;

void RestartMinigame2(const DifficultyConfig &config) {
  config.Apply();

  PLUGS.rows = ROWS;
  PLUGS.columns = COLUMNS;
  PLUGS.GeneratePlugs();
  PLUGS.GenerateWires();

  SLICES.clear();

  CURRENT_GAME_STATE = GAME_PLAYING;
  CURRENT_TIME_LEFT = GAME_DURATION;
  SHOCK_AMOUNT = 0.0f;
  MISSED_CABLES_COUNT = 0;
}

void RestartMinigame2() { RestartMinigame2(CURRENT_CONFIG); }

// detects if the player clicks on cells.
void InputSlices() {
  Vector2 mousePosition = GetMousePosition();
  if (CURRENT_GAME_STATE == GAME_PLAYING && SHOCK_AMOUNT <= 0.0f &&
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CheckCollisionPointRec(mousePosition, GetGridArea())) {
    Vector2 gridPosition = MousePositionToGridPosition(mousePosition);
    SLICES.push_back(Slice(GridPositionToRectangle(gridPosition), 0.0f));
    SlashWiresInGridPosition(PLUGS, gridPosition);
  }
  SHOCK_AMOUNT = min(SHOCK_AMOUNT, SHOCK_MAX);
}

// Update wires and counts any newly expired wires to check if user failed or
// not
void UpdateWires(float DELTA_TIME) {
  int newlyExpired = PLUGS.UpdateWires(DELTA_TIME);
  MISSED_CABLES_COUNT += newlyExpired;

  int totalWires = static_cast<int>(PLUGS.wires.size());
  int maxAllowedMissed = static_cast<int>(totalWires * FAIL_THRESHOLD_RATIO);

  if (MISSED_CABLES_COUNT > maxAllowedMissed) {
    CURRENT_GAME_STATE = GAME_LOST_TOO_MANY_MISSED;
  }
}

int InitMinigame2() {
  PLUG = LoadTexture("minigame2/assets/plug.png");
  if (PLUG.id == 0) {
    CloseWindow();
    return -1;
  }
  RestartMinigame2(CURRENT_CONFIG);
  return 0;
}

void NextFrame() {
  BeginDrawing();
  ClearBackground(BLACK);
  DELTA_TIME = GetFrameTime();

  DrawGrid();

  PLUGS.UpdatePlugs(DELTA_TIME);
  UpdateSlices(SLICES, DELTA_TIME);
  SHOCK_AMOUNT = max(0.0f, SHOCK_AMOUNT - SHOCK_DRAIN_RATE * DELTA_TIME);

  if (CURRENT_GAME_STATE == GAME_PLAYING) {
    CURRENT_TIME_LEFT -= DELTA_TIME;

    UpdateWires(DELTA_TIME);
    InputSlices();

    if (CURRENT_GAME_STATE == GAME_PLAYING) {
      if (!PLUGS.HasUnresolvedOverheating()) {
        CURRENT_GAME_STATE = GAME_WON;
      } else if (CURRENT_TIME_LEFT <= 0.0f) {
        CURRENT_GAME_STATE = GAME_LOST_TIME;
      }
    }
  }

  DrawWires(PLUGS);
  DrawPlugs(PLUGS);
  DrawSlices(SLICES);
  DrawHud();

  EndDrawing();
}

SessionState LevelUpIfNeeded() {
  if (CURRENT_GAME_STATE != GAME_PLAYING) {
    POST_ROUND_TIMER += GetFrameTime();

    if (POST_ROUND_TIMER >= 3.0f) {
      if (CURRENT_GAME_STATE == GAME_WON) {
        float difficultyStep = (rand() % 500 + 300) / 10000.0;
        float nextFactor =
            min(1.0f, CURRENT_CONFIG.difficultyFactor + difficultyStep);
        RestartMinigame2(DifficultyConfig(nextFactor));
        POST_ROUND_TIMER = 0.0f;
      } else {
        return SESSION_LOST;
      }
    }
  }
  return SESSION_PLAYING;
}