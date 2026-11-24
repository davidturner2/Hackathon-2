// headers here ...
#include "raylib.h"
#include <algorithm>
#include <vector>

using namespace std;

inline Texture2D PLUG;

const int PADDING = 10; // Padding for the UI

inline int MINIGAME_START_X = PADDING; // The offset of the minigame grid from x
inline int MINIGAME_START_Y = PADDING; // The offset of the minigame grid from y

const int GAME_SIZE = 600; // Size of the board in pixels

inline int ROWS = 9;    // Number of cell rows
inline int COLUMNS = 9; // Number of cell columns
inline int PLUG_WIDTH;  // plug's longest side
inline int PLUG_HEIGHT; // plug's shortest side
inline int CELL_SIZE;   // The size of each cell

// Gameplay Stuff
inline float GAME_DURATION = 10.0; // seconds the player has to clear the board
inline float MIN_EXPLODE_TIME = 3.0; // shortest fuse of an overheating plug (s)
inline float MAX_EXPLODE_TIME = 10.0; // longest fuse of an overheating plug (s)
const float FADE_DURATION = 0.5;      // seconds a cut/exploded system fades out

// Constants for the Shock bar
inline float SHOCK_AMOUNT = 0.0;
const float SHOCK_MAX = 100.0f; // highest amount of shock
inline float SHOCK_DRAIN_RATE =
    50.0f; // how fast the shock bar drains per second
const float SHOCK_CUT_MIN =
    10.0f; // lowest amount of shock to add when cutting a system
const float SHOCK_CUT_MAX = 40.0f;   // opposite
const float SHOCK_EXPLOSION = 40.0f; // an overheating system exploded
inline float SHOCK_SAFE_CUT = 30.0f; // shock added when cutting a safe wire
inline float FAIL_THRESHOLD_RATIO =
    0.20f; // Percentage of total wire allowed to miss.

const float SLICE_DURATION =
    0.4f; // seconds a streak takes to go from one corner to the other

inline int NUM_PLUGS;           // Number of plugs on the board
inline int NUM_EXPLODING_PLUGS; // How many plugs will overheat

// plug colors
inline Color DANGER_PLUG_COLOR = RED;
inline Color SAFE_PLUG_COLOR = DARKGRAY;
const Color WIRE_COLOR = {203, 212, 216, 255}; // Wire line color

class DifficultyConfig {
public:
  int rows;
  int columns;
  float dangerRatio;
  float minExplodeTime;
  float maxExplodeTime;
  float gameDuration;
  float shockSafeCut;
  float shockDrainRate;
  float failThresholdRatio;
  float difficultyFactor;

  DifficultyConfig();
  DifficultyConfig(float factor);

  void Apply() const;
};

inline DifficultyConfig CURRENT_CONFIG;

// Previously we used some math to check if the wire was on the left or right
// side of the grid, or top or bottom. We decided to use an enum since it can
// easily represent what side the plug is on and make future math more clear:
// For example we had something like:
// int CalculateYInGrid(int row, int offset) {
//     return row*CELL_SIZE + (CELL_SIZE-PLUG_HEIGHT)*(row != 0) + offset;
// }
// Where (row != 0) would check if the plug was on top (row == 0), which would
// align the plug with the top of the cell, and other wise would align it at the
// bottom of the cell for anything else. And given each row only has two plugs
// it could only be top of bottom aligned. Initially wanted to only use concepts
// learned in class, but it got very un-readable :)
enum Side { SIDE_TOP, SIDE_BOTTOM, SIDE_LEFT, SIDE_RIGHT };

enum GameState {
  GAME_PLAYING,
  GAME_WON,
  GAME_LOST_TIME,
  GAME_LOST_TOO_MANY_MISSED
};

inline GameState CURRENT_GAME_STATE = GAME_PLAYING;
inline float CURRENT_TIME_LEFT = 10.0f;
inline int MISSED_CABLES_COUNT = 0;

void RestartMinigame2(const DifficultyConfig &config);
void RestartMinigame2();

class Plug {
public:
  int row;
  int column;
  Side side;
  float tick;       // seconds since the game started
  float explodeAt;  // when the whole system which includes 2 plugs and the wire
                    // explodes
  float resolvedAt; // time when the system was cut or exploded so we can fade
                    // it out
  bool ignore;      // true when the system was cut or exploded
  bool safe; // false only if this plug overheats, but its connected plug may be
             // safe.
  bool saved;   // true if the player cut the system before it exploded
  bool expired; // true if the plug turned grey without being cut
  Rectangle GetPlugRect() const;
  Vector2 GetWireAnchor() const;
  Plug(int newRow, int newColumn, Side newSide);
};

class Wire {
public:
  unsigned int plugIndex1;
  unsigned int plugIndex2;
  Wire(unsigned int newPlugIndex1, unsigned int newPlugIndex2);
};

class Plugs {
public:
  int rows;
  int columns;
  vector<Plug> plugs;
  vector<Wire> wires;

  void GeneratePlugs();
  void GenerateWires();
  void UpdatePlugs(float deltaTime);
  int UpdateWires(float deltaTime);
  void ResolveWire(Wire wire, bool saved);
  bool HasUnresolvedOverheating() const;
  Plugs();
};

class Slice {
public:
  Rectangle cell;
  float tick;
};

Rectangle GridPositionToRectangle(Vector2 gridPosition);
bool SlashWiresInGridPosition(Plugs &plugs, Vector2 gridPosition);
void UpdateSlices(vector<Slice> &slices, float deltaTime);

void DrawGrid();
void DrawPlugs(Plugs plugs);
void DrawWires(Plugs plugs);
void DrawSlices(vector<Slice> slices);
void DrawShockBar(int x, int y);
void DrawHud();

// Some utility functions
Vector2 MousePositionToGridPosition(Vector2 mousePosition);
bool CohenSutherlandLineClip(double &x0, double &y0, double &x1, double &y1,
                             double xmin, double ymin, double xmax,
                             double ymax);
Vector2 GetWireHeadPosition(Vector2 center, Side side);
