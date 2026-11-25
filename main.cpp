#include "main.h"
#include "minigame2/header.h"
#include "minigameboat.h"
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

int BoatSpeedErrorNonNumeric(string boatSpeedInput) {
  cout << "Error on line 2:" << endl
       << "  # TODO implement the speed of the cargo ship\n  speed = "
       << boatSpeedInput << endl;
  cout << "          ^" << endl;
  // Regex from https://stackoverflow.com/a/9043564
  cout << "Expected Numeric value matching (?<=\\s|^)[-+]?\\d+(?=\\s|$) "
          "Found \""
       << boatSpeedInput << "\"" << endl;
  return 0;
}

int BoatSpeedErrorOverflow(string boatSpeedInput) {
  cout << "Error on line 2:" << endl
       << "  # TODO implement the speed of the cargo ship\n  speed = "
       << boatSpeedInput << endl;
  cout << "          ^" << endl;
  cout << "Expected Numeric value in range [-2147483648, 2147483647] "
          "Found \""
       << boatSpeedInput << "\"" << endl;
  return 0;
}

int ShipForceErrorNonNumeric(string shipForceInput) {
  cout << "Error on line 4:" << endl
       << "  # TODO implement the force to push the cargo ship into the ice "
          "berg\n  force = "
       << shipForceInput << endl;
  cout << "          ^" << endl;
  // Regex from https://stackoverflow.com/a/9043564
  cout << "Expected Numeric value matching (?<=\\s|^)[-+]?\\d+(?=\\s|$) "
          "Found \""
       << shipForceInput << "\"" << endl;
  return 0;
}

int ShipForceErrorOverflow(string shipForceInput) {
  cout << "Error on line 4:" << endl
       << "  # TODO implement the force to push the cargo ship into the ice "
          "berg\n  force = "
       << shipForceInput << endl;
  cout << "          ^" << endl;
  cout << "Expected Numeric value in range [-2147483648, 2147483647] "
          "Found \""
       << shipForceInput << "\"" << endl;
  return 0;
}

int main() {
  string boatSpeedInput, shipForceInput, plugColorInput;
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
  cin >> boatSpeedInput;
  cout << endl
       << "  # TODO implement the force to push the cargo ship into the ice "
          "berg\n  force = ";
  cin >> shipForceInput;
  this_thread::sleep_for(1.5s);
  cout << endl << "Running..." << endl << endl;
  this_thread::sleep_for(1.5s);

  // Validation logic from
  // https://en.cppreference.com/cpp/string/basic_string/stol
  size_t pos{};
  try {
    BOAT_SPEED = stoi(boatSpeedInput, &pos);
    if (pos < boatSpeedInput.size()) {
      return BoatSpeedErrorNonNumeric(boatSpeedInput);
    }
  } catch (invalid_argument const &ex) {
    return BoatSpeedErrorNonNumeric(boatSpeedInput);
  } catch (out_of_range const &ex) {
    return BoatSpeedErrorOverflow(boatSpeedInput);
  }

  try {
    SHIP_FORCE = stoi(shipForceInput, &pos);
    if (pos < shipForceInput.size()) {
      return ShipForceErrorNonNumeric(shipForceInput);
    }
  } catch (invalid_argument const &ex) {
    return ShipForceErrorNonNumeric(shipForceInput);
  } catch (out_of_range const &ex) {
    return ShipForceErrorOverflow(shipForceInput);
  }

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
          "green, blue, purple, orange, pink, brown, white)\n  color = :";
  cin >> plugColorInput;

  this_thread::sleep_for(1.5s);
  cout << endl << "Running..." << endl << endl;
  this_thread::sleep_for(1.5s);

  if (plugColorInput == "red" || plugColorInput == "yellow" ||
      plugColorInput == "green" || plugColorInput == "blue" ||
      plugColorInput == "purple" || plugColorInput == "orange" ||
      plugColorInput == "pink" || plugColorInput == "brown" ||
      plugColorInput == "white") {
    PLUG_COLOR = plugColorInput;
  } else {
    cout << endl
         << "Error on line 6:" << endl
         << "  # TODO implement the color of the overheating plugs (red, "
            "yellow, "
            "green, blue, purple, orange, pink, brown, white)\n  color = :"
         << plugColorInput << endl;
    cout << "          ^" << endl;
    cout << "Expected one of [:red, :yellow, :green, :blue, :purple, :orange, "
            ":pink, "
            ":brown, :white] Found \":"
         << plugColorInput << "\"" << endl;
    return 0;
  }

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

  Minigameboat hi = Minigameboat(BOAT_SPEED, SHIP_FORCE);
  // run every frame
  while (!WindowShouldClose()) {
    if (CURRENT_STATE == MINIGAME_1) {
      hi.update();
      if (hi.won) {
        CURRENT_STATE = MINIGAME_2;
        hi.won = false;
      }
      if (hi.lose) {

        cout << "Game over!" << endl;
        cout << "You reached level: " << MINIGAME_2_ITERATIONS << endl;
        break;
      }
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
        CURRENT_STATE = MINIGAME_1;
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