#include <cstddef>
#include <vector>
#include "include/raylib.h"
#include "include/krit.h"
#include "include/map.h"
#include "include/globals.h"
using namespace std;

vector<Krit> krits;

bool TileEmpty(int x, int y) {
  if (tileMap[y][x] != '.') { return false; } // Return false if space is not empty

  for (const auto& krit : krits) {
    // Return false if a krit is already there
    if (&krit != currentKrit && x == krit.position.x && y == krit.position.y) { return false; }
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

void DeleteKrit(int x, int y) {
  for (size_t k = 0; k < krits.size(); k++) {
    if (x == krits[k].position.x && y == krits[k].position.y) { krits.erase(krits.begin() + k); }
  }
}

bool CheckKrit(int x, int y) {
  for (size_t k = 0; k < krits.size(); k++) {
    if (x == krits[k].position.x && y == krits[k].position.y) { return true; }
  }
  return false;
}
