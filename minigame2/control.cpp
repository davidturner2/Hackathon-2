#include "header.h"

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
