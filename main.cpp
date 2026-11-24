#include "main.h"
#include "minigame2/header.h"
#include "raylib.h"
// Thread + chrono are used to add delays for the first mini-game and works for
// Windows, Linux, and MacOS without any changes. Plus much neater and allows
// typing 3s for example. From https://stackoverflow.com/a/62047818
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

enum State {
  MINIGAME_1,
  MINIGAME_2,
};
State CURRENT_STATE = MINIGAME_1;
int MINIGAME_2_ITERATIONS = 0;

int BOAT_SPEED;
int SHIP_FORCE;
string PLUG_COLOR;

int main() {
  cout << "👋 Welcome to the Terminal, Player 1!" << endl;
  cout << "In this experience you will play several iterations of the same "
          "mini-games to target the production on the Calculating Empires map "
          "and ruin AI production. The more iterations of the mini-games you "
          "successfully complete, the higher your score will be!"
       << endl
       << endl;
  this_thread::sleep_for(3s);

  cout << "📏 Minigame 0: Algorithms" << endl;
  cout << "This first mini-game will only be played once. In this game, you "
          "will write an algorithm with the parameters that will affect the "
          "subsequent mini-games. "
          "Choose values wisely, as they can make your experience easier or "
          "harder!"
       << endl;
  cout << "🚢 First choose values for the first mini-game in which you will "
          "drive "
          "an autonomous cargo ship into an iceberg to make it crash. This "
          "ship carries microchips which are critical for AI usage. There will "
          "be slip streams that push your boat away from the iceberg, however "
          "your goal is to crash the ship."
       << endl
       << endl;
  cout << "  # TODO implement the speed of the cargo ship\n  speed = ";
  cin >> BOAT_SPEED;
  cout << endl
       << "  # TODO implement the force to push the cargo ship into the ice "
          "berg\n  force = ";
  cin >> SHIP_FORCE;
  this_thread::sleep_for(1.5s);
  cout << endl << "Values saved..." << endl << endl;
  this_thread::sleep_for(1.5s);
  cout << "🔌 The second mini-game will be about destroying a data center by "
          "un-pluging the wires that are overheating from excess computation. "
          "This "
          "relates to the Cloud Storage and Computing section of the Diagram, "
          "as "
          "AI heavily relies on cloud storage to store data like your chat "
          "histories and logs. Additionally, AI uses cloud computing as an LLM "
          "running on your local machine would likely be too slow. In this "
          "way, you send your message to a data-center which has many "
          "computers with higher specs that can run the LLM processes and "
          "return a response quicker."
       << endl;
  cout << endl
       << "  # TODO implement the color of the overheating plugs (red, yellow, "
          "green, blue, purple, orange, pink, brown, white)\n  color = ";
  cin >> PLUG_COLOR;
  SetDangerPlugColor(PLUG_COLOR);
  this_thread::sleep_for(1.5s);
  cout << endl << "Values saved..." << endl << endl;
  this_thread::sleep_for(1.5s);
  cout << "Calculating seed based on values chosen..." << endl;
  int seed = BOAT_SPEED * 43 + SHIP_FORCE * 17;
  for (const auto &c : PLUG_COLOR) {
    seed += static_cast<int>(c);
  }
  srand(seed);
  this_thread::sleep_for(3s);
  cout << "Seed calculated: " << seed << endl;
  this_thread::sleep_for(1.5s);
  cout << "Starting mini-games..." << endl;
  this_thread::sleep_for(1.5s);

  // Starting
  InitWindow(1000, 800, "hackathon example");
  SetTargetFPS(30);

  // minigame 2 initialization
  if (InitMinigame2() != 0)
    return -1;

  Vector2 mousePosition;
  Vector2 boatPosition = {100, 400};
  Color blue = {120, 240, 200, 255};
  Rectangle stream = {500, 300, 50, 200};
  // run every frame
  while (!WindowShouldClose()) {
    if (CURRENT_STATE == MINIGAME_1) {
      BeginDrawing();
      mousePosition = GetMousePosition();
      // background color
      ClearBackground(blue);
      // position= Vector2MoveTowards(Vector2 v, Vector2 target, float
      // maxDistance)
      if (IsMouseButtonDown(0) &&
          abs(mousePosition.y - boatPosition.y) > 5.1f &&
          mousePosition.y < 800 && mousePosition.y > 0) {
        if (mousePosition.y < boatPosition.y) {
          boatPosition.y -= 120.0f * GetFrameTime();
        } else {
          boatPosition.y += 120.0f * GetFrameTime();
        }
      }
      if (boatPosition.x > 1000) {
        boatPosition.x = -20;
      }

      // slipstream
      DrawRectangle(stream.x, stream.y, stream.width, stream.height,
                    (Color){58, 79, 23, 255});

      // cursor
      DrawCircle(mousePosition.x, mousePosition.y, 20,
                 (Color){255, 255, 255, 100});

      // boat
      DrawCircle(boatPosition.x, boatPosition.y, 20, WHITE);

      if (CheckCollisionCircleRec(boatPosition, 20, stream)) {
        boatPosition.y -= 100 * GetFrameTime();
        boatPosition.x += 20.5 * GetFrameTime();
      } else {
        boatPosition.x += 70.5 * GetFrameTime();
      }

      // ice berg
      DrawRectangle(800, 300, 20, 100, (Color){219, 233, 255, 255});
      EndDrawing();
    } else {
      NextFrame();
      SessionState sessionState = LevelUpIfNeeded();
      if (sessionState == SESSION_LOST) {
        cout << "Game over!" << endl;
        cout << "You reached level: " << MINIGAME_2_ITERATIONS << endl;
        break;
      }
      if (sessionState == SESSION_GAME_WON) {
        MINIGAME_2_ITERATIONS++;
      }
    }
  }
  /***
  //from raylibs example
deltaCircle.x += GetFrameTime()*6.0f*speed;
   your project code here!

   ***/

  // Minigame 2 stuff
  UnloadTexture(PLUG);

  return 0;
}