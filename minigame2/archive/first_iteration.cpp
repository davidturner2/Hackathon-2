// void Plugs::GenerateWires() {
//   vector<Plug> local_plugs = plugs;
//   while (local_plugs.size() > 0) {
//     unsigned int index1 = rand() % local_plugs.size();
//     Plug plug1 = local_plugs.at(index1);
//     local_plugs.erase(local_plugs.begin() + index1);
//     unsigned int index2 = rand() % local_plugs.size();
//     Plug plug2 = local_plugs.at(index2);
//     local_plugs.erase(local_plugs.begin() + index2);
//     wires.push_back(Wire(index1, index2));
//   }
// }

// int CalculateXInGrid(int coloumn, int offset) {
//   return coloumn * GRID_SIZE + ((GRID_SIZE - PLUG_WIDTH) / 2) + offset;
// }
//
// int CalculateYInGrid(int row, int offset) {
//   return row * GRID_SIZE + (GRID_SIZE - PLUG_HEIGHT) * (row != 0) + offset;
// }
//
// void DrawWires(Plugs plugs) {
//   int x1;
//   int y1;
//   int x2;
//   int y2;
//   // cout << endl;
//   for (const auto &wire : plugs.wires) {
//     if (wire.cacheRow1 == 0 || wire.cacheRow1 == plugs.rows - 1) {
//       x1 = CalculateXInGrid(wire.cachecoloumn1, PLUG_WIDTH / 2);
//       y1 = CalculateYInGrid(wire.cacheRow1, PLUG_HEIGHT / 2);
//     }
//     if (wire.cachecoloumn1 == 0 || wire.cachecoloumn1 == plugs.coloumns - 1)
//     {
//       x1 = CalculateYInGrid(wire.cachecoloumn1, PLUG_WIDTH / 2);
//       y1 = CalculateXInGrid(wire.cacheRow1, PLUG_HEIGHT / 2);
//     }
//
//     if (wire.cacheRow2 == 0 || wire.cacheRow2 == plugs.rows - 1) {
//       x2 = CalculateXInGrid(wire.cachecoloumn2, PLUG_WIDTH / 2);
//       y2 = CalculateYInGrid(wire.cacheRow2, PLUG_HEIGHT / 2);
//     }
//     if (wire.cachecoloumn2 == 0 || wire.cachecoloumn2 == plugs.coloumns - 1)
//     {
//       x2 = CalculateYInGrid(wire.cachecoloumn2, PLUG_WIDTH / 2);
//       y2 = CalculateXInGrid(wire.cacheRow2, PLUG_HEIGHT / 2);
//     }
//
//     // cout << x1 << ", " << y1 << ", " << x2 << ", " << y2 << endl;
//
//     DrawLine(x1 + MINIGAME_START_X, y1 + MINIGAME_START_Y,
//              x2 + MINIGAME_START_X, y2 + MINIGAME_START_Y, BLUE);
//   }
// }
//
// void DrawPlugs(Plugs plugs) {
//   int x;
//   int y;
//   int width;
//   int height;
//   for (const auto &plug : plugs.plugs) {
//     if (plug.row == 0 || plug.row == plugs.rows - 1) {
//       x = CalculateXInGrid(plug.coloumn, 0);
//       y = CalculateYInGrid(plug.row, 0);
//       width = PLUG_WIDTH;
//       height = PLUG_HEIGHT;
//     }
//     if (plug.coloumn == 0 || plug.coloumn == plugs.coloumns - 1) {
//       x = CalculateYInGrid(plug.coloumn, 0);
//       y = CalculateXInGrid(plug.row, 0);
//       width = PLUG_HEIGHT;
//       height = PLUG_WIDTH;
//     }
//     DrawRectangle(x + MINIGAME_START_X, y + MINIGAME_START_Y, width, height,
//                   BLUE);
//   }
// }