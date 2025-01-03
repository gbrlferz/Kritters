#include <string>
#include <vector>
#include "../include/raylib.h"

struct Cursor {
  Vector2 position;
  Vector2 size;
  // Constructor
  Cursor(Vector2 pos, Vector2 sz) : position(pos), size(sz) {}
};
