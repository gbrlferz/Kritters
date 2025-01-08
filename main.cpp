#include <math.h>
#include <cstddef>
#include <cstdlib>
#include "include/cursor.h"
#include "include/globals.h"
#include "include/krit.h"
#include "include/map.h"
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"
#include "include/raylib.h"
using namespace std;

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY } GameScreen;

Krit* currentKrit = nullptr;

// DEBUG
bool debug = false;
bool showRes = false;
float value = 0.5f;

int score = 0;

int TILE_SIZE = 16;

void AddPoints(int points) { score += points; }

int main(void) {
  // INITIALIZATION //
  const float ratio = 4.0 / 3.0;
  const int resolution = 720;
  const int screenWidth = resolution * ratio;
  const int screenHeight = resolution;

  const int virtualScreenWidth = 240;
  const int virtualScreenHeight = 180;

  const float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

  InitWindow(screenWidth, screenHeight, "Krazy Kreatures");

  InitMap();

  GameScreen currentScreen = LOGO;

  int framesCounter = 0;  // Useful to count frames

  Camera2D worldSpaceCamera = {0};  // Game world camera
  worldSpaceCamera.zoom = 1.0f;

  Cursor cursor = Cursor({4, 4}, {static_cast<float>(TILE_SIZE - 1), static_cast<float>(TILE_SIZE - 1)});

  // This is where we'll draw all our objects.
  RenderTexture2D target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight);

  Rectangle sourceRec = {0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height};
  Rectangle destRec = {-virtualRatio, -virtualRatio, screenWidth + (virtualRatio * 2), screenHeight + (virtualRatio * 2)};

  Texture2D titleScreen = LoadTexture("resources/title_screen.png");

  Vector2 origin = {0.0f, 0.0f};

  float dt = 0.0f;
  float spawnTime = 0.0f;

  SetTargetFPS(60);

  PopulateMapWithKrits(10);

  // MAIN GAME LOOP //
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_GRAVE)) { debug = !debug; }
    // UPDATE //
    switch (currentScreen) {
      case LOGO: {
        framesCounter++;  // Count frames
        if (framesCounter > 120) { currentScreen = TITLE; }
      } break;
      case TITLE: {
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) { currentScreen = GAMEPLAY; }
      } break;
      case GAMEPLAY: {
        dt += GetFrameTime();
        spawnTime += GetFrameTime();

        if (spawnTime >= 1.0f) {
          CreateRandomKrit();
          spawnTime = 0.0f;
        }

        if (currentKrit) { currentKrit->position = cursor.position; }

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
          // Deselect krit
          if (currentKrit && TileEmpty(cursor.position.x, cursor.position.y)) {
            currentKrit = nullptr;
            AddPoints(CheckPattern(cursor.position));
          }
          // Select krit
          else {
            for (size_t k = 0; k < krits.size(); k++) {
              if (cursor.position.x == krits[k].position.x && cursor.position.y == krits[k].position.y) { currentKrit = &krits[k]; };
            }
          }
        }
      } break;
    }

    // DRAW
    BeginTextureMode(target);
    ClearBackground(BLACK);
    BeginMode2D(worldSpaceCamera);

    switch (currentScreen) {
      case LOGO: {
        DrawText("Gabiru", 20, 20, 20, LIGHTGRAY);
      } break;
      case TITLE: {
        DrawTexture(titleScreen, 0, 0, WHITE);
      } break;
      case GAMEPLAY: {
        DrawMap();
        for (size_t k = 0; k < krits.size(); k++) {
          Krit krit = krits[k];
          Vector2 position = {krit.position.x * TILE_SIZE + grid_offset.x, krit.position.y * TILE_SIZE + grid_offset.y};
          DrawRectangle(position.x, position.y, krit.size.x, krit.size.y - 1, krit.color);
        }
        Rectangle cursorRec = {cursor.position.x * TILE_SIZE + grid_offset.x, cursor.position.y * TILE_SIZE + grid_offset.y, cursor.size.x,
                               cursor.size.y};
        DrawRectangleRoundedLinesEx(cursorRec, 0.1f, 16.0f, 1.0f + fabs(sin(dt * 5.0f)), RED);
        DrawText(TextFormat("Score: %i", score), 4, 4, 4, DARKBLUE);
      } break;
    }

    EndMode2D();
    EndTextureMode();

    BeginDrawing();

    ClearBackground(RED);
    DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

    EndMode2D();

    if (debug) {
      if (GuiButton((Rectangle){16, 16, 256, 32}, "#191#Show resolution")) { showRes = !showRes; }
      if (showRes) {
        DrawText(TextFormat("Screen resolution: %ix%i", screenWidth, screenHeight), 16, 48, 20, DARKBLUE);
        DrawText(TextFormat("World resolution: %ix%i", virtualScreenWidth, virtualScreenHeight), 16, 80, 20, DARKGREEN);
      }
      DrawFPS(GetScreenWidth() - 95, 10);
    }

    EndDrawing();
  }

  // DE-INITIALIZATION
  UnloadRenderTexture(target);  // Unload render texture
  CloseWindow();                // Close window and OpenGL context
  return 0;
}
