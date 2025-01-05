#include <math.h>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <string>
#include "include/cursor.h"
#include "include/krit.h"
#include "include/map.h"
#include "include/raylib.h"

using namespace std;

float tile_size = 16;

vector<string> tileMap;

Cursor cursor = Cursor({4, 4}, {tile_size - 1, tile_size - 1});

int main(void) {
  // INITIALIZATION
  ifstream file("resources/level.txt");
  string str;
  while (getline(file, str)) { tileMap.push_back(str); };

  // Calculate map dimensions
  int MAP_ROWS = tileMap.size();
  int MAP_COLS = tileMap[0].size();

  const int screenWidth = 1280;
  const int screenHeight = 720;

  const int virtualScreenWidth = 320;
  const int virtualScreenHeight = 180;

  // GRID
  Vector2 grid_offset = {5, 8};

  bool debug = false;

  const float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

  InitWindow(screenWidth, screenHeight, "Krazy Kreatures");

  Camera2D worldSpaceCamera = {0};  // Game world camera
  worldSpaceCamera.zoom = 1.0f;

  // This is where we'll draw all our objects.
  RenderTexture2D target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight);

  Rectangle sourceRec = {0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height};
  Rectangle destRec = {-virtualRatio, -virtualRatio, screenWidth + (virtualRatio * 2), screenHeight + (virtualRatio * 2)};

  Vector2 origin = {0.0f, 0.0f};

  float dt = 0.0f;
  float spawnTime = 0.0f;

  SetTargetFPS(60);

  Krit* currentKrit = nullptr;

  PopulateMapWithKrits(10);

  // MAIN GAME LOOP
  while (!WindowShouldClose()) {
    // UPDATE
    dt += GetFrameTime();
    spawnTime += GetFrameTime();

    if (currentKrit) { currentKrit->position = cursor.position; }

    if (IsKeyPressed(KEY_GRAVE)) { debug = !debug; }

    if (spawnTime >= 1.0f) {
      CreateRandomKrit();
      spawnTime = 0.0f;
    }

    // GAMEPLAY

    // Movement
    if (IsKeyPressed(KEY_UP)) {
      if (cursor.position.y > 0 && tileMap[cursor.position.y - 1][cursor.position.x] == '.') { cursor.position.y--; }
    }
    if (IsKeyPressed(KEY_DOWN)) {
      if (cursor.position.y < MAP_ROWS - 1 && tileMap[cursor.position.y + 1][cursor.position.x] == '.') { cursor.position.y++; }
    }
    if (IsKeyPressed(KEY_LEFT)) {
      if (cursor.position.x > 0 && tileMap[cursor.position.y][cursor.position.x - 1] == '.') { cursor.position.x--; }
    }
    if (IsKeyPressed(KEY_RIGHT)) {
      if (cursor.position.x < MAP_COLS - 1 && tileMap[cursor.position.y][cursor.position.x + 1] == '.') { cursor.position.x++; }
    }

    // Selection
    if (IsKeyPressed(KEY_Z)) {
      if (currentKrit) {
        currentKrit = nullptr;
      } else {
        for (size_t k = 0; k < krits.size(); k++) {
          if (cursor.position.x == krits[k].position.x && cursor.position.y == krits[k].position.y) { 
            currentKrit = &krits[k];
          };
        }
      }
    }

    // DRAW
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    BeginMode2D(worldSpaceCamera);

    for (int w = 0; w < MAP_ROWS; w++) {
      for (int h = 0; h < MAP_COLS; h++) {
        Vector2 position = {h * tile_size + grid_offset.x, w * tile_size + grid_offset.y};
        char tile = tileMap[w][h];
        switch (tile) {
          case '.':
            DrawRectangle(position.x, position.y, tile_size - 1, tile_size - 1, LIGHTGRAY);
            break;
          case '#':
            DrawRectangle(position.x, position.y, tile_size - 1, tile_size - 1, DARKGRAY);
            break;
        }
      }
    }

    for (size_t k = 0; k < krits.size(); k++) {
      Krit krit = krits[k];
      Vector2 position = {krit.position.x * tile_size + grid_offset.x, krit.position.y * tile_size + grid_offset.y};
      DrawRectangle(position.x, position.y, krit.size.x, krit.size.y - 1, krit.color);
    }

    Rectangle cursorRec = {cursor.position.x * tile_size + grid_offset.x,
                           cursor.position.y * tile_size + grid_offset.y,
                           cursor.size.x,
                           cursor.size.y
    };

    DrawRectangleRoundedLinesEx(cursorRec, 0.1f, 16.0f, 1.0f + fabs(sin(dt * 5.0f)), RED);

    EndMode2D();
    EndTextureMode();

    BeginDrawing();

    ClearBackground(RED);
    DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

    EndMode2D();

    if (debug) {
      DrawText(TextFormat("Screen resolution: %ix%i", screenWidth, screenHeight), 10, 10, 20, DARKBLUE);
      DrawText(TextFormat("World resolution: %ix%i", virtualScreenWidth, virtualScreenHeight), 10, 40, 20, DARKGREEN);
      DrawFPS(GetScreenWidth() - 95, 10);
    }

    EndDrawing();
  }

  // DE-INITIALIZATION
  UnloadRenderTexture(target);  // Unload render texture
  CloseWindow();                // Close window and OpenGL context
  return 0;
}
