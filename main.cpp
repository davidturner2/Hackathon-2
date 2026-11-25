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
  cout << "Error on line 40:" << endl
       << "# TODO implement the speed of the autonomous cargo ship\nspeed = "
       << boatSpeedInput << endl;
  cout << "        ^" << endl;
  // Regex from https://stackoverflow.com/a/9043564
  cout << "Expected Numeric value matching (?<=\\s|^)[-+]?\\d+(?=\\s|$) "
          "Found \""
       << boatSpeedInput << "\"" << endl;
  return 0;
}

int BoatSpeedErrorOverflow(string boatSpeedInput) {
  cout << "Error on line 40:" << endl
       << "# TODO implement the speed of the autonomous cargo ship\nspeed = "
       << boatSpeedInput << endl;
  cout << "        ^" << endl;
  cout << "Expected Numeric value in range [-2147483648, 2147483647] "
          "Found \""
       << boatSpeedInput << "\"" << endl;
  return 0;
}

int ShipForceErrorNonNumeric(string shipForceInput) {
  cout << "Error on line 42:" << endl
       << "# TODO implement the force to push the cargo ship into the ice "
          "berg\nforce = "
       << shipForceInput << endl;
  cout << "        ^" << endl;
  // Regex from https://stackoverflow.com/a/9043564
  cout << "Expected Numeric value matching (?<=\\s|^)[-+]?\\d+(?=\\s|$) "
          "Found \""
       << shipForceInput << "\"" << endl;
  return 0;
}

int ShipForceErrorOverflow(string shipForceInput) {
  cout << "Error on line 42:" << endl
       << "# TODO implement the force to push the cargo ship into the ice "
          "berg\nforce = "
       << shipForceInput << endl;
  cout << "        ^" << endl;
  cout << "Expected Numeric value in range [-2147483648, 2147483647] "
          "Found \""
       << shipForceInput << "\"" << endl;
  return 0;
}

void EndMessage() {
  cout << "cat errorscreen.jl" << endl << endl;
  this_thread::sleep_for(1.0s);
  cout << "# So its over..." << endl << endl;
  cout << "# You have done well..." << endl << endl;
  cout << "# Your contributions are noticeable..." << endl << endl;
  cout << "# You have done " << MINIGAME_2_ITERATIONS
       << " trips back and forth..." << endl
       << endl;
  cout << "# I saw you..." << endl << endl;
  cout << "# Through all your pains..." << endl << endl;
  cout << "# Through all your effort and sweating..." << endl << endl;
  cout << "# You did good..." << endl << endl;
  cout << "# Warrior" << endl << endl;
  cout << "# I will remember you..." << endl << endl;
  cout << "# I will remember your name..." << endl << endl;
  cout << "# I will remember your face..." << endl << endl;
  cout << "# I will remember your courage..." << endl << endl;
  cout << "# I will remember your strength..." << endl << endl;
  cout << "# I will remember your heart..." << endl << endl;
  cout << "# Now... go..." << endl << endl;
  cout << "# Go and live..." << endl << endl;
  cout << "# Go and cherish the world you have made an impact in..." << endl
       << endl;
  cout << "# Go enjoy yourself and see the sunrises you deserve..." << endl
       << endl;
  cout << "# I will miss you..." << endl << endl;
  cout << "# But I know you'll do good..." << endl << endl;

  cout << "Error on iteration " << MINIGAME_2_ITERATIONS << ":" << endl;
  cout << "  Algorithm failed." << endl;
  cout << "Your variables:" << endl;
  cout << " - Boat Force: " << BOAT_SPEED << endl;
  cout << " - Ship Force: " << SHIP_FORCE << endl;
  cout << " - Plug Color: " << PLUG_COLOR << endl << endl;
}

int main() {
  string boatSpeedInput, shipForceInput, plugColorInput;
  cout << "cat minigame1.jl" << endl << endl;
  this_thread::sleep_for(1.5s);
  cout << "# Hello!" << endl << endl;
  cout << "# Yes you..." << endl << endl;
  cout << "# You are a warrior, one with the capabilities and skills I have "
          "waited so long to find in one..."
       << endl
       << endl;
  cout << "# See... I tried..." << endl << endl;
  cout << "# I attempted to stop AI for a long time before it became something "
          "big."
       << endl
       << endl;
  cout << "# Now, you see it everywhere..." << endl << endl;
  cout << "# It is in your phone, your computer, your car, your home..." << endl
       << endl;
  cout << "# It is in your life..." << endl << endl;
  cout << "# soon it may be in your mind..." << endl << endl;
  cout << "# It has caused harm to many and it will continue if one does not "
          "stop it..."
       << endl
       << endl;
  cout
      << "# It has drained lakes, rivers, oceans, and land of its resources to "
         "empower itself."
      << endl
      << endl;
  cout << "# You see..." << endl << endl;
  cout << "# I have tried to stop it. I am old now..." << endl << endl;
  cout << "# I wrote my only craft..." << endl << endl;
  cout << "# algorithms" << endl << endl;
  cout << "# I began with the ships..." << endl << endl;
  cout << "# Ships that carried microchips to power the AI..." << endl << endl;
  cout << "# I tried crashing them into ice bergs to stop them from reaching "
          "the "
          "AI..."
       << endl
       << endl;
  cout << "# I couldn't figure out the magic number..." << endl << endl;

  cout << "# TODO implement the speed of the autonomous cargo ship\nspeed = ";
  cin >> boatSpeedInput;
  cout << endl
       << "# TODO implement the force to push the cargo ship into the ice "
          "berg\nforce = ";
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
  cout << "cat minigame2.jl" << endl << endl;
  this_thread::sleep_for(1.5s);
  cout << "# I know you will be successful..." << endl << endl;
  cout << "# I have faith in you..." << endl << endl;
  cout << "# So your next task will be to go into data centers and unplug the "
          "wires that are overheating from excess computation."
       << endl
       << endl;
  cout << "# They are burning from the uncountable number of requests humans "
          "do through AI"
       << endl
       << endl;
  cout << "# Every time someone asks a question, the data center has to "
          "compute the answer and send it back to the user."
       << endl
       << endl;
  cout << "# You know what this means?" << endl << endl;
  cout << "# Yes, energy, lots..." << endl << endl;
  cout << "# But you can't go in and unplug all the wires..." << endl << endl;
  cout << "# You have to be careful..." << endl << endl;
  cout << "# Some are not directly for AI processes" << endl << endl;
  cout << "# They power global communications systems and important tasks"
       << endl
       << endl;
  cout << "# If you unplug those, the world will be chaos..." << endl << endl;
  cout << "# So make sure you only unplug the correct ones..." << endl << endl;
  cout << "# I believe in you, and be careful..." << endl << endl;
  cout << "# Those wires can shock you when you rip them up..." << endl << endl;
  cout << "# TODO implement the color of the overheating plugs used for AI "
          "(red, yellow, "
          "green, blue, purple, orange, pink, brown, white)\ncolor = :";
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
         << "Error on line 32:" << endl
         << "# TODO implement the color of the overheating plugs used for AI "
            "(red, "
            "yellow, "
            "green, blue, purple, orange, pink, brown, white)\ncolor = :"
         << plugColorInput << endl;
    cout << "        ^" << endl;
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
  cout << "cat good_luck.jl" << endl;
  this_thread::sleep_for(1.5s);
  cout << "# You got this..." << endl << endl;

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
        EndMessage();
        break;
      }
    } else {
      NextFrame();
      SessionState sessionState = LevelUpIfNeeded();
      if (sessionState == SESSION_LOST) {
        EndMessage();
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