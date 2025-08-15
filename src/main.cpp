#include "config.h"
#include "game.h"
#include "raylib.h"

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Boids");

  while (!WindowShouldClose()) {
    // drawing
    BeginDrawing();
    DrawGame();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
