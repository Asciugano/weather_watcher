#include <raylib.h>
#include <stdio.h>

#include "./lib/weather.h"

int main(int argc, char *argv[]) {

  InitWindow(800, 600, "Weather Watcher");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});
    fetch_weather_data();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
