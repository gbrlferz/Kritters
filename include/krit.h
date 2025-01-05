#ifndef KRIT_H
#define KRIT_H

#include "raylib.h"
#include <vector>

struct Krit {
  Vector2 position;
  Vector2 size = {15, 16};
  Color color;
  // Constructor
  Krit(Vector2 pos, Color clr) : position(pos), color(clr) {}
};

extern std::vector<Krit> krits;

bool TileEmpty(int x, int y);
void CreateRandomKrit();
void PopulateMapWithKrits(int amount);

#endif
