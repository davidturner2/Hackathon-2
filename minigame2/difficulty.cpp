#include "header.h"
#include "raymath.h"

DifficultyConfig::DifficultyConfig() {
  rows = 9;
  columns = 9;
  dangerRatio = 0.45f;
  minExplodeTime = 4.0f;
  maxExplodeTime = 8.0f;
  gameDuration = 10.0f;
  shockSafeCut = 30.0f;
  shockDrainRate = 50.0f;
  failThresholdRatio = 0.20f;
  difficultyFactor = 0.5f;
}

DifficultyConfig::DifficultyConfig(float factor) {
  factor = Clamp(factor, 0.0f, 1.0f);
  difficultyFactor = factor;

  int gridSize = 4 + static_cast<int>(round(factor * (30 - 4)));
  rows = gridSize;
  columns = gridSize;

  dangerRatio = Lerp(0.35f, 0.75f, factor);

  minExplodeTime = Lerp(6.0f, 2.0f, factor);
  maxExplodeTime = Lerp(10.0f, 4.5f, factor);

  gameDuration = Lerp(8.0f, 25.0f, factor);

  shockSafeCut = Lerp(20.0f, 45.0f, factor);
  shockDrainRate = Lerp(60.0f, 40.0f, factor);
  failThresholdRatio = 0.25f;
}

void DifficultyConfig::Apply() const {
  CURRENT_CONFIG = *this;
  ROWS = rows;
  COLUMNS = columns;
  CELL_SIZE = GAME_SIZE / max(ROWS, COLUMNS);
  PLUG_WIDTH = static_cast<int>(CELL_SIZE * 0.7f);
  PLUG_HEIGHT = static_cast<int>(CELL_SIZE * 0.4f);
  NUM_PLUGS = ROWS * 2 + COLUMNS * 2 - 8;
  GAME_DURATION = gameDuration;
  MIN_EXPLODE_TIME = minExplodeTime;
  MAX_EXPLODE_TIME = maxExplodeTime;
  SHOCK_DRAIN_RATE = shockDrainRate;
  SHOCK_SAFE_CUT = shockSafeCut;
  FAIL_THRESHOLD_RATIO = failThresholdRatio;
}