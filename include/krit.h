#ifndef KRIT_H
#define KRIT_H

#include <vector>
#include "raylib.h"

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
void DeleteKrit(int x, int y);
bool CheckKrit(int x, int y);
void CheckPattern(Vector2 pos);

#endif
