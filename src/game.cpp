#include "game.h"
#include <raylib.h>

void DrawGame(void) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("Whats up dawg?", 10, 10, 20, BLACK);
  EndDrawing();
}
