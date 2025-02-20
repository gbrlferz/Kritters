#ifndef KRIT_H
#define KRIT_H

#include <vector>
#include "raylib.h"

struct Krit {
  Vector2 position;
  Vector2 size = {15, 16};
  Color color;
  int type;
  // Constructor
  Krit(Vector2 pos, Color clr, int tp) : position(pos), color(clr), type(tp) {}
};

extern std::vector<Krit> krits;

bool TileEmpty(int x, int y);
void CreateRandomKrit();
void PopulateMapWithKrits(int amount);
void DeleteKrit(int x, int y);
bool CheckKrit(int x, int y);
int CheckPattern(Vector2 pos);
void DrawKrits();

#endif
