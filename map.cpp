#include "include/map.h"
#include <fstream>
#include <string>
#include <vector>
#include "include/globals.h"
#include "include/raylib.h"
using namespace std;

vector<string> tileMap;

int MAP_ROWS;
int MAP_COLS;

Vector2 grid_offset = {5, 8};

void InitMap() {
  ifstream file("resources/level.txt");
  string str;
  while (getline(file, str)) { tileMap.push_back(str); };

  // Calculate map dimensions
  MAP_ROWS = tileMap.size();
  MAP_COLS = tileMap[0].size();
}

void DrawMap() {
  for (int w = 0; w < MAP_ROWS; w++) {
    for (int h = 0; h < MAP_COLS; h++) {
      Vector2 position = {h * TILE_SIZE + grid_offset.x, w * TILE_SIZE + grid_offset.y};
      char tile = tileMap[w][h];
      switch (tile) {
        case '.':
          DrawRectangle(position.x, position.y, TILE_SIZE - 1, TILE_SIZE - 1, LIGHTGRAY);
          break;
        case '#':
          DrawRectangle(position.x, position.y, TILE_SIZE - 1, TILE_SIZE - 1, DARKGRAY);
          break;
      }
    }
  }
}
