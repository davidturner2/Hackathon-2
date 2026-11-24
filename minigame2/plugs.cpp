#include "header.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

Plugs::Plugs() {
  rows = ROWS;
  columns = COLUMNS;
}

void Plugs::GeneratePlugs() {
  plugs.clear();
  for (int i = 1; i < rows - 1; i++) {
    plugs.push_back(Plug(i, 0, SIDE_LEFT));
    plugs.push_back(Plug(i, columns - 1, SIDE_RIGHT));
  }
  for (int i = 1; i < columns - 1; i++) {
    plugs.push_back(Plug(0, i, SIDE_TOP));
    plugs.push_back(Plug(rows - 1, i, SIDE_BOTTOM));
  }
}

void Plugs::GenerateWires() {
  wires.clear();

  // Copies the plugs into another vector so we can remove plugs as they are
  // paired up
  vector<unsigned int> remaining;
  for (unsigned int i = 0; i < plugs.size(); i++)
    remaining.push_back(i);

  while (!remaining.empty()) {
    unsigned int pos1 = rand() % remaining.size();
    unsigned int plugIndex1 = remaining[pos1];
    remaining.erase(remaining.begin() + pos1);

    // Filters the remaining plugs into another vector that only contains the
    // plugs that are not on the same side of the grid as the first plug chosen
    vector<unsigned int> candidates;
    for (unsigned int i = 0; i < remaining.size(); i++) {
      if (plugs[remaining[i]].side != plugs[plugIndex1].side) {
        candidates.push_back(i);
      }
    }

    if (candidates.empty()) {
      for (unsigned int i = 0; i < remaining.size(); i++) {
        candidates.push_back(i);
      }
    }

    unsigned int pos2 = candidates[rand() % candidates.size()];
    unsigned int plugIndex2 = remaining[pos2];
    remaining.erase(remaining.begin() + pos2);

    wires.push_back(Wire(plugIndex1, plugIndex2));
  }

  // Makes both plugs in a system connected by the same wire share the same
  // explodeAt time so that both disappear at the same time
  for (const auto &wire : wires) {
    Plug &plug1 = plugs[wire.plugIndex1];
    Plug &plug2 = plugs[wire.plugIndex2];

    if (plug1.safe && plug2.safe) {
      continue;
    }

    float shortest;
    if (plug1.safe) {
      shortest = plug2.explodeAt;
    } else if (plug2.safe) {
      shortest = plug1.explodeAt;
    } else {
      shortest = min(plug1.explodeAt, plug2.explodeAt);
    }
    plug1.explodeAt = shortest;
    plug2.explodeAt = shortest;
  }
}

void Plugs::UpdatePlugs(float deltaTime) {
  for (auto &plug : plugs) {
    plug.tick += deltaTime;
  }
}

// Updates every wire system. Returns how many systems expired this frame.
int Plugs::UpdateWires(float deltaTime) {
  int expired = 0;
  for (const auto &wire : wires) {
    Plug &plug1 = plugs[wire.plugIndex1];
    Plug &plug2 = plugs[wire.plugIndex2];

    bool systemSafe = plug1.safe && plug2.safe;
    if (plug1.ignore || systemSafe) {
      continue;
    }

    if (plug1.tick > plug1.explodeAt) {
      ResolveWire(wire, false);
      expired++;
    }
  }
  return expired;
}

void Plugs::ResolveWire(Wire wire, bool saved) {
  Plug &plug1 = plugs[wire.plugIndex1];
  Plug &plug2 = plugs[wire.plugIndex2];

  plug1.ignore = true;
  plug1.saved = saved;
  plug1.expired = !saved;
  plug1.resolvedAt = plug1.tick;

  plug2.ignore = true;
  plug2.saved = saved;
  plug2.expired = !saved;
  plug2.resolvedAt = plug2.tick;
}

// returns true if one system that is not safe is on the board
bool Plugs::HasUnresolvedOverheating() const {
  for (const auto &wire : wires) {
    const Plug &plug1 = plugs[wire.plugIndex1];
    const Plug &plug2 = plugs[wire.plugIndex2];

    if (!plug1.ignore && (!plug1.safe || !plug2.safe)) {
      return true;
    }
  }
  return false;
}
