// headers here ...
#include "raylib.h"
#include <vector>

using namespace std;

inline Texture2D PLUG;

inline int MINIGAME_START_X = 10;
inline int MINIGAME_START_Y = 10;
inline int PLUG_WIDTH = 50;
inline int PLUG_HEIGHT = 30;
inline int GRID_SIZE = 70;

const int GAME_SIZE = 600;
const int PADDING = 10;

inline int ROWS = 5;
inline int COLUMNS = 5;

const Color WIRE_COLOR = {203, 212, 216, 255};

inline int NUM_PLUGS = 0;
inline int NUM_EXPLODING_PLUGS = 0;

inline int setup(int rows, int columns) {
  ROWS = rows;
  COLUMNS = columns;
  GRID_SIZE = GAME_SIZE / max(rows, columns);
  PLUG_WIDTH = GRID_SIZE * 0.7;
  PLUG_HEIGHT = GRID_SIZE * 0.4;

  NUM_PLUGS = ROWS * 2 + COLUMNS * 2 - 8;
  NUM_EXPLODING_PLUGS = 0;
  return 0;
}

// Previously we used some math to check if the wire was on the left or right
// side of the grid, or top or bottom. We decided to use an enum since it can
// easily represent what side the plug is on and make future math more clear:
// For example we had something like:
// int CalculateYInGrid(int row, int offset) {
//     return row*GRID_SIZE + (GRID_SIZE-PLUG_HEIGHT)*(row != 0) + offset;
// }
// Where (row != 0) would check if the plug was on top (row == 0), which would
// align the plug with the top of the cell, and other wise would align it at the
// bottom of the cell for anything else. And given each row only has two plugs
// it could only be top of bottom aligned. Initially wanted to only use concepts
// learned in class, but it got very un-readable :)
enum Side { SIDE_TOP, SIDE_BOTTOM, SIDE_LEFT, SIDE_RIGHT };

class Plug {
public:
  int row;
  int column;
  Side side;
  float tick;
  float explodeAt;
  bool explodedThisFrame;
  bool ignore;
  bool safe;
  bool saved;
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
  void UpdateWires(float deltaTime);
  Plugs();
};

void DrawGrid();
void DrawPlugs(Plugs plugs);
void DrawWires(Plugs plugs);
void DrawHud();

// Some utility functions
Vector2 MousePositionToGridPosition(Vector2 mousePosition);
bool CohenSutherlandLineClip(double &x0, double &y0, double &x1, double &y1,
                             double xmin, double ymin, double xmax,
                             double ymax);
Vector2 GetWireHeadPosition(Vector2 center, Side side);