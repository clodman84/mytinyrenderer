#include "raylib.h"
#include "resource_dir.h"

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(1280, 800, "Hello Raylib");
  SearchAndSetResourceDir("resources");

  Camera camera = {0};
  camera.position = (Vector3){25.0f, 25.0f, 25.0f};
  camera.target = (Vector3){0.0f, 10.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  Model model = LoadModel("suzanne.obj");

  Vector3 position = {0.0f, 0.0f, 0.0f};
  BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);

  // game loop
  while (!WindowShouldClose()) {
    // drawing
    BeginDrawing();

    BeginMode3D(camera);
    ClearBackground(BLACK);
    DrawModel(model, position, 1.0f, WHITE);
    EndMode3D();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
