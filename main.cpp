#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include "include/cursor.h"
#include "include/raylib.h"

float tile_size = 16;

std::vector<std::string> tileMap;

Cursor cursor = Cursor({0, 0}, {tile_size - 1, tile_size - 1});

struct Krit {
 public:
  Vector2 position;
  Vector2 size;
  Color color;
  // Constructor
  Krit(Vector2 pos, Vector2 sz, Color clr) : position(pos), size(sz), color(clr){}
};

int main(void) {
  // INITIALIZATION

  std::ifstream file("resources/level.txt");
  std::string str;
  while (std::getline(file, str)) { tileMap.push_back(str); };

  // Calculate map dimensions
  int MAP_ROWS = tileMap.size();
  int MAP_COLS = tileMap[0].size();

  const int screenWidth = 1280;
  const int screenHeight = 720;

  const int virtualScreenWidth = 320;
  const int virtualScreenHeight = 180;

  std::vector<Krit> krits;

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

  float time = 0.0f;

  SetTargetFPS(60);

  // MAIN GAME LOOP
  while (!WindowShouldClose()) {  // Detect window close button or ESC key
    // UPDATE
    time += GetFrameTime();

    if (IsKeyPressed(KEY_GRAVE)) { debug = !debug; }

    // Movement
    if (IsKeyPressed(KEY_DOWN)) {
      if (tileMap[cursor.position.y + 1][cursor.position.x] == '.') { cursor.position.y++; }
    }
    if (IsKeyPressed(KEY_UP)) {
      if (tileMap[cursor.position.y - 1][cursor.position.x] == '.') { cursor.position.y--; }
    }
    if (IsKeyPressed(KEY_RIGHT)) {
      if (tileMap[cursor.position.y][cursor.position.x + 1] == '.') { cursor.position.x++; }
    }
    if (IsKeyPressed(KEY_LEFT)) {
      if (tileMap[cursor.position.y][cursor.position.x - 1] == '.') { cursor.position.x--; }
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
            DrawRectangle(position.x, position.y, tile_size - 1, tile_size - 1, BLUE);
            break;
          case '#':
            DrawRectangle(position.x, position.y, tile_size - 1, tile_size - 1, BLACK);
            break;
        }
      }
    }

    Rectangle cursorRec = {cursor.position.x * tile_size + grid_offset.x, cursor.position.y * tile_size + grid_offset.y, cursor.size.x,
                           cursor.size.y};
    DrawRectangleRoundedLinesEx(cursorRec, 0.1f, 16.0f, 1.0f + fabs(sin(time * 5.0f)), RED);

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
