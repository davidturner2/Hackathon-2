#include "header.h"
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

    unsigned int pos2 = candidates[rand() % candidates.size()];
    unsigned int plugIndex2 = remaining[pos2];
    remaining.erase(remaining.begin() + pos2);

    wires.push_back(Wire(plugIndex1, plugIndex2));
  }
}

void Plugs::UpdatePlugs(float deltaTime) {
  for (auto &plug : plugs) {
    plug.explodedThisFrame = false;
    plug.tick += deltaTime;
    if (plug.tick > plug.explodeAt && !plug.safe) {
      plug.ignore = true;
      plug.explodedThisFrame = true;
    }
  }
}

void Plugs::UpdateWires(float deltaTime) {
  for (const auto &wire : wires) {
    Plug &plug1 = plugs[wire.plugIndex1];
    Plug &plug2 = plugs[wire.plugIndex2];

    if (plug1.explodedThisFrame &&
        (plug2.tick < plug2.explodeAt || plug2.safe)) {
      plug2.explodeAt = plug2.tick + (plug1.tick - plug1.explodeAt);
      plug2.safe = false;
    }
    if (plug2.explodedThisFrame &&
        (plug1.tick < plug1.explodeAt || plug1.safe)) {
      plug1.explodeAt = plug1.tick + (plug2.tick - plug2.explodeAt);
      plug1.safe = false;
    }
  }
}