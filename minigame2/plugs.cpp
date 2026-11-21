#include "header.h"
#include <cstdlib>

Plugs::Plugs(int newRows, int newColumns) {
  rows = newRows;
  columns = newColumns;
}

void Plugs::GeneratePlugs() {
  plugs.clear();
  // cout << "rows: " << rows << ", columns: " << columns << endl;
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