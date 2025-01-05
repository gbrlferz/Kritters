#include <vector>
#include "include/raylib.h"
#include "include/krit.h"
#include "include/map.h"
using namespace std;

vector<Krit> krits;

bool TileEmpty(int x, int y) {
  for (const auto& krit : krits) {
    // Return false if space is not empty
    if (tileMap[y][x] != '.') { return false; }
    // Return false if a krit is already there
    if (x == krit.position.x && y == krit.position.y) { return false; }
  }
  return true;
}

void CreateRandomKrit() {
  while (true) {
    int x = rand() % tileMap[0].size();
    int y = rand() % tileMap.size();
    if (TileEmpty(x, y)) {
      krits.push_back(Krit({static_cast<float>(x), static_cast<float>(y)}, MAROON));
      break;
    }
  }
}

void PopulateMapWithKrits(int amount) {
  for (int i = 0; i < amount; i++) { CreateRandomKrit(); }
}
