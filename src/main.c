#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./lib/weather.h"

WeatherData data;

int main(int argc, char *argv[]) {

  char *city = "Milan";

  if (argc < 2) {
    printf("Errore: devi inserire anche la citta'\n");
    return 1;
  } else {
    city = argv[1];
  }

  InitWindow(800, 600, "Weather Watcher");
  SetTargetFPS(60);
  bool fetched = false;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});

    if (!fetched) {
      data = fetch_weather_data(city);
      fetched = true;
    }

    if (IsKeyPressed(KEY_R)) {
      data = fetch_weather_data(city);
    }

    DrawWeatherInfo(10, 10, data);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
