#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include "raylib.h"

extern int MAP_ROWS;
extern int MAP_COLS;
extern Vector2 grid_offset;

// Declare tileMap as an external variable
extern std::vector<std::string> tileMap;

void InitMap();
void DrawMap();

#endif // MAP_H
