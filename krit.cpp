#include "include/krit.h"
#include <cmath>
#include <cstddef>
#include <vector>
#include "include/globals.h"
#include "include/map.h"
#include "include/raylib.h"
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
  Color pink = GetColor(0xfe626eFF);
  Color B5 = GetColor(0xa4f022FF);
  vector<Color> colors = {pink, B5};
  while (true) {
    int x = rand() % tileMap[0].size();
    int y = rand() % tileMap.size();
    if (TileEmpty(x, y)) {
      int kritColor = rand() % colors.size();
      krits.push_back(Krit({static_cast<float>(x), static_cast<float>(y)}, colors[kritColor]));
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

int CheckPattern(Vector2 pos) {
  // TODO: Simplify

  bool xPatternMade = false;
  bool yPatternMade = false;

  int min_x = 0;
  int max_x = 0;
  int min_y = 0;
  int max_y = 0;

  // Check horizontal
  for (int i = 0; i < INFINITY; i++) {
    if (CheckKrit(pos.x - i, pos.y)) {
      min_x--;
    } else { break; }
  }

  for (int i = 0; i < INFINITY; i++) {
    if (CheckKrit(pos.x + i, pos.y)) {
      max_x++;
    } else { break; }
  }

  int dist_x = abs(min_x) + max_x;

  // Check vertical
  for (int i = 0; i < INFINITY; i++) {
    if (CheckKrit(pos.x, pos.y + i)) {
      max_y++;
    } else { break; }
  }

  for (int i = 0; i < INFINITY; i++) {
    if (CheckKrit(pos.x, pos.y - i)) {
      min_y--;
    } else { break; }
  }

  int dist_y = abs(min_y) + max_y;

  if (dist_y >= 4) {
    for (int i = min_y; i < max_y; i++) {
      DeleteKrit(pos.x, pos.y + i);
    }
    yPatternMade = true;
  }
  if (dist_x >= 4) {
    for (int i = min_x; i < max_x; i++) {
      DeleteKrit(pos.x + i, pos.y);
    }
    xPatternMade = true;
  }

  if (xPatternMade && yPatternMade) {
    int points = dist_x + dist_y - 3;
    return points * 10 * points;
  } else if (xPatternMade) {
    return (dist_x - 1) * 10;
  } else if (yPatternMade) {
    return (dist_y - 1) * 10;
  } else {
    return 0;
  }
}

void DrawKrits() {
  for (size_t k = 0; k < krits.size(); k++) {
    Krit krit = krits[k];
    Vector2 position = {krit.position.x * TILE_SIZE + grid_offset.x, krit.position.y * TILE_SIZE + grid_offset.y};
    DrawRectangle(position.x, position.y, krit.size.x, krit.size.y - 1, krit.color);
  }
}
